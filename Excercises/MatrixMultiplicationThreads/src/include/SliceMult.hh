#ifndef SLICEMULT_HH
#define SLICEMULT_HH

#include <iostream>

#include <string>
#include <chrono>
#include <thread>

#include <timer.hh>

#include <Eigen/Core>

#include <BS_thread_pool.hpp>

class SliceMult
{
private:
  BS::thread_pool pool;
  // mutex
  std::mutex mtx;
public:
  SliceMult();
  SliceMult(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p);
  void multiply(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p);
  ~SliceMult();
private:
  bool checkValidity(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);
  void ComputeCBlock(  const Eigen::MatrixXd &A, 
  const Eigen::MatrixXd &B, 
  Eigen::MatrixXd &C,
  const int size_n, const int size_m, const int size_p, 
  const int i, const int j, 
  const int rem_n=0, const int rem_m=0, const int rem_p=0);
};


#endif