#ifndef IOSTRUCT_H_
#define IOSTRUCT_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

struct PIMCParams {

  int Np;         // number of particles
  double Delta_t; // imaginary time step

  int relaxation_steps; //
  int MC_steps;

  double delta;   // particle walks random [-delta,delta]
  double Delta_p; //

  int N; //

  double beta; //

  double a, b; // [a,b]

  int nshow;

  std::string logfilename;
};

PIMCParams readPIMCParams(std::ifstream &infile);

#endif
