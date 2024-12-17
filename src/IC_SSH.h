#ifndef IC_SSH_H
#define IC_SSH_H

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <random>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include "HistogramDensityEst.h"
#include "RelEntropy.h"
#include <RcppThread.h>

double IC_SSH(const std::vector<double>& d,
              const std::vector<int>& s,
              const std::string& bin_method);

std::vector<double> IC_SSHICM(const std::vector<double>& d,
                              const std::vector<int>& s,
                              unsigned int seed,
                              int permutation_number,
                              const std::string& bin_method = "Sturges");

#endif // IC_SSH_H
