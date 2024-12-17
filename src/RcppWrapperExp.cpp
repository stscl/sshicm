#include <vector>
#include "IN_SSH.h"
#include "IC_SSH.h"
#include <Rcpp.h>

// Rcpp wrapper for IN_SSH
// [[Rcpp::export]]
double RcppINSSH(Rcpp::IntegerVector d, Rcpp::IntegerVector s) {
  // Convert Rcpp::IntegerVector to std::vector<int>
  std::vector<int> d_std = Rcpp::as<std::vector<int>>(d);
  std::vector<int> s_std = Rcpp::as<std::vector<int>>(s);

  // Call the IN_SSH function
  double result = IN_SSH(d_std, s_std);

  // Return the result as a double
  return result;
}

// Rcpp wrapper for IN_SSHICM
// [[Rcpp::export]]
Rcpp::NumericVector RcppINSSHICM(Rcpp::IntegerVector d,
                                 Rcpp::IntegerVector s,
                                 unsigned int seed,
                                 int permutation_number) {
  // Convert Rcpp::IntegerVector to std::vector<int>
  std::vector<int> d_std = Rcpp::as<std::vector<int>>(d);
  std::vector<int> s_std = Rcpp::as<std::vector<int>>(s);

  // Call the IN_SSHICM function
  std::vector<double> result = IN_SSHICM(d_std, s_std, seed, permutation_number);

  // Convert the std::vector<double> result to Rcpp::NumericVector
  return Rcpp::wrap(result);
}

// Rcpp wrapper for IC_SSH
// [[Rcpp::export]]
double RcppICSSH(Rcpp::NumericVector d,
                 Rcpp::IntegerVector s,
                 std::string bin_method = "Sturges") {
  // Convert Rcpp::NumericVector to std::vector<double>
  std::vector<double> d_std = Rcpp::as<std::vector<double>>(d);

  // Convert Rcpp::IntegerVector to std::vector<int>
  std::vector<int> s_std = Rcpp::as<std::vector<int>>(s);

  // Call the IC_SSH function
  double result = IC_SSH(d_std, s_std, bin_method);

  // Return the result as a double
  return result;
}

// Rcpp wrapper for IC_SSHICM
// [[Rcpp::export]]
Rcpp::NumericVector RcppICSSHICM(Rcpp::NumericVector d,
                                 Rcpp::IntegerVector s,
                                 unsigned int seed,
                                 int permutation_number,
                                 std::string bin_method = "Sturges") {
  // Convert Rcpp::NumericVector to std::vector<double>
  std::vector<double> d_std = Rcpp::as<std::vector<double>>(d);

  // Convert Rcpp::IntegerVector to std::vector<int>
  std::vector<int> s_std = Rcpp::as<std::vector<int>>(s);

  // Call the IC_SSHICM function
  std::vector<double> result = IC_SSHICM(d_std, s_std, seed, permutation_number, bin_method);

  // Convert the std::vector<double> result to Rcpp::NumericVector
  return Rcpp::wrap(result);
}
