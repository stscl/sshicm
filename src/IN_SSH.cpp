#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <random>
#include <stdexcept>
#include <RcppThread.h>

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::depends(RcppThread)]]

// Function to compute the frequency of each element in a vector
std::map<int, int> ComputeFrequency(const std::vector<int>& data) {
  std::map<int, int> frequency;
  for (int value : data) {
    frequency[value]++;
  }
  return frequency;
}

// Function to compute the joint frequency of two vectors
std::map<std::pair<int, int>, int> ComputeJointFrequency(const std::vector<int>& d,
                                                         const std::vector<int>& s) {
  std::map<std::pair<int, int>, int> joint_frequency;
  for (size_t i = 0; i < d.size(); ++i) {
    joint_frequency[std::make_pair(s[i], d[i])]++;
  }
  return joint_frequency;
}

// Function to compute the probability distribution from frequency
std::map<int, double> ComputeProb(const std::map<int, int>& frequency, int total_count) {
  std::map<int, double> probability;
  for (const auto& pair : frequency) {
    probability[pair.first] = static_cast<double>(pair.second) / total_count;
  }
  return probability;
}

// Function to compute the joint probability distribution from joint frequency
std::map<std::pair<int, int>, double> ComputeJointProb(const std::map<std::pair<int, int>, int>& joint_frequency, int total_count) {
  std::map<std::pair<int, int>, double> joint_probability;
  for (const auto& pair : joint_frequency) {
    joint_probability[pair.first] = static_cast<double>(pair.second) / total_count;
  }
  return joint_probability;
}

// Function to compute the conditional probability distribution
std::map<int, double> ComputeConditionalProb(const std::map<std::pair<int, int>, double>& joint_probability,
                                             const std::map<int, double>& marginal_probability, int s_value) {
  std::map<int, double> conditional_probability;
  for (const auto& pair : joint_probability) {
    if (pair.first.first == s_value) {
      conditional_probability[pair.first.second] = pair.second / marginal_probability.at(s_value);
    }
  }
  return conditional_probability;
}

// Function to compute the entropy of a probability distribution
double ComputeEntropy(const std::map<int, double>& probability) {
  double entropy = 0.0;
  for (const auto& pair : probability) {
    if (pair.second > 0) {
      entropy -= pair.second * std::log(pair.second);
    }
  }
  return entropy;
}

// Function to compute the conditional entropy
double ComputeConditionalEntropy(const std::map<int, double>& marginal_probability,
                                 const std::map<std::pair<int, int>, double>& joint_probability) {
  double conditional_entropy = 0.0;
  for (const auto& marginal_pair : marginal_probability) {
    int s_value = marginal_pair.first;
    double s_probability = marginal_pair.second;
    std::map<int, double> conditional_probability = ComputeConditionalProb(joint_probability, marginal_probability, s_value);
    for (const auto& conditional_pair : conditional_probability) {
      double x_probability = conditional_pair.second;
      if (x_probability > 0) {
        conditional_entropy -= s_probability * x_probability * std::log2(x_probability);
      }
    }
  }
  return conditional_entropy;
}

// Function to compute IN_SSH
// [[Rcpp::export]]
double IN_SSH(const std::vector<int>& d, const std::vector<int>& s) {
  if (d.size() != s.size()) {
    throw std::invalid_argument("Vectors d and s must have the same length.");
  }

  int total_count = d.size();

  // Step 1: Compute frequency and probability distributions
  std::map<int, int> d_frequency = ComputeFrequency(d);
  std::map<int, int> s_frequency = ComputeFrequency(s);
  std::map<std::pair<int, int>, int> joint_frequency = ComputeJointFrequency(d, s);

  std::map<int, double> d_probability = ComputeProb(d_frequency, total_count);
  std::map<int, double> s_probability = ComputeProb(s_frequency, total_count);
  std::map<std::pair<int, int>, double> joint_probability = ComputeJointProb(joint_frequency, total_count);

  // Step 2: Compute entropy of d
  double I_d = ComputeEntropy(d_probability);

  // Step 3: Compute conditional entropy of d given s
  double I_d_given_s = ComputeConditionalEntropy(s_probability, joint_probability);

  // Step 4: Compute IN_SSH
  double IN_SSH_value = 1.0 - (I_d_given_s / I_d);

  return IN_SSH_value;
}

// IN_SSHICM: Parallel computation of IN_SSH over permutations, returning IN_SSH value and p-value
// [[Rcpp::export]]
std::vector<double> IN_SSHICM(const std::vector<int>& d,
                              const std::vector<int>& s,
                              unsigned int seed,
                              int permutation_number) {
  if (s.size() != d.size()) {
    throw std::invalid_argument("Vectors s and d must have the same length.");
  }

  // Step 1: Calculate the true IN_SSH value using the original d and s
  double true_IN_SSH = IN_SSH(d, s);

  // Step 2: Generate random permutations of d and compute IN_SSH for each
  std::vector<double> IN_SSH_results(permutation_number, 0.0);  // Store IN_SSH values for each permutation

  // Initialize the random number generator with the seed
  std::mt19937 gen(seed);

  // Step 3: Perform parallel computation
  RcppThread::parallelFor(0, permutation_number, [&](size_t i) {
    // Generate a unique seed for each permutation
    std::mt19937 local_gen(seed + i);  // Modify seed for each thread

    // Step 3.1: Permute d
    std::vector<int> permuted_d = d;  // Copy the original d
    std::shuffle(permuted_d.begin(), permuted_d.end(), local_gen); // Shuffle based on the unique seed for each thread

    // Step 3.2: Compute IN_SSH for the permuted d
    IN_SSH_results[i] = IN_SSH(permuted_d, s);
  });

  // Step 4: Compute p-value by comparing permuted IN_SSH values to the true IN_SSH value
  int greater_count = 0;
  for (size_t i = 0; i < IN_SSH_results.size(); ++i) {
    if (IN_SSH_results[i] >= true_IN_SSH) {
      greater_count++;
    }
  }

  double p_value = static_cast<double>(greater_count) / permutation_number;

  // Return a vector containing the true IN_SSH and p-value
  return {true_IN_SSH, p_value};
}
