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
#include <MatrixMult.hh>

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
                                  
  Eigen::initParallel();
  std::cout << "Eigen Test" << std::endl;
  double mean   = 0.0;
  double stdDev = 0.0;
  Eigen::MatrixXd M1, M2, M3a, M3b, M3c, M3d;
  int n_runs = 100;
  Eigen::VectorXd times(n_runs);
  Eigen::VectorXd stdDev_vec(n_runs);
  int m_size = 500;
  M1.resize(m_size,m_size);
  M2.resize(m_size,m_size);
  M3a.resize(m_size,m_size);
  M3b.resize(m_size,m_size);
  M3c.resize(m_size,m_size);
  M3d.resize(m_size,m_size);

  M1.Random(m_size,m_size);
  M2.Random(m_size,m_size);

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

  // ██████╗ ██╗      ██████╗  ██████╗██╗  ██╗
  // ██╔══██╗██║     ██╔═══██╗██╔════╝██║ ██╔╝
  // ██████╔╝██║     ██║   ██║██║     █████╔╝ 
  // ██╔══██╗██║     ██║   ██║██║     ██╔═██╗ 
  // ██████╔╝███████╗╚██████╔╝╚██████╗██║  ██╗
  // ╚═════╝ ╚══════╝ ╚═════╝  ╚═════╝╚═╝  ╚═╝
                                          
  std::cout << "Block Product " << std::endl;
  for (int i = 0; i < n_runs; i++)
  {
    {
      mytimer t;
      BlockMultiplication(M1, M2, M3b);
      times(i) = t.elapsed();
    }
  }
  mean   = times.mean();
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;

  // ████████╗██╗  ██╗██████╗ ███████╗ █████╗ ██████╗ ███████╗
  // ╚══██╔══╝██║  ██║██╔══██╗██╔════╝██╔══██╗██╔══██╗██╔════╝
  //    ██║   ███████║██████╔╝█████╗  ███████║██║  ██║███████╗
  //    ██║   ██╔══██║██╔══██╗██╔══╝  ██╔══██║██║  ██║╚════██║
  //    ██║   ██║  ██║██║  ██║███████╗██║  ██║██████╔╝███████║
  //    ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ ╚══════╝
                                                          
  std::cout << "Block Product with Threads " << std::endl;
  for (int i = 0; i < n_runs; i++)
  {
    {
      mytimer t;
      BlockMultiplicationThreads(M1, M2, M3c);
      times(i) = t.elapsed();
    }
  }
  mean   = times.mean();
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;

  // ██████╗  ██████╗  ██████╗ ██╗     
  // ██╔══██╗██╔═══██╗██╔═══██╗██║     
  // ██████╔╝██║   ██║██║   ██║██║     
  // ██╔═══╝ ██║   ██║██║   ██║██║     
  // ██║     ╚██████╔╝╚██████╔╝███████╗
  // ╚═╝      ╚═════╝  ╚═════╝ ╚══════╝

  std::cout << "Block Product with Thread Pool " << std::endl;
  for (int i = 0; i < n_runs; i++)
  {
    {
      mytimer t;
      BlockMultiplicationThreadPool(M1, M2, M3d, 4);
      times(i) = t.elapsed();
    }
  }
  mean   = times.mean();
  stdDev = (((times.array() - mean) * (times.array() - mean)).sqrt()).sum()/((double)(n_runs-1));
  std::cout << "Average time: " << mean << " ms" << std::endl;
  std::cout << "Standard deviation: " << stdDev << " ms" << std::endl;
                                    

  //  ██████╗██╗  ██╗███████╗ ██████╗██╗  ██╗
  // ██╔════╝██║  ██║██╔════╝██╔════╝██║ ██╔╝
  // ██║     ███████║█████╗  ██║     █████╔╝ 
  // ██║     ██╔══██║██╔══╝  ██║     ██╔═██╗ 
  // ╚██████╗██║  ██║███████╗╚██████╗██║  ██╗
  //  ╚═════╝╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝
                                        
  // check if the results are the same
  std::cout << "Check if the results are the same" << std::endl;
  std::cout << "M3a - M3b: " << (M3a - M3b).norm() << std::endl;
  std::cout << "M3a - M3c: " << (M3a - M3c).norm() << std::endl;

  return 0;
}