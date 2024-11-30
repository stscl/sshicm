#ifndef HistogramDensityEst_H
#define HistogramDensityEst_H

#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <numeric>

// Histogram-based density estimation
std::vector<std::pair<double, double>> HistogramDensityEst(const std::vector<double>& data,
                                                           const std::string& bin_method);
// Compute density using predefined bins
std::vector<std::pair<double, double>> HistogramDensityEstWithBins(const std::vector<double>& data,
                                                                   const std::vector<double>& bins);

#endif // HistogramDensityEst_H
