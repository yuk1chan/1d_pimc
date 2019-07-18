#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char const *argv[]) {

  int Np = 50;

  double beta = 1.0;

  double delta = 1.0;

  double Delta_p = 0.001;

  int relaxation_steps = 100000;
  int MC_steps = 1000000;

  double a = -3;
  double b = 3;

  int N = 50;
  int nshow = 100000000;

  int index = 0;
  int max = 100;
  for( int i = 0; i < max; i++ ){


    string dir = "./params/";
    string logdir = "./log/";

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
  }

  return 0;
}
