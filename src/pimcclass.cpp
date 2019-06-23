#include "../include/iostruct.h"
#include "../include/particle.h"
#include "../include/pathclass.h"
#include "../include/pimcclass.h"
#include "../include/potential.h"

#include <cmath>
#include <iostream>
#include <random>

PIMCClass::PIMCClass(PIMCParams param)
    : randu(0.0, 1.0), rand_delta(-param.delta, param.delta),
      rand_ab(param.a, param.b) {
  param_ = param;
  path_ = new Path(param_.Np); // define path

  P = new double[P_SIZE];

  for (int i = 0; i < P_SIZE; i++) {
    P[i] = 0.0;
  }

  std::random_device rd;

  randgen_.seed(rd());
  // path init
  for (int i = 0; i < param_.Np; i++) {
    Particle p;
    p.x = rand_ab(randgen_);
    // p.x = (randu(randgen_) * 2 - 1) + 1;
    // p.x = 1;
    path_->set_particle(i, p);
  }

  accept = 0;
}

PIMCClass::~PIMCClass() {
  delete path_;
  delete[] P;
}

Particle PIMCClass::update_path() {

  int j;
  Particle p_trial, p;

  // 1つの原子jを選択
  j = randgen_() % (param_.Np - 1);

  p_trial = p = path_->get_particle(j);

  p_trial.x += rand_delta(randgen_);

  double tmpE1, tmpE2;

  path_->set_particle(j, p_trial);
  tmpE1 = 0.5 * pow(path_->get_left_distance(j), 2) / pow(param_.Delta_t, 2);
  tmpE1 += 0.5 * pow(path_->get_right_distance(j), 2) / pow(param_.Delta_t, 2);
  tmpE1 += V(p_trial.x);

  path_->set_particle(j, p);
  tmpE2 = 0.5 * pow(path_->get_left_distance(j), 2) / pow(param_.Delta_t, 2);
  tmpE2 += 0.5 * pow(path_->get_right_distance(j), 2) / pow(param_.Delta_t, 2);
  tmpE2 += V(p.x);

  double Delta_E = tmpE1 - tmpE2;

  if (std::min(1.0, exp(-Delta_E * param_.Delta_t)) > randu(randgen_)) {

    path_->set_particle(j, p_trial);
    accept += 1;

  } // else 受け入れない

  return path_->get_particle(j);
}

void PIMCClass::countP(Particle const &p) {

  double position = p.x;
  int bin = (int)P_SIZE / 2 + floor(position / param_.Delta_p + 0.5);

  P[bin] += 1.0;
}

double PIMCClass::calcE(int const &imcs) {

  int bin;
  double E = 0.0;
  double position = -param_.Delta_p * P_SIZE / 2; // xmin

  while (position < param_.Delta_p * P_SIZE / 2) { // xmaxまで

    bin = (int)P_SIZE / 2 + floor(position / param_.Delta_p + 0.5);
    E += P[bin] * (T(position) + V(position)) / (imcs * param_.Np);

    position += param_.Delta_p;
  }

  return E;
}

void PIMCClass::outputE(double const &E) {

  static int count = 0;
  static double Esum = 0.0;
  static double Esum2 = 0.0;

  count++;
  Esum += E;
  Esum2 += E * E;

  std::cout << "エネルギー :" << E << std::endl;
  std::cout << "エネルギーの分散 :" << Esum2 / count - pow(Esum / count, 2)
            << std::endl;
  std::cout << std::endl;
}

void PIMCClass::outputP(std::ofstream &outfile, int const &mcs) {

  int pi, mi;
  for (int i = 0; i < P_SIZE / 2; i++) {

    if (i == 0) {
      outfile << i * param_.Delta_p << "\t"
              << P[i] / (param_.Np * mcs) / param_.Delta_p << std::endl;
    } else {

      pi = i + P_SIZE / 2;
      mi = -i + P_SIZE / 2;

      outfile << i * param_.Delta_p << "\t"
              << P[pi] / (param_.Np * mcs) / param_.Delta_p << std::endl;
      outfile << -i * param_.Delta_p << "\t"
              << P[mi] / (param_.Np * mcs) / param_.Delta_p << std::endl;
    }
  }
}

int PIMCClass::getAccept() { return accept; }
void PIMCClass::setAccept(int value) { accept = value; }

void PIMCClass::outputPath(std::ofstream &outfile) {

  for (int i = 0; i < param_.Np; i++) {
    outfile << i << ": " << path_->get_particle(i).x << std::endl;
  }
}

double PIMCClass::getE(int imcs) {

  int bin;
  double E = 0.0;
  double position = -param_.Delta_p * P_SIZE / 2; // xmin

  while (position < param_.Delta_p * P_SIZE / 2) { // xmaxまで

    bin = (int)P_SIZE / 2 + floor(position / param_.Delta_p + 0.5);
    E += P[bin] * (position * position / 2 + V(position)) / (imcs * param_.Np);

    position += param_.Delta_p;
  }

  return E;
}
