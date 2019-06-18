#include "../include/iostruct.h"
#include "../include/particle.h"
#include "../include/pathclass.h"
#include "../include/pimcclass.h"
#include "../include/potential.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <vector>

void outputE(std::vector<double> &Ev) {

  double Esum = 0.0;
  double Esum2 = 0.0;

  for (std::vector<double>::size_type i = 0; i < Ev.size(); i++) {
    Esum += Ev[i];
    Esum2 += pow(Ev[i], 2);
  }

  std::cout << "エネルギー :" << Esum / Ev.size() << std::endl;
  std::cout << "エネルギーの分散 :"
            << Esum2 / Ev.size() - pow(Esum / Ev.size(), 2) << std::endl;
  std::cout << std::endl;
}

int main(int argc, char const *argv[]) {

  if (argc != 2) {
    std::cout << "Usage: " << argv[0] << "input parameter file" << std::endl;
    return 0;
  }

  std::ifstream input_param(argv[1]);
  if (!input_param) {
    std::cerr << " Error: input file not found." << std::endl;
    input_param.close();
    return 1;
  }

  PIMCParams params = readPIMCParams(input_param);
  input_param.close();

  // init omp
  int num_of_threads = omp_get_max_threads();
  omp_set_num_threads(num_of_threads);
  // std::cout << num_of_threads << std::endl;

  //  PIMCClass p1(params);
  // PIMCClass p2(params);
  std::vector<PIMCClass *> pimc;

  for (int i = 0; i < params.N; i++) {
    PIMCClass *p = new PIMCClass(params);
    pimc.push_back(p);
  }

  int nshow = 10000;

#pragma omp parallel for
  for (int i = 0; i < params.N; i++) {

    for (int imcs = 1; imcs <= params.relaxation_steps; imcs++) {
      for (int Ni = 0; Ni < params.Np; Ni++) {
        pimc[i]->update_path();
      }

      if (imcs % nshow == 0) {
        std::cout << "Thread number[" << omp_get_thread_num() << "]";
        std::cout << " imcs :" << imcs << std::endl;
        std::cout << " Accept rate:" << (double)imcs / pimc[i]->getAccept();
        std::cout << std::endl;
      }
    }
  }

  // 平衡化より前のAcceptの初期化
  for (int i = 0; i < params.N; i++) {
    pimc[i]->setAccept(0);
  }

#pragma omp parallel for
  for (int i = 0; i < params.N; i++) {
    for (int imcs = 1; imcs <= params.MC_steps; imcs++) {

      Particle p;
      double E;
      for (int Ni = 0; Ni < params.Np; Ni++) {
        p = pimc[i]->update_path();
        pimc[i]->countP(p);
      }

      if (imcs % nshow == 0) {
        std::cout << "Thread number[" << omp_get_thread_num() << "]";
        std::cout << "imcs :" << imcs << std::endl;
        std::cout << "Accept rate:" << (double)imcs / pimc[i]->getAccept()
                  << std::endl;
        E = pimc[i]->calcE(imcs);
        pimc[i]->outputE(E);
      }
    }
  }

  std::cout << "---- output ---" << std::endl;

  std::vector<double> Ev;
  for (int i = 0; i < params.N; i++) {
    double E;
    std::cout << "i : " << i << std::endl;
    std::cout << "Accept rate:"
              << (double)params.MC_steps / pimc[i]->getAccept() << std::endl;
    E = pimc[i]->calcE(params.MC_steps);
    Ev.push_back(E);
    pimc[i]->outputE(E);
  }

  std::cout << "---- result ---" << std::endl;
  outputE(Ev);

  std::string dir = "./data/";
  for (int i = 0; i < params.N; i++) {
    std::string filename = dir + "data" + std::to_string(i) + ".txt";
    std::ofstream output(filename);
    pimc[i]->outputP(output, params.MC_steps);
    output.close();
  }

  return 0;
}
