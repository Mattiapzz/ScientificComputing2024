#ifndef MATRIXMULT_HH
#define MATRIXMULT_HH

#include <iostream>

#include <string>
#include <chrono>
#include <thread>

#include <timer.hh>

#include <Eigen/Core>

#include <BS_thread_pool.hpp>


bool
checkValidity(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);

bool
checkPartition(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);

void
BlockMultiplication(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);

void
BlockMultiplicationThreads(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C);

void
BlockMultiplicationThreadPool(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, int n_threads);

void
BlockMultiplication(const Eigen::MatrixXd &A, const Eigen::MatrixXd &B, Eigen::MatrixXd &C, const int n, const int m, const int p);



#endif