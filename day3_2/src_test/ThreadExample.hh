#ifndef DAY3_HH
#define DAY3_HH

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <deque>

// #include <fstream>
// #include <vector>


#define ASSERT_DAY3(cond,msg) \
  if (!(cond)) { \
    std::cout << "Sudoku is invalid: " << msg << "\n"; \
    return 1; \
  } 

#endif                                                      