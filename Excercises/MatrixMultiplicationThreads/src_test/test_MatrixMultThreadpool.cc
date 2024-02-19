// ██████╗ ███████╗ ██████╗ ██╗   ██╗███████╗███████╗████████╗
// ██╔══██╗██╔════╝██╔═══██╗██║   ██║██╔════╝██╔════╝╚══██╔══╝
// ██████╔╝█████╗  ██║   ██║██║   ██║█████╗  ███████╗   ██║   
// ██╔══██╗██╔══╝  ██║▄▄ ██║██║   ██║██╔══╝  ╚════██║   ██║   
// ██║  ██║███████╗╚██████╔╝╚██████╔╝███████╗███████║   ██║   
// ╚═╝  ╚═╝╚══════╝ ╚══▀▀═╝  ╚═════╝ ╚══════╝╚══════╝   ╚═╝   
//
// Write a C++11 or later program that utilizes the Eigen3 library to perform matrix-matrix multiplication using block partitioning.
// Given matrices A and B, compute the matrix C = A * B. Matrices A and B must be compatible for multiplication. Given the integers N, P, M, partition the matrices as follows:
//  - Matrix A into N x P blocks
//  - Matrix B into P x M blocks
//  - Matrix C into N x M blocks
// Ensure that the partitioning is compatible. If matrices A and B are incompatible, or if the required partitioning (N, P, M) is not possible, throw an exception.
// Each (i, j) block of matrix C must be computed on a separate thread if available, enabling parallel code execution.
// Finally, compare the execution speed of your block partitioning matrix multiplication with the timing of the standard Eigen3 matrix multiplication command.
// Use the proposed ThreadPool to find a better one to perform parallel tasks.

#include <iostream>

#include <timer.hh>
#include <SliceMult.hh>

// eigen do not parallelize
#define EIGEN_DONT_PARALLELIZE

#include <Eigen/Core>

int main()
{
  // ████████╗███████╗███████╗████████╗
  // ╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝
  //    ██║   █████╗  ███████╗   ██║   
  //    ██║   ██╔══╝  ╚════██║   ██║   
  //    ██║   ███████╗███████║   ██║   
  //    ╚═╝   ╚══════╝╚══════╝   ╚═╝   

  srand(0);     
  Eigen::initParallel();
  std::cout << "Eigen Test" << std::endl;
  double mean   = 0.0;
  double stdDev = 0.0;
  Eigen::MatrixXd M1, M2, M3a, M3b;
  int n_runs = 100;
  Eigen::VectorXd times(n_runs);
  Eigen::VectorXd stdDev_vec(n_runs);
  int n_size = 47;
  int p_size = 121;
  int m_size = 187;
  M1.resize(n_size,p_size);
  M2.resize(p_size,m_size);
  M3a.resize(n_size,m_size);
  M3b.resize(n_size,m_size);
  

  M1.setRandom(n_size,p_size);
  M2.setRandom(p_size,m_size);

  // ███████╗████████╗ █████╗ ███╗   ██╗██████╗  █████╗ ██████╗ ██████╗ 
  // ██╔════╝╚══██╔══╝██╔══██╗████╗  ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗
  // ███████╗   ██║   ███████║██╔██╗ ██║██║  ██║███████║██████╔╝██║  ██║
  // ╚════██║   ██║   ██╔══██║██║╚██╗██║██║  ██║██╔══██║██╔══██╗██║  ██║
  // ███████║   ██║   ██║  ██║██║ ╚████║██████╔╝██║  ██║██║  ██║██████╔╝
  // ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝ 
                                                                   
  std::cout << "Standard Product " << std::endl;
  for (int i = 0; i < n_runs; i++)
  {
    {
      mytimer t;
      M3a = M1 * M2;
      times(i) = t.elapsed();
    }
  }
  mean   = times.mean();
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;

  SliceMult SM;
  // test
  for (int i = 0; i < n_runs; i++)
  {
    M3b.setZero(n_size,m_size);
    {
      mytimer t;
      SM.multiply(M1, M2, M3b, 2, 2, 2);
      times(i) = t.elapsed();
    }
  }
  mean   = times.mean();
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;


  std::cout << "Check if the results are the same" << std::endl;
  std::cout << "M3a - M3b: " << (M3a- M3b).norm() << std::endl;

  return 0;
}
