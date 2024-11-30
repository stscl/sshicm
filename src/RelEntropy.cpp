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
                  const std::string& bin_method) {
  if (DIvec.empty() || Dvec.empty()) {
    throw std::invalid_argument("Input vectors must not be empty.");
  }

  // Step 1: Filter Dvec based on the range of DIvec
  double min_DI = *std::min_element(DIvec.begin(), DIvec.end());
  double max_DI = *std::max_element(DIvec.begin(), DIvec.end());
  std::vector<double> filtered_Dvec;
  for (double value : Dvec) {
    if (value >= min_DI && value <= max_DI) {
      filtered_Dvec.push_back(value);
    }
  }
  if (filtered_Dvec.empty()) {
    throw std::invalid_argument("No elements in Dvec are within the range of DIvec.");
  }

  // Step 2: Compute density FD for filtered_Dvec
  std::vector<std::pair<double, double>> FD = HistogramDensityEst(filtered_Dvec, bin_method);

  // Extract bin centers from FD
  std::vector<double> bins;
  for (const auto& pair : FD) {
    bins.push_back(pair.first); // Extract bin centers
  }

  // Step 3: Compute density FDI for DIvec using the same bins
  std::vector<std::pair<double, double>> FDI = HistogramDensityEstWithBins(DIvec, bins);

  // Step 4: Compute relative entropy
  double rel_entropy = 0.0;
  for (size_t i = 0; i < FD.size(); ++i) {
    double fd = FD[i].second;   // Density from FD
    double fdi = FDI[i].second; // Density from FDI
    if (fd > 0 && fdi > 0) { // Avoid log(0) and division by zero
      rel_entropy += fdi * std::log(fdi / fd);
    }
  }

  return rel_entropy;
}
