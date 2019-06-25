#include "./include/iostruct.h"
#include <cstdlib>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Param {
  string param_file;
  PIMCParams param;
  double ac_rate;
  double E;
  double V_E;
};

bool cmp(const Param &a, const Param &b) { return a.V_E < b.V_E; }

int main(int argc, char const *argv[]) {

  const char *path = "./log/";
  DIR *dp;
  dirent *entry;

  dp = opendir(path); // pathを開く
  if (dp == NULL) {
    exit(1);
  }

  string filename;
  vector<string> log;
  // path以下に存在するファイルを読み込んでlog入れてリスト化する
  do {
    entry = readdir(dp);

    if (entry != NULL) {
      filename = entry->d_name;
      cout << path + filename << endl;
      log.push_back(path + filename);
    }
  } while (entry != NULL);
  closedir(dp);

  log.erase(log.begin()); // .の削除
  log.erase(log.begin()); // ..の削除

  vector<Param> params;

  // logから必要なデータを抜き出す
  for (int i = 0; i < log.size(); i++) {

    ifstream infile(log[i]);
    string in;
    Param p;

    if (infile) { // logファイルを読み込む
      cout << log[i] << endl;

      infile >> in >> p.param_file; // paramファイルのpathの読み込み

      ifstream input(p.param_file);
      p.param = readPIMCParams(input); // paramファイルを読み込む
      input.close();

      for (int i = 0; i < p.param.N; i++) {
        double tmp;

        infile >> in >> tmp; // iのskip
        // cout << in << tmp << endl;
        infile >> in >> tmp; // ac rateのskip
        // cout << in << tmp << endl;
        infile >> in >> tmp; // エネルギーのskip
        // cout << in << tmp << endl;
      }

      infile >> in >> p.E;
      infile >> in >> p.V_E;
    }

    params.push_back(p);
  }

  sort(params.begin(), params.end(), cmp);

  for (auto itr : params) {
    cout << itr.param_file << "\t" << itr.V_E << endl;
  }

  return 0;
}
