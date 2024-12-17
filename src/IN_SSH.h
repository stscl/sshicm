#ifndef IN_SSH_H
#define IN_SSH_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <RcppThread.h>

double IN_SSH(const std::vector<int>& d,
              const std::vector<int>& s);

std::vector<double> IN_SSHICM(const std::vector<int>& d,
                              const std::vector<int>& s,
                              unsigned int seed,
                              int permutation_number);

#endif // IN_SSH_H
