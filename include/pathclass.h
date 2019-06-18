#ifndef PATHCLASS_H_
#define PATHCLASS_H_

#include "./particle.h"

class Path {

private:
  int Np_; // num of particle
  Particle *path_;

public:
  Path();
  Path(int N); // define the path
  ~Path();

  void set_particle(int n, Particle p);
  Particle get_particle(int n);
  Particle get_left_neightbour(int n);  // n-1
  Particle get_right_neightbour(int n); // n+1
  double get_left_distance(int n);
  double get_right_distance(int n);
};

#endif
