#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <stdexcept>

// Compute IQR (Interquartile Range)
double ComputeIQR(const std::vector<double>& data) {
  std::vector<double> sorted_data = data;
  std::sort(sorted_data.begin(), sorted_data.end());
  size_t n = sorted_data.size();

  double q1 = sorted_data[n / 4];
  double q3 = sorted_data[3 * n / 4];
  return q3 - q1;
}

// Compute bin width or bin count based on different methods
int CalculateBins(const std::vector<double>& data, const std::string& method) {
  size_t n = data.size();
  if (n < 2) {
    throw std::invalid_argument("Data size must be at least 2.");
  }

  if (method == "SquareRoot") {
    return static_cast<int>(std::ceil(std::sqrt(n)));
  } else if (method == "Scott") {
    // Compute standard deviation
    double mean = std::accumulate(data.begin(), data.end(), 0.0) / n;
    double variance = std::inner_product(data.begin(), data.end(), data.begin(), 0.0) / n - mean * mean;
    double stddev = std::sqrt(variance);

    double bin_width = 3.49 * stddev / std::cbrt(n);
    return static_cast<int>(std::ceil((data.back() - data.front()) / bin_width));
  } else if (method == "FreedmanDiaconis") {
    double iqr = ComputeIQR(data);
    double bin_width = 2 * iqr / std::cbrt(n);
    return static_cast<int>(std::ceil((data.back() - data.front()) / bin_width));
  } else if (method == "Sturges") {
    return static_cast<int>(std::ceil(std::log2(n) + 1));
  } else {
    throw std::invalid_argument("Unknown binning method.");
  }
}

// Histogram-based density estimation
std::vector<std::pair<double, double>> HistogramDensityEst(const std::vector<double>& data,
                                                           const std::string& bin_method) {
  size_t n = data.size();
  if (n < 2) {
    throw std::invalid_argument("Data size must be at least 2.");
  }

  // Sort data for range computation
  std::vector<double> sorted_data = data;
  std::sort(sorted_data.begin(), sorted_data.end());
  double min_val = sorted_data.front();
  double max_val = sorted_data.back();

  // Calculate bins and bin width
  int bins = CalculateBins(sorted_data, bin_method);
  double bin_width = (max_val - min_val) / bins;

  // Initialize histogram bins
  std::vector<int> counts(bins, 0);

  // Count data points in each bin
  for (double value : data) {
    int bin_index = static_cast<int>((value - min_val) / bin_width);
    if (bin_index == bins) { // Handle edge case where value == max_val
      bin_index -= 1;
    }
    counts[bin_index]++;
  }

  // Compute density
  std::vector<std::pair<double, double>> density;
  for (int i = 0; i < bins; ++i) {
    double bin_center = min_val + (i + 0.5) * bin_width; // Bin center
    double bin_density = static_cast<double>(counts[i]) / (n * bin_width); // Probability density
    density.emplace_back(bin_center, bin_density);
  }

  return density;
}
