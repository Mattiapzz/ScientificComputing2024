#include <BlockMult.hh>

#define TH_POOL 1

BlockMult::BlockMult(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p)
  : pool{10}
{
  // avoid copy of the matrices
  A_blocks.clear();
  B_blocks.clear();
  C_blocks.clear();
  //
  m_n = n;
  m_m = m;
  m_p = p;
  //
  if(!checkValidity(A,B,C))
  {
    std::cerr << "Invalid matrix multiplication!!!" << std::endl; 
    return;
  }
  //
  BlockMultiplication(A,B,C);
  //
  return;
}

BlockMult::~BlockMult()
{
  A_blocks.clear();
  B_blocks.clear();
  C_blocks.clear();
  return;
}


void
BlockMult::multiply(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p)
{
  A_blocks.clear();
  B_blocks.clear();
  C_blocks.clear();
  //
  m_n = n;
  m_m = m;
  m_p = p;
  //
  if(!checkValidity(A,B,C))
  {
    std::cerr << "Invalid matrix multiplication!!!" << std::endl; 
    return;
  }
  //
  BlockMultiplication(A,B,C);
  //
  return;
}


void
BlockMult::BlockMultiplication(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
  int a_n_row, a_p_col, b_p_row, b_m_col, c_n_row, c_m_col; 
  std::vector<std::future<void>> futures;
  //
  a_n_row = A.rows() / m_n;
  a_p_col = A.cols() / m_p;
  b_p_row = B.rows() / m_p;
  b_m_col = B.cols() / m_m;
  c_n_row = C.rows() / m_n;
  c_m_col = C.cols() / m_m;
  //
  A_blocks.resize(a_n_row);
  B_blocks.resize(b_p_row);
  C_blocks.resize(c_n_row);
  //
  for(int i = 0; i < a_n_row; i++) A_blocks[i].resize(a_p_col);
  for(int i = 0; i < b_p_row; i++) B_blocks[i].resize(b_m_col);
  for(int i = 0; i < c_n_row; i++) C_blocks[i].resize(c_m_col);
  //
  for(int i = 0; i < c_n_row; i++)
  {
    for(int j = 0; j < c_m_col; j++)
    {
      #if TH_POOL
      futures.push_back(
        pool.submit_task(
          [this, &A, &B, &C, i, j](){this->ComputeCBlock( A, B, C, i, j);})
        );
      #else
      ComputeCBlock( A, B, C, i, j);
      #endif
    }
  }
  // pool.wait();
  for(auto &f : futures) f.get();
}

void
BlockMult::ComputeCBlock(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, int i, int j)
{
  // race condition problem if matrices are too small
  // dead lock betwee A and B
  //
  // std::lock_guard<std::mutex> lockC(mtxC);
  if(C_blocks[i][j].alreadyComputed) return;
  C_blocks[i][j].BM = C.block( i * C.rows() / m_n, j * C.cols() / m_m, m_n, m_m);
  for(int k = 0; k < A.cols()/m_p; k++)
  {
    if(!A_blocks[i][k].alreadyComputed)
    {
      // std::lock_guard<std::mutex> lockA(mtxA);
      A_blocks[i][k].BM = A.block( i * A.rows() / m_n, k * A.cols() / m_p, m_n, m_p );
      A_blocks[i][k].alreadyComputed = true;
    }
    if(!B_blocks[k][j].alreadyComputed)
    {
      // std::lock_guard<std::mutex> lockB(mtxB);
      B_blocks[k][j].BM = B.block( k * B.rows() / m_p, j * B.cols() / m_m, m_p, m_m );
      B_blocks[k][j].alreadyComputed = true;
    }
    C_blocks[i][j].BM += A_blocks[i][k].BM * B_blocks[k][j].BM;
  }
  C_blocks[i][j].alreadyComputed = true;
  return;
}

bool 
BlockMult::checkValidity(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
  if(A.cols() != B.rows())
  {
    std::cerr << "Invalid matrix multiplication. Found " << A.cols() << "x" << A.rows() << " Times " << B.cols() << "x" << B.rows() << std::endl; 
    return false;
  }
  if(A.rows() != C.rows() || B.cols() != C.cols())
  {
    std::cerr << "Invalid matrix multiplication. Found " << A.rows() << "x" << A.cols() << " Times " << B.rows() << "x" << B.cols() << " Expected to be " << C.rows() << "x" << C.cols() << std::endl;
    return false;
  }
  return true;
}