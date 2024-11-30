#ifndef RelEntropy_H
#define RelEntropy_H

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "HistogramDensityEst.h"

// Relative Entropy computation
double RelEntropy(const std::vector<double>& DIvec,
                  const std::vector<double>& Dvec,
                  const std::string& bin_method);

#endif // RelEntropy_H
