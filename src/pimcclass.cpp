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

  P.resize(P_SIZE);

  for (int i = 0; i < P_SIZE; i++) {
    P[i] = 0.0;
  }

  std::random_device rd;

  // randgen_.seed(rd());
  randgen_.seed(10000);
  // path init
  for (int i = 0; i < param_.Np; i++) {
    Particle p;
    // p.x = rand_ab(randgen_);
    p.x = 0.0;
    init_path.push_back(p);
    path_->set_particle(i, p);
  }

  accept = 0;
  Esum = 0.0;
  E2sum = 0.0;
}

PIMCClass::~PIMCClass() { delete path_; }

Particle PIMCClass::update_local_path() {

  int j;
  Particle p_trial, p;

  // 1つの原子jを選択
  j = randgen_() % (param_.Np);

  p_trial = p = path_->get_particle(j);

  p_trial.x += rand_delta(randgen_);

  double tmpE1, tmpE2;
  double inv_Delta_t2 = 1 / pow(param_.Delta_t, 2);

  path_->set_particle(j, p_trial);
  tmpE1 = 0.5 * pow(path_->get_left_distance(j), 2) * inv_Delta_t2;
  tmpE1 += 0.5 * pow(path_->get_right_distance(j), 2) * inv_Delta_t2;
  tmpE1 += V(p_trial.x);

  path_->set_particle(j, p);
  tmpE2 = 0.5 * pow(path_->get_left_distance(j), 2) * inv_Delta_t2;
  tmpE2 += 0.5 * pow(path_->get_right_distance(j), 2) * inv_Delta_t2;
  tmpE2 += V(p.x);

  double Delta_E = tmpE1 - tmpE2;
  double w = exp(-Delta_E * param_.Delta_t);

  if (std::min(1.0, w) > randu(randgen_)) {

    path_->set_particle(j, p_trial);
    accept += 1;

  } // else 受け入れない

  p = path_->get_particle(j);

  double E = T(p.x) + V(p.x);
  Esum += E;
  E2sum += E * E;
  return path_->get_particle(j);
}

void PIMCClass::update_global_path() {

  double inv_Np = 1.0 / param_.Np;
  double inv_Delta_t2 = 1 / pow(param_.Delta_t, 2);
  int k = randgen_() % ((int)param_.Np / 4);
  // std::cout << k << std::endl;

  // 現在のpathのエネルギー
  double nowE = get_global_Action();
  std::vector<Particle> now_path;
  // 現在のpathを退避
  for (int i = 0; i < param_.Np; i++) {
    now_path.push_back(path_->get_particle(i));
  }

  // trial pathをset
  for (int i = 0; i < param_.Np; i++) {
    Particle trial = path_->get_particle(i);
    trial.x += 0.3 * cos(2 * M_PI * k * (i * inv_Np + rand_ab(randgen_)));
    path_->set_particle(i, trial);
  }

  // trial pathのエネルギーの計算
  double trialE = 0.0;
  for (int i = 0; i < param_.Np; i++) {
    trialE += 0.5 * pow(path_->get_right_distance(i), 2) * inv_Delta_t2;
    trialE += V(path_->get_particle(i).x);
  }

  double Delta_E = trialE - nowE;
  double w = exp(-Delta_E * param_.beta);
  if (std::min(1.0, w) > randu(randgen_)) { // accept
    std::cout << "tiral E\t" << trialE << std::endl;
    std::cout << "now   E\t" << nowE << std::endl;
  } else { // pathを戻す
    for (int i = 0; i < param_.Np; i++) {
      path_->set_particle(i, now_path[i]);
    }
  }
}

void PIMCClass::countP(Particle const &p) {

  double position = p.x;
  double inv_Delta_p = 1.0 / param_.Delta_p;
  int bin = (int)P_SIZE / 2 + floor(position * inv_Delta_p + 0.5);

  P[bin] += 1.0;
}

double PIMCClass::calcE(int const &imcs) {

  int bin;
  double E = 0.0;
  double position = -param_.Delta_p * P_SIZE / 2; // xmin
  double inv_Delta_p = 1.0 / param_.Delta_p;
  double inv_imcs_Np = 1.0 / (imcs * param_.Np);

  while (position < param_.Delta_p * P_SIZE / 2) { // xmaxまで

    bin = (int)P_SIZE / 2 + floor(position * inv_Delta_p + 0.5);
    E += P[bin] * (T(position) + V(position)) * inv_imcs_Np;

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
  double inv_Delta_p = 1.0 / param_.Delta_p;
  double inv_mcs_Np = 1.0 / (param_.Np * mcs);
  for (int i = 0; i < P_SIZE / 2; i++) {

    if (i == 0) {
      outfile << i * param_.Delta_p << "\t" << P[i] * inv_mcs_Np * inv_Delta_p
              << std::endl;
    } else {

      pi = i + P_SIZE / 2;
      mi = -i + P_SIZE / 2;

      outfile << i * param_.Delta_p << "\t" << P[pi] * inv_mcs_Np * inv_Delta_p
              << std::endl;
      outfile << -i * param_.Delta_p << "\t" << P[mi] * inv_mcs_Np * inv_Delta_p
              << std::endl;
    }
  }
}

int PIMCClass::getAccept() { return accept; }
void PIMCClass::setAccept(int value) { accept = value; }

void PIMCClass::outputPath(std::ofstream &outfile) {

  for (int i = 0; i < param_.Np; i++) {
    outfile << path_->get_particle(i).x << "\t" << i * param_.Delta_t
            << std::endl;
  }
}

double PIMCClass::get_global_Action() {

  double inv_Delta_t2 = 1 / pow(param_.Delta_t, 2);

  double E = 0.0;
  for (int i = 0; i < param_.Np; i++) {
    E += 0.5 * pow(path_->get_right_distance(i), 2) * inv_Delta_t2;
    E += V(path_->get_particle(i).x);
  }

  return E;
}

void PIMCClass::initE() { Esum = E2sum = 0.0; }
double PIMCClass::getE(int const &mcs) { return Esum / mcs; }
double PIMCClass::getV_E(int const &mcs) { return E2sum / mcs; }

Particle PIMCClass::update_local_path(int const &j) {

  Particle p_trial, p;

  p_trial = p = path_->get_particle(j);

  p_trial.x += rand_delta(randgen_);

  double tmpE1, tmpE2;
  double inv_Delta_t2 = 1 / pow(param_.Delta_t, 2);

  path_->set_particle(j, p_trial);
  tmpE1 = 0.5 * pow(path_->get_left_distance(j), 2) * inv_Delta_t2;
  tmpE1 += 0.5 * pow(path_->get_right_distance(j), 2) * inv_Delta_t2;
  tmpE1 += V(p_trial.x);

  path_->set_particle(j, p);
  tmpE2 = 0.5 * pow(path_->get_left_distance(j), 2) * inv_Delta_t2;
  tmpE2 += 0.5 * pow(path_->get_right_distance(j), 2) * inv_Delta_t2;
  tmpE2 += V(p.x);

  double Delta_E = tmpE1 - tmpE2;
  double w = exp(-Delta_E * param_.Delta_t);

  if (std::min(1.0, w) > randu(randgen_)) {

    path_->set_particle(j, p_trial);
    accept += 1;

  } // else 受け入れない

  p = path_->get_particle(j);

  double E = T(p.x) + V(p.x);
  Esum += E;
  E2sum += E * E;
  return path_->get_particle(j);
}
