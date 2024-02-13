#include <iostream>
#include <string>
#include <chrono>
#include <thread>

// eigen do not parallelize
#define EIGEN_DONT_PARALLELIZE

#include <Eigen/Core>

#include <ThreadPool.hh>


static nbsdx::concurrent::ThreadPool<10> pool;

class mytimer
{
public:
  std::chrono::time_point<std::chrono::high_resolution_clock> start;
  mytimer()
  {
    start = std::chrono::high_resolution_clock::now();
  }
  ~mytimer()
  {
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    //std::cout << "Elapsed time: " << elapsed.count() << " s\n";
  }
  // function to return a double of the elapsed time in ms
  double elapsed()
  {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
  }

  // std::chrono::duration<double> elapsed()
  // {
  //   auto end = std::chrono::high_resolution_clock::now();
  //   return std::chrono::milliseconds( end - start);
  // }
};

void
BlockMultiplication(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
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

  C11 = A11 * B11 + A12 * B21;
  C12 = A11 * B12 + A12 * B22;
  C21 = A21 * B11 + A22 * B21;
  C22 = A21 * B12 + A22 * B22;

  return;
}

void
BlockMultiplicationThreads(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C)
{
  int n = A.rows();
  int m = A.cols();
  int p = B.cols();

  if (A.cols() != B.rows())
  {
    std::cerr << "Invalid matrix multiplication:" 
              << "trying to multiply " << A.rows() << "x" << A.cols() << " with " 
                                       << B.rows() << "x" << B.cols() << std::endl;
    return;
  }
  if(C.rows() != A.rows() || C.cols() != B.cols())
  {
    std::cerr << "Invalid matrix multiplication:" 
              << "trying to store the result in a " << C.rows() << "x" << C.cols() << " matrix" << std::endl 
              << "Expected to be " << A.rows() << "x" << B.cols() << std::endl;

    return;
  }

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


  #if 0
  std::thread t1([&](){C11 = A11 * B11 + A12 * B21;});
  std::thread t2([&](){C12 = A11 * B12 + A12 * B22;});
  std::thread t3([&](){C21 = A21 * B11 + A22 * B21;});
  std::thread t4([&](){C22 = A21 * B12 + A22 * B22;});

  t1.join();
  t2.join();
  t3.join();
  t4.join();

  #else

  pool.AddJob([&](){C11 = A11 * B11 + A12 * B21;});
  pool.AddJob([&](){C12 = A11 * B12 + A12 * B22;});
  pool.AddJob([&](){C21 = A21 * B11 + A22 * B21;});
  pool.AddJob([&](){C22 = A21 * B12 + A22 * B22;});

  pool.WaitAll();

  #endif

  return;
}


int main()
{
  Eigen::initParallel();
  std::cout << "Eigen Test" << std::endl;
  double mean   = 0.0;
  double stdDev = 0.0;

  Eigen::MatrixXd M1, M2, M3a, M3b, M3c;

  int m_size = 500;

  M1.resize(m_size,m_size);
  M2.resize(m_size,m_size);
  M3a.resize(m_size,m_size);
  M3b.resize(m_size,m_size);
  M3c.resize(m_size,m_size);

  M1.Random(m_size,m_size);
  M2.Random(m_size,m_size);

  // loop of matrices to test the time with average time and standard deviation with eigen
  int n_runs = 100;
  Eigen::VectorXd times(n_runs);
  Eigen::VectorXd stdDev_vec(n_runs);
  std::cout << "Standard Product " << std::endl;
  for (int i = 0; i < n_runs; i++)
  {
    M1.Random(m_size,m_size);
    M2.Random(m_size,m_size);
    {
      mytimer t;
      M3a = M1 * M2;
      times(i) = t.elapsed();
    }
  }
  mean = times.mean();
  // standard deviation implementation
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;

  std::cout << "Block Product " << std::endl;
  for (int i = 0; i < n_runs; i++)
  {
    M1.Random(m_size,m_size);
    M2.Random(m_size,m_size);
    {
      mytimer t;
      BlockMultiplication(M1, M2, M3b);
      times(i) = t.elapsed();
    }
  }
  mean = times.mean();
  // standard deviation implementation
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;

  std::cout << "Block Product with Threads " << std::endl;
  for (int i = 0; i < n_runs; i++)
  {
    M1.Random(m_size,m_size);
    M2.Random(m_size,m_size);
    {
      mytimer t;
      BlockMultiplicationThreads(M1, M2, M3c);
      times(i) = t.elapsed();
    }
  }
  mean = times.mean();
  // standard deviation implementation
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;


  // check if the results are the same
  std::cout << "Check if the results are the same" << std::endl;
  std::cout << "M3a - M3b: " << (M3a - M3b).norm() << std::endl;
  std::cout << "M3a - M3c: " << (M3a - M3c).norm() << std::endl;

  return 0;
}
