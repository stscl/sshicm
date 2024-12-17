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

  // Step 3: Generate a random seed using the input seed
  std::mt19937 seed_gen(seed);  // Initialize random number generator with the input seed
  // std::uniform_int_distribution<> dis(1, std::numeric_limits<int>::max());  // Define a distribution for random integers
  std::uniform_int_distribution<> dis(1, 100);
  int randomseed = dis(seed_gen);  // Generate a random integer using the input seed

  // Step 4: Perform parallel computation
  RcppThread::parallelFor(0, permutation_number, [&](size_t i) {
    // Step 4.1: Generate a unique seed for each permutation by adding the iteration index to the randomseed
    std::mt19937 local_gen(randomseed + i);  // Modify seed for each thread

    // Step 4.2: Permute d
    std::vector<double> permuted_d = d;  // Copy the original d
    std::shuffle(permuted_d.begin(), permuted_d.end(), local_gen); // Shuffle based on the unique seed for each thread

    // Step 4.3: Compute IC for the permuted d
    IC_results[i] = IC_SSH(permuted_d, s, bin_method);
  });

  // Step 5: Compute p-value by comparing permuted IC values to the true IC value
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
//                               unsigned int seed,
//                               int permutation_number,
//                               const std::string& bin_method = "Sturges") {
//   if (s.size() != d.size()) {
//     throw std::invalid_argument("Vectors s and d must have the same length.");
//   }
//
//   // Step 1: Calculate the true IC value using the original d and s
//   double true_IC = IC_SSH(d, s, bin_method);
//
//   // Step 2: Generate random permutations of s and compute IC for each
//   std::vector<double> IC_results(permutation_number, 0.0);  // Store IC values for each permutation
//
//   // Initialize the random number generator with the seed
//   std::mt19937 gen(seed);
//
//   // Step 3: Perform parallel computation
//   RcppThread::parallelFor(0, permutation_number, [&](size_t i) {
//     // Generate a unique seed for each permutation
//     std::mt19937 local_gen(seed + i);  // Modify seed for each thread
//
//     // Step 3.1: Permute d
//     std::vector<double> permuted_d = d;  // Copy the original d
//     std::shuffle(permuted_d.begin(), permuted_d.end(), local_gen); // Shuffle based on the unique seed for each thread
//
//     // Step 3.2: Compute IC for the permuted d
//     IC_results[i] = IC_SSH(permuted_d, s, bin_method);
//   });
//
//   // Step 4: Compute p-value by comparing permuted IC values to the true IC value
//   int greater_count = 0;
//   for (size_t i = 0; i < IC_results.size(); ++i) {
//     if (IC_results[i] >= true_IC) {
//       greater_count++;
//     }
//   }
//
//   double p_value = static_cast<double>(greater_count) / permutation_number;
//
//   // Return a vector containing the true IC and p-value
//   return {true_IC, p_value};
// }
