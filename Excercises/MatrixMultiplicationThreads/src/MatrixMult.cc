#include <MatrixMult.hh>

static BS::thread_pool pool(10);

bool
checkValidity(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
  if (A.cols() != B.rows())
  {
    std::cerr << "Invalid matrix multiplication:" 
              << "trying to multiply " << A.rows() << "x" << A.cols() << " with " 
                                       << B.rows() << "x" << B.cols() << std::endl;
    return false;
  }
  if(C.rows() != A.rows() || C.cols() != B.cols())
  {
    std::cerr << "Invalid matrix multiplication:" 
              << "trying to store the result in a " << C.rows() << "x" << C.cols() << " matrix" << std::endl 
              << "Expected to be " << A.rows() << "x" << B.cols() << std::endl;

    return false;
  }
  return true;
}

bool
checkPartition(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
  bool out_flag = false;
  int n = A.rows();
  int m = A.cols();
  int p = B.cols();

  int n1 = n / 2;
  int m1 = m / 2;
  int p1 = p / 2;


  // !(checkValidity(A11,B11,C11) &&
  //   checkValidity(A12,B21,C11) &&
  //   checkValidity(A11,B12,C12) &&
  //   checkValidity(A12,B22,C12) &&
  //   checkValidity(A21,B11,C21) &&
  //   checkValidity(A22,B21,C21) &&
  //   checkValidity(A21,B12,C22) &&
  //   checkValidity(A22,B22,C22))

  return out_flag;

}

void
BlockMultiplication(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
  if(!checkValidity(A, B, C))
  {
    std::cerr << "Invalid matrix multiplication!!!" << std::endl; 
    return;
  }

  int n0 = A.rows();
  int m0 = A.cols();
  int p0 = B.cols();

  int n1 = n0 / 2;
  int m1 = m0 / 2;
  int p1 = p0 / 2;

  Eigen::MatrixXd A11 = A.block(0, 0, n1, m1);
  Eigen::MatrixXd A12 = A.block(0, m1, n1, m0 - m1);
  Eigen::MatrixXd A21 = A.block(n1, 0, n0 - n1, m1);
  Eigen::MatrixXd A22 = A.block(n1, m1, n0 - n1, m0 - m1);

  Eigen::MatrixXd B11 = B.block(0, 0, m1, p1);
  Eigen::MatrixXd B12 = B.block(0, p1, m1, p0 - p1);
  Eigen::MatrixXd B21 = B.block(m1, 0, m0 - m1, p1);
  Eigen::MatrixXd B22 = B.block(m1, p1, m0 - m1, p0 - p1);
  
  Eigen::MatrixXd C11 = C.block(0, 0, n1, p1);
  Eigen::MatrixXd C12 = C.block(0, p1, n1, p0 - p1);
  Eigen::MatrixXd C21 = C.block(n1, 0, n0 - n1, p1);
  Eigen::MatrixXd C22 = C.block(n1, p1, n0 - n1, p0 - p1);

  C11 = A11 * B11 + A12 * B21;
  C12 = A11 * B12 + A12 * B22;
  C21 = A21 * B11 + A22 * B21;
  C22 = A21 * B12 + A22 * B22;

  return;
}

void
BlockMultiplicationThreads(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
  if(!checkValidity(A, B, C))
  {
    std::cerr << "Invalid matrix multiplication!!!" << std::endl; 
    return;
  }

  int n = A.rows();
  int m = A.cols();
  int p = B.cols();

  int n1 = n / 2;
  int m1 = m / 2;
  int p1 = p / 2;

  Eigen::MatrixXd A11 = A.block(0, 0, n1, m1);
  Eigen::MatrixXd A12 = A.block(0, m1, n1, m - m1);
  Eigen::MatrixXd A21 = A.block(n1, 0, n - n1, m1);
  Eigen::MatrixXd A22 = A.block(n1, m1, n - n1, m - m1);

  Eigen::MatrixXd B11 = B.block(0, 0, m1, p1);
  Eigen::MatrixXd B12 = B.block(0, p1, m1, p - p1);
  Eigen::MatrixXd B21 = B.block(m1, 0, m - m1, p1);
  Eigen::MatrixXd B22 = B.block(m1, p1, m - m1, p - p1);
  
  Eigen::MatrixXd C11 = C.block(0, 0, n1, p1);
  Eigen::MatrixXd C12 = C.block(0, p1, n1, p - p1);
  Eigen::MatrixXd C21 = C.block(n1, 0, n - n1, p1);
  Eigen::MatrixXd C22 = C.block(n1, p1, n - n1, p - p1);

  std::thread t1([&](){C11 = A11 * B11 + A12 * B21;});
  std::thread t2([&](){C12 = A11 * B12 + A12 * B22;});
  std::thread t3([&](){C21 = A21 * B11 + A22 * B21;});
  std::thread t4([&](){C22 = A21 * B12 + A22 * B22;});

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  return;
}

void
BlockMultiplicationThreadPool(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, int n_threads)
{
  if(!checkValidity(A, B, C))
  {
    std::cerr << "Invalid matrix multiplication!!!" << std::endl; 
    return;
  }

  int n = A.rows();
  int m = A.cols();
  int p = B.cols();

  int n1 = n / 2;
  int m1 = m / 2;
  int p1 = p / 2;

  Eigen::MatrixXd A11 = A.block(0, 0, n1, m1);
  Eigen::MatrixXd A12 = A.block(0, m1, n1, m - m1);
  Eigen::MatrixXd A21 = A.block(n1, 0, n - n1, m1);
  Eigen::MatrixXd A22 = A.block(n1, m1, n - n1, m - m1);

  Eigen::MatrixXd B11 = B.block(0, 0, m1, p1);
  Eigen::MatrixXd B12 = B.block(0, p1, m1, p - p1);
  Eigen::MatrixXd B21 = B.block(m1, 0, m - m1, p1);
  Eigen::MatrixXd B22 = B.block(m1, p1, m - m1, p - p1);
  
  Eigen::MatrixXd C11 = C.block(0, 0, n1, p1);
  Eigen::MatrixXd C12 = C.block(0, p1, n1, p - p1);
  Eigen::MatrixXd C21 = C.block(n1, 0, n - n1, p1);
  Eigen::MatrixXd C22 = C.block(n1, p1, n - n1, p - p1);

  std::future<void> my_future;

  my_future = pool.submit_task([&](){C11 = A11 * B11 + A12 * B21;});
  my_future = pool.submit_task([&](){C12 = A11 * B12 + A12 * B22;});
  my_future = pool.submit_task([&](){C21 = A21 * B11 + A22 * B21;});
  my_future = pool.submit_task([&](){C22 = A21 * B12 + A22 * B22;});

  pool.wait();

  return;
}

