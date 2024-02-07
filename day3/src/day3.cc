#include "day3.hh"

static std::mutex mtx;
static std::mutex mtx_deque;
static std::mutex mtx_data;


// static std::deque<int> deque_data;
// static int last_read_data = 0;
static std::deque<std::pair<int,int>> deque_data;
static std::pair<int,int> last_read_data;

//-----------------------------------------------------------------------------

void write_loop(int imax, int icolor = 0)
{
  char excape{0x1B};
  for(int i=0; i < imax; i++)
  {
    // mtx.lock();
    {
      std::lock_guard<std::mutex> lock(mtx);
      std::cout 
        << excape << "[" << 31+icolor << "m" 
        << "write loop (thread " << icolor << ") i = " << i 
        << excape << "[" << 0 << "m" 
        << std::endl;
      // mtx.unlock();
    }
    {
      std::lock_guard<std::mutex> lock(mtx_deque);
      deque_data.push_back( std::pair<int,int>(i,icolor) );
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

//-----------------------------------------------------------------------------

void read_loop(int imax )
{
  for(int i=0; i < imax; i++)
  {
    {
      std::lock_guard<std::mutex> lock(mtx);
      std::cout << "read loop " << i << " i = " << last_read_data.first << "," << last_read_data.second << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
}

//-----------------------------------------------------------------------------

void fast_read_loop( )
{
  while(true)
  {
    // read the stack and push in last_read_data
    {
      std::lock_guard<std::mutex> lock1(mtx_data);
      std::lock_guard<std::mutex> lock2(mtx_deque);
      std::lock_guard<std::mutex> lock3(mtx);

      if (!deque_data.empty())
      {
        last_read_data = deque_data.front();
        deque_data.pop_front();
      }
      std::cout << "fast read loop i => value " << last_read_data.first << " thread num " << last_read_data.second << " size of deque " << deque_data.size() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
}

//-----------------------------------------------------------------------------

int main()
{
  std::cout << "Day 3" << std::endl;
  // write_loop(10);

  // // ex2

  // std::thread th_write(write_loop, 10, 0);
  // th_write.join(); // wait for thread termination
  // std::cout<< "That's all folks!" << std::endl;

  // // ex3
  // std::vector<std::thread *> thread_loops;

  // // thread_loops.resize(10);
  // for(int i = 0; i<10; i++)
  // {
  //   thread_loops.emplace_back( new std::thread(write_loop, 10 + 1, i) );
  // }

  // for (auto th : thread_loops) th->join();

  // // ex4
  // std::vector<std::unique_ptr< std::thread> > thread_loops;
  // // thread_loops.resize(10);
  // for(int i = 0; i<10; i++)
  // {
  //   thread_loops.emplace_back(
  //     std::unique_ptr<std::thread>( new std::thread(write_loop, 10 + 1, i) ) 
  //   );
  // }
  // for (auto & th : thread_loops) th->join();
  // std::cout<< "That's all folks!" << std::endl;

  // // ex5
  // std::vector<std::unique_ptr< std::thread> > thread_loops;
  // // thread_loops.resize(10);
  // for(int i = 0; i<10; i++)
  // {
  //   thread_loops.emplace_back(new std::thread(write_loop, 10 + 1, i) );
  // }
  // for (auto & th : thread_loops) th->join();
  // std::cout<< "That's all folks!" << std::endl;

  // ex6
  std::vector<std::unique_ptr< std::thread> > thread_write_loops;
  std::thread thread_read_loop(read_loop,1000);
  std::thread thread_fast_read_loop(fast_read_loop);
  for(int i = 0; i<10; i++)
  {
    thread_write_loops.emplace_back(new std::thread(write_loop, 1000, i) );
  }
  for (auto & th : thread_write_loops) th->join();
  std::cout<< "That's all folks!" << std::endl;

  // exit(0);

  return 0;
}
