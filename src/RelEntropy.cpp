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

  // Extract number of bins used in FD
  size_t bin_count = FD.size();

  // Step 3: Calculate actual bin right edges
  std::vector<double> bins;
  double bin_width = (max_DI - min_DI) / bin_count; // Calculate bin width
  for (size_t i = 1; i <= bin_count; ++i) {
    bins.push_back(min_DI + i * bin_width); // Calculate right edge of each bin
  }

  // Step 4: Compute density FDI for DIvec using the same bins
  std::vector<std::pair<double, double>> FDI = HistogramDensityEstWithBins(DIvec, bins);

  // Step 5: Compute relative entropy
  double rel_entropy = 0.0;
  double integral_step = (FD[1].first - FD[0].first);
  for (size_t i = 0; i < bin_count; ++i) {
    double fd = FD[i].second;   // Density from FD
    double fdi = FDI[i].second; // Density from FDI
    if (fd > 0 && fdi > 0) { // Avoid log(0) and division by zero
      rel_entropy += fdi * std::log(fdi / fd) * integral_step;
    }
  }

  return rel_entropy;
}

