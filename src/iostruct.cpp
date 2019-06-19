#include "../include/iostruct.h"

PIMCParams readPIMCParams(std::ifstream &infile) {
  PIMCParams param;
  std::string in;

  infile >> in >> param.Np;
  infile >> in >> param.Delta_t;
  infile >> in >> param.delta;
  infile >> in >> param.Delta_p;
  infile >> in >> param.relaxation_steps;
  infile >> in >> param.MC_steps;
  infile >> in >> param.N;

  param.beta = param.Np * param.Delta_t;

  return param;
}
