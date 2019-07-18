#include "../include/iostruct.h"

PIMCParams readPIMCParams(std::ifstream &infile) {
  PIMCParams param;
  std::string in;

  infile >> in >> param.Np;
  infile >> in >> param.Delta_t;
  // infile >> in >> param.beta;
  infile >> in >> param.delta;
  infile >> in >> param.Delta_p;
  infile >> in >> param.relaxation_steps;
  infile >> in >> param.MC_steps;
  infile >> in >> param.a;
  infile >> in >> param.b;
  infile >> in >> param.N;
  infile >> in >> param.nshow;
  infile >> in >> param.logfilename;

  // param.Delta_t = param.beta / param.Np;
  param.beta = param.Delta_t * param.Np;

  return param;
}
