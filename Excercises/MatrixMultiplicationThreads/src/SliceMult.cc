#include <SliceMult.hh>

#define TH_POOL 1

//-----------------------------------------------------

SliceMult::SliceMult()
  : pool{4}
{
  return;
}

//-----------------------------------------------------

SliceMult::SliceMult(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p)
  : pool{4}
{
  //
  multiply(A,B,C,n,m,p);
  //
  return;
}

//-----------------------------------------------------

SliceMult::~SliceMult()
{
  return;
}

//-----------------------------------------------------

void
SliceMult::multiply(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p)
{
  //
  if(!checkValidity(A,B,C))
  {
    std::cerr << "Invalid matrix multiplication!!!" << std::endl; 
    return;
  }
  //
  int n_block_size = A.rows() / n;
  int p_block_size = A.cols() / p;
  int m_block_size = B.cols() / m;
  int n_block_remainder = A.rows() % n;
  int p_block_remainder = A.cols() % p;
  int m_block_remainder = B.cols() % m;
  std::vector<std::future<void>> futures;
  //
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < m; j++)
    {
      #if TH_POOL
      futures.push_back(
        pool.submit_task(
          [this, &A, &B, &C, 
            n_block_size, m_block_size, p_block_size, 
            i, j, 
            n_block_remainder, m_block_remainder, p_block_remainder]
          ()
          {
            this->ComputeCBlock(
              A, B, C,
              n_block_size, m_block_size, p_block_size,
              i, j,
              n_block_remainder, m_block_remainder, p_block_remainder);
          }
          )
        );
      #else
      // Adjust block sizes for the last block if there is a remainder
      ComputeCBlock(
        A, B, C,
        n_block_size, m_block_size, p_block_size,
        i, j,
        n_block_remainder, m_block_remainder, p_block_remainder);
      #endif
    }
  }
  //
  for(auto &f : futures) f.get();
  //
  return;
}

//-----------------------------------------------------

void
SliceMult::ComputeCBlock(
  const Eigen::MatrixXd &A, 
  const Eigen::MatrixXd &B, 
  Eigen::MatrixXd &C,
  const int size_n, const int size_m, const int size_p, 
  const int i, const int j, 
  const int rem_n, const int rem_m, const int rem_p)
{
  // check if the current block is the one with the reminder for n and m dimentions
  int current_n_block_size = size_n;
  int current_m_block_size = size_m;
  int current_p_block_size = size_p;
  current_n_block_size = (i == A.rows()/size_n - 1 && rem_n != 0) ? size_n + rem_n : size_n;
  current_m_block_size = (j == B.cols()/size_m - 1 && rem_m != 0) ? size_m + rem_m : size_m;
      // create a temporaru block to store the result of the multiplication
  Eigen::Block<Eigen::MatrixXd> &&C_tmp = C.block(i*size_n,j*size_m,current_n_block_size,current_m_block_size);
  for(int k = 0; k < A.cols()/size_p; k++)
  {
    // check if the current block is the one with the reminder for p dimention
    current_p_block_size = (k == A.cols()/size_p - 1 && rem_p != 0) ? size_p + rem_p : size_p;
    // core multiplication
    C_tmp +=  A.block(i*size_n,k*size_p,current_n_block_size,current_p_block_size) *
              B.block(k*size_p,j*size_m,current_p_block_size,current_m_block_size);
  }
  return;
}

//-----------------------------------------------------

bool 
SliceMult::checkValidity(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
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

//-----------------------------------------------------
