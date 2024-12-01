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

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::depends(RcppThread)]]

// Compute IC_SSH
// [[Rcpp::export]]
double IC_SSH(const std::vector<double>& d,
              const std::vector<int>& s,
              const std::string& bin_method) {
  if (s.size() != d.size()) {
    throw std::invalid_argument("Vectors s and d must have the same length.");
  }

  // Step 1: Group `d` by unique values in `s` using std::map to ensure ordered keys
  std::map<int, std::vector<double>> grouped_d;
  for (size_t i = 0; i < s.size(); ++i) {
    grouped_d[s[i]].push_back(d[i]);
  }

  // Step 2: Compute p(s_i) for each unique value in `s` using the same order
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

// IC_SSHICM: Parallel computation of IC_SSH over permutations, returning IC value and p-value
// [[Rcpp::export]]
std::vector<double> IC_SSHICM(const std::vector<double>& d,
                              const std::vector<int>& s,
                              unsigned int seed,
                              int permutation_number,
                              const std::string& bin_method = "Sturges") {
  if (s.size() != d.size()) {
    throw std::invalid_argument("Vectors s and d must have the same length.");
  }

  // Step 1: Calculate the true IC value using the original d and s
  double true_IC = IC_SSH(d, s, bin_method);

  // Step 2: Generate random permutations of s and compute IC for each
  std::vector<double> IC_results(permutation_number, 0.0);  // Store IC values for each permutation

  // Initialize the random number generator with the seed
  std::mt19937 gen(seed);

  // Step 3: Perform parallel computation
  RcppThread::parallelFor(0, permutation_number, [&](size_t i) {
    // Generate a unique seed for each permutation
    std::mt19937 local_gen(seed + i);  // Modify seed for each thread

    // Step 3.1: Permute d
    std::vector<double> permuted_d = d;  // Copy the original d
    std::shuffle(permuted_d.begin(), permuted_d.end(), local_gen); // Shuffle based on the unique seed for each thread

    // Step 3.2: Compute IC for the permuted d
    IC_results[i] = IC_SSH(permuted_d, s, bin_method);
  });

  // Step 4: Compute p-value by comparing permuted IC values to the true IC value
  int greater_count = 0;
  for (size_t i = 0; i < IC_results.size(); ++i) {
    if (IC_results[i] >= true_IC) {
      greater_count++;
    }
  }

  double p_value = static_cast<double>(greater_count) / permutation_number;

  // Return a vector containing the true IC and p-value
  return {true_IC, p_value};
}


// // IC_SSHICM: Parallel computation of IC_SSH over permutations, returning IC value and p-value
// // [[Rcpp::export]]
// std::vector<double> IC_SSHICM(const std::vector<double>& d,
//                               const std::vector<int>& s,
//                               const std::vector<std::vector<int>>& permutation,
//                               const std::string& bin_method = "SquareRoot") {
//   if (s.size() != d.size()) {
//     throw std::invalid_argument("Vectors s and d must have the same length.");
//   }
//
//   size_t num_permutations = permutation.size();  // Number of permutations
//   std::vector<double> IC_results(num_permutations, 0.0);  // Store IC values for each permutation
//
//   // Calculate the true IC value using the original d and s
//   double true_IC = IC_SSH(d, s, bin_method);
//
//   // Parallel computation using RcppThread
//   RcppThread::parallelFor(0, num_permutations, [&](size_t i) {
//     // Extract a single permutation from the matrix
//     std::vector<int> permuted_s(s.size());
//     for (size_t j = 0; j < s.size(); ++j) {
//       permuted_s[j] = permutation[i][j];  // Permute s based on the current permutation
//     }
//
//     // Compute IC for the current permutation
//     IC_results[i] = IC_SSH(d, permuted_s, bin_method);
//   });
//
//   // Compute p-value by comparing permuted IC values to the true IC value
//   int greater_count = 0;
//   for (size_t i = 0; i < IC_results.size(); ++i) {
//     if (IC_results[i] >= true_IC) {
//       greater_count++;
//     }
//   }
//
//   double p_value = static_cast<double>(greater_count) / num_permutations;
//
//   // Return a vector containing the true IC and p-value
//   return {true_IC, p_value};
// }
