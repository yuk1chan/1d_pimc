#include "../include/particle.h"
#include "../include/pathclass.h"

Path::Path() {}

Path::Path(int Np) {
  Np_ = Np;

  path_ = new Particle[Np];
}

Path::~Path() { delete[] path_; }

void Path::set_particle(int n, Particle p) { path_[n] = p; }

Particle Path::get_particle(int n) { return path_[n]; }

Particle Path::get_left_neightbour(int n) {
  if (n == 0) {
    return path_[Np_ - 1];
  }

  return path_[n - 1];
}

Particle Path::get_right_neightbour(int n) {
  if (n == Np_ - 1) {
    return path_[0];
  }

  return path_[n + 1];
}

double Path::get_left_distance(int n) {

  Particle p = get_particle(n);
  Particle pl = get_left_neightbour(n);

  return p.x - pl.x;
}

double Path::get_right_distance(int n) {
  Particle p = get_particle(n);
  Particle pr = get_right_neightbour(n);

  return p.x - pr.x;
}
