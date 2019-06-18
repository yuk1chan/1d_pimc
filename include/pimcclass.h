#ifndef PIMCCLASS_H_
#define PIMCCLASS_H_

#include "./iostruct.h"
#include "./particle.h"
#include "./pathclass.h"
#include <cmath>
#include <random>

#define P_SIZE 10000

class PIMCClass {

private:
  // random number generators
  std::mt19937 randgen_;
  std::uniform_real_distribution<> randu;
  std::uniform_real_distribution<> rand_delta;
  double *P;

  int accept;

public:
  PIMCClass(PIMCParams param);
  ~PIMCClass();

  PIMCParams param_; // parameters
  Path *path_;       // path
  // double PIMCdelta;   // pimc delta
  Particle update_path(); // update path with Metropolis
  void countP(Particle const &p);
  double calcE(int const &imcs);
  void outputE(double const &E);
  void outputP(std::ofstream &outfile, int const &mcs);
  // void print_info();  // print

  int getAccept(void);
  void setAccept(int);
};

#endif
