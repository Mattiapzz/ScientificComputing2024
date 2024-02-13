#ifndef BLOCKMULT_HH
#define BLOCKMULT_HH

#include <iostream>

#include <string>
#include <chrono>
#include <thread>

#include <timer.hh>

#include <Eigen/Core>

#include <BS_thread_pool.hpp>

struct block{
  bool alreadyComputed = false;
  Eigen::MatrixXd BM;
};

class BlockMult
{
private:
  std::vector<std::vector<block>> A_blocks;
  std::vector<std::vector<block>> B_blocks;
  std::vector<std::vector<block>> C_blocks;
  int m_n, m_m, m_p;
  BS::thread_pool pool;
  // mutex
  std::mutex mtxA, mtxB, mtxC;
public:
  BlockMult()
  : pool{10}
  {
    A_blocks.clear();
    B_blocks.clear();
    C_blocks.clear();
  };
  BlockMult(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p);
  void multiply(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p);
  ~BlockMult();
private:
  bool checkValidity(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);
  void BlockMultiplication(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);
  void ComputeCBlock(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, int i, int j);
};


#endif