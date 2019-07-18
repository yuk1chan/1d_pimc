#ifndef PIMCCLASS_H_
#define PIMCCLASS_H_

#include "./iostruct.h"
#include "./particle.h"
#include "./pathclass.h"
#include <cmath>
#include <random>
#include <vector>

#define P_SIZE 50000

class PIMCClass {

private:
  // random number generators
  std::mt19937 randgen_;
  std::uniform_real_distribution<> randu;      // [0,1]
  std::uniform_real_distribution<> rand_delta; // [-delta,delta]
  std::uniform_real_distribution<> rand_ab;    // [a,b]
  std::vector<double> P;
  std::vector<Particle> init_path;

  int accept;
  double Esum;
  double E2sum;

public:
  PIMCClass(PIMCParams param);
  ~PIMCClass();

  PIMCParams param_; // parameters
  Path *path_;       // path
  // double PIMCdelta;   // pimc delta
  Particle update_local_path();            // update path with Metropolis
  Particle update_local_path(int const &); // update path with Metropolis
  void update_global_path();
  void countP(Particle const &p);
  double calcE(int const &imcs);
  void outputE(double const &E);
  void outputP(std::ofstream &outfile, int const &mcs);
  // void print_info();  // print

  int getAccept(void);
  void setAccept(int);

  void outputPath(std::ofstream &outfile);

  // double get_local_Action(int);
  double get_global_Action();

  void initE();
  double getE(int const &mcs);
  double getV_E(int const &mcs);
};

#endif
