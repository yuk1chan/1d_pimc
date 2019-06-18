
#include "../include/potential.h"

/* 調和振動子 ポテンシャル */
double V(double const &x) { return x * x / 2; }

/* モースポテンシャル */
// double V(double const &x) { return 2 * (1 - exp(-x)) * (1 - exp(-x)); }

double dV(double const &x) { return (V(x + H) - V(x)) / H; }
double T(double const &x) { return x * dV(x) / 2; }
