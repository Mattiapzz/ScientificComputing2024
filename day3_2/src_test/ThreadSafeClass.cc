#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include <deque>

#include <map>


#define ASSERT(cond,msg) \
  if (!(cond)) { \
    std::cout << "Sudoku is invalid: " << msg << "\n"; \
    return -1; \
  } 


// class no_copy_string : public std::string
// {
//   public:
//     no_copy_string(const no_copy_string&) = delete;

//     no_copy_string(const std::string & s) : std::string(s) {}

//     // no_copy_string() = default;
//     // no_copy_string& operator=(const no_copy_string &) = delete;
// };

class gradient
{
  private:
    mutable double v = 0;
    mutable double args[3] = {0,0,0};
    mutable double g[3] = {0,0,0};

  public:
    
    void eval(const double x, const double y, const double z)  
    { 
      v = x*x -x*log(y) *exp(z-x); 
    }
    void eval_grad(const double x, const double y, const double z) const
    {
      std::cout << "enter eval_grad" << std::endl;
      if( args[0] == x && args[1] == y && args[2] == z) return;
      //
      std::cout << "compute the gradient" << std::endl;
      args[0] = x;
      args[1] = y;
      args[2] = z;
      g[0] = 2*x - log(y)*exp(z-x);
      g[1] = x/(y) * exp(z-x);
      g[2] = -x*exp(z-x);
    }

    double operator()(const double x, const double y, const double z)
    {
      eval(x,y,z);
      return v;
    }

    double Dx(const double x, const double y, const double z)
    {
      eval_grad(x,y,z);
      return g[0];
    }

    double Dy(const double x, const double y, const double z)
    {
      eval_grad(x,y,z);
      return g[1];
    }

    double Dz(const double x, const double y, const double z)
    {
      eval_grad(x,y,z);
      return g[2];
    }
};




int main()
{
  std::cout << "Thread Safe Class Example" << std::endl;
  // std::map<std::string,double> m;

  // m["pippo"] = 1.23;

  // m.insert( std::pair<std::string,double>("pluto", 2.34) );

  // for( auto const & el : m )
  // {
  //   std::cout << "M[ " << el.first << ", " << el.second << " ]" << std::endl;
  // }

  gradient F;

  std::cout << "F.(0,1,2) = " << F(0.1,1,2) << std::endl;
  std::cout << "F.Dx(0,1,2) = " << F.Dx(0.1,1,2) << std::endl;
  std::cout << "F.Dy(0,1,2) = " << F.Dy(0.1,1,2) << std::endl;
  std::cout << "F.Dz(0,1,2) = " << F.Dz(0.1,1,2) << std::endl;


  return 0;
}
