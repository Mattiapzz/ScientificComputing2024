#ifndef TIMER_HH
#define TIMER_HH

#include <chrono>
#include <thread>

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
  }
  // function to return a double of the elapsed time in ms
  double elapsed()
  {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
  }
};

#endif