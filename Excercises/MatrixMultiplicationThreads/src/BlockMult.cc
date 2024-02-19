#include <BlockMult.hh>

#define TH_POOL 1

//-----------------------------------------------------

BlockMult::BlockMult()
  : pool{4}
{
  return;
}

//-----------------------------------------------------

BlockMult::BlockMult(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p)
  : pool{4}
{
  //
  multiply(A,B,C,n,m,p);
  //
  return;
}

//-----------------------------------------------------

BlockMult::~BlockMult()
{
  return;
}

//-----------------------------------------------------

void
BlockMult::multiply(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p)
{
  //
  if(!checkValidity(A,B,C))
  {
    std::cerr << "Invalid matrix multiplication!!!" << std::endl; 
    return;
  }
  std::vector<std::future<void>> futures;
  //
  for(int i = 0; i < A.rows() / n; i++)
  {
    for(int j = 0; j < B.cols() / m; j++)
    {
      #if TH_POOL
      futures.push_back(
        pool.submit_task(
          [this, &A, &B, &C, n, m, p, i, j](){this->ComputeCBlock( A, B, C.block(i*n,j*m,n,m), n, m, p, i, j);})
        );
      #else
      ComputeCBlock( A, B, C.block(i*n,j*m,n,m), n, m, p, i, j);
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
BlockMult::ComputeCBlock(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::Block<Eigen::MatrixXd> &&C, const int n, const int m, const int p, int i, int j)
{
  for(int k = 0; k < A.cols()/p; k++)
  {
    C +=  A.block( i * n, k * p, n, p ) * 
          B.block( k * p, j * m, p, m );
  }
  return;
}

//-----------------------------------------------------

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

//-----------------------------------------------------
