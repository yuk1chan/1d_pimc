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

void outputE(std::vector<double> &Ev, std::ofstream &logfile) {

  double Emin = 9999, Emax = -9999;
  double Esum = 0.0;
  double Esum2 = 0.0;

  for (std::vector<double>::size_type i = 0; i < Ev.size(); i++) {

    if( Emin > Ev[i] ){
      Emin = Ev[i];
    }

    if( Emax < Ev[i] ){
      Emax = Ev[i];
    }

    Esum += Ev[i];
    Esum2 += pow(Ev[i], 2);
  }

  logfile << "result_E: " << Esum / Ev.size() << std::endl;
  logfile << "result_V[E]: " << Esum2 / Ev.size() - pow(Esum / Ev.size(), 2)
          << std::endl;
  logfile << "Emin: " << Emin << std::endl;
  logfile << "Emax: " << Emax << std::endl;
  logfile << std::endl;
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

  std::vector<PIMCClass *> pimc;

  for (int i = 0; i < params.N; i++) {
    PIMCClass *p = new PIMCClass(params);
    pimc.push_back(p);
  }

#pragma omp parallel for
  for (int i = 0; i < params.N; i++) {

    for (int imcs = 1; imcs <= params.relaxation_steps; imcs++) {
      for (int Ni = 0; Ni < params.Np; Ni++) {
        // pimc[i]->update_local_path();
        pimc[i]->update_local_path(Ni);
      }

      if (imcs % params.nshow == 0) {
        // pimc[i]->update_global_path();
        std::cout << "Thread number[" << omp_get_thread_num() << "]";
        std::cout << " imcs :" << imcs << std::endl;
        std::cout << " Accept rate:"
                  << (double)pimc[i]->getAccept() / (imcs * params.Np);
        std::cout << std::endl;
        std::cout << "path エネルギー: " << pimc[i]->get_global_Action()
                  << std::endl;
      }
    }
  }

  // 平衡化より前のAcceptおよびエネルギーの初期化
  for (int i = 0; i < params.N; i++) {
    pimc[i]->setAccept(0);
    pimc[i]->initE();
  }

#pragma omp parallel for
  for (int i = 0; i < params.N; i++) {
    for (int imcs = 1; imcs <= params.MC_steps; imcs++) {

      Particle p;
      double E;
      for (int Ni = 0; Ni < params.Np; Ni++) {
        // p = pimc[i]->update_local_path();
        p = pimc[i]->update_local_path(Ni);
        pimc[i]->countP(p);
      }

      if (imcs % params.nshow == 0) {
        // pimc[i]->update_global_path();
        std::cout << "Thread number[" << omp_get_thread_num() << "]";
        std::cout << "imcs :" << imcs << std::endl;
        std::cout << "Accept rate: "
                  << (double)pimc[i]->getAccept() / (imcs * params.Np);
        std::cout << std::endl;
        std::cout << "path エネルギー: " << pimc[i]->get_global_Action()
                  << std::endl;
        E = pimc[i]->calcE(imcs);
        std::cout << "エネルギー calc from P(x): " << E << std::endl;
        E = pimc[i]->getE(imcs * params.Np); // calc from path
        pimc[i]->outputE(E);

        std::string dir = "./path_data/";
        std::string filename = dir + "data";
        filename +=
            std::to_string(i) + "MCstep" + std::to_string(imcs) + ".txt";

        std::ofstream output(filename);
        pimc[i]->outputPath(output);
        output.close();
      }
    }
  }

  std::ofstream logfile(params.logfilename);

  logfile << "params_file: " << argv[1] << std::endl;
  // logfile << "---- output ---" << std::endl;
  std::vector<double> Ev;
  for (int i = 0; i < params.N; i++) {
    double E;
    logfile << "i: " << i << std::endl;
    logfile << "Accept_rate: "
            << (double)pimc[i]->getAccept() / (params.MC_steps * params.Np);
    logfile << std::endl;
    // E = pimc[i]->calcE(params.MC_steps);
    E = pimc[i]->getE(params.MC_steps * params.Np);
    logfile << "エネルギー: " << E << std::endl;
    Ev.push_back(E);
    logfile << "path エネルギー: " << pimc[i]->get_global_Action() << std::endl;
  }

  // logfile << "\n---- result ---" << std::endl;
  outputE(Ev, logfile);
  std::string dir = "./data/";
  for (int i = 0; i < params.N; i++) {
    std::string filename = dir + "data" + std::to_string(i) + ".txt";
    std::ofstream output(filename);
    pimc[i]->outputP(output, params.MC_steps);
    output.close();
  }

  dir = "./path_data/";
#pragma omp parallel for
  for (int i = 0; i < params.N; i++) {
    std::string filename = dir + "data" + std::to_string(i) + ".txt";
    std::ofstream output(filename);
    pimc[i]->outputPath(output);
    output.close();
  }

  return 0;
}
