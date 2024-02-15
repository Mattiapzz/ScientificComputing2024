#ifndef BLOCKMULT_HH
#define BLOCKMULT_HH

#include <iostream>

#include <string>
#include <chrono>
#include <thread>

#include <timer.hh>

#include <Eigen/Core>

#include <BS_thread_pool.hpp>

class BlockMult
{
private:
  BS::thread_pool pool;
  // mutex
  std::mutex mtx;
public:
  BlockMult();
  BlockMult(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p);
  void multiply(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p);
  ~BlockMult();
private:
  bool checkValidity(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);
  void ComputeCBlock(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::Block<Eigen::MatrixXd> &&C, const int n, const int m, const int p, int i, int j);
};


#endif