#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <stdexcept>
#include <numeric>
#include "HistogramDensityEst.h"
#include "RelEntropy.h"

// Compute IC_SSH
double IC_SSH(const std::vector<double>& d,
              const std::vector<int>& s,
              const std::string& bin_method) {
  if (s.size() != d.size()) {
    throw std::invalid_argument("Vectors s and d must have the same length.");
  }

  // Step 1: Group `d` by unique values in `s`
  std::unordered_map<int, std::vector<double>> grouped_d;
  for (size_t i = 0; i < s.size(); ++i) {
    grouped_d[s[i]].push_back(d[i]);
  }

  // Step 2: Compute p(s_i) for each unique value in `s`
  std::unordered_map<int, double> probabilities;
  for (const auto& pair : grouped_d) {
    probabilities[pair.first] = static_cast<double>(pair.second.size()) / s.size();
  }

  // Step 3: Calculate IC value
  double IC = 0.0;
  for (const auto& pair : grouped_d) {
    const int s_i = pair.first;
    const std::vector<double>& d_i = pair.second;

    // Compute relative entropy for d_i and d
    double rel_entropy = RelEntropy(d_i, d, bin_method);

    // Compute contribution to IC
    IC += probabilities[s_i] * (std::atan(rel_entropy) / (M_PI / 2));
  }

  return IC;
}
