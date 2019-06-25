#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

  int Np_min = 10;
  int Np_max = 50;

  double beta_min = 1;
  double beta_max = 1;

  double delta_min = 0.2;
  double delta_max = 1;

  double Delta_p = 0.01;

  int relaxation_steps = 100000;
  int MC_steps = 1000000;

  double a_min = -3;
  double a_max = 3;

  double b_min = 0.5;
  double b_max = 1;

  int N = 5;
  int nshow = 100000000;

  int index = 0;

  string dir = "./params/";
  string logdir = "./log/";

  for (int Np = Np_min; Np <= Np_max; Np += 10) {

    for (double beta = beta_min; beta <= beta_max; beta += 0.2) {

      for (double delta = delta_min; delta <= delta_max; delta += 0.2) {

        for (double a = a_min; a <= a_max; a += 0.5) {

          for (double b = a + b_min; b <= a + b_max; b += 0.5) {

            string name = "param" + to_string(index) + ".txt";
            string filename = dir + name;
            string logfilename = logdir + "log" + to_string(index) + ".txt";
            ofstream output(filename);

            output << "Np\t" << Np << endl;
            output << "beta\t" << beta << endl;
            output << "delta\t" << delta << endl;
            output << "Delta_p\t" << Delta_p << endl;
            output << "relaxation_steps\t" << relaxation_steps << endl;
            output << "MC_steps\t" << MC_steps << endl;
            output << "a\t" << a << endl;
            output << "b\t" << b << endl;
            output << "N\t" << N << endl;
            output << "nshow\t" << nshow << endl;
            output << "logfile\t" << logfilename;
            cout << filename << endl;

            index += 1;
          } // b
        }   // a
      }     // delta
    }       // beta
  }         // Np

  return 0;
}
