#' Measurement of Spatial Stratified Heterogeneity Based on Information Consistency for Continuous Variables
#'
#' @param d The target variable.
#' @param s The stratification.
#' @param seed (optional) Random number seed, default is `42`.
#' @param permutation_number (optional) Number of Random Permutations, default is `999`.
#' @param bin_method (optional) Histogram binning method for probability density estimation, default is
#' `Sturges`.
#'
#' @return A two-element numerical vector.
#' @export
#'
#' @examples
#' \donttest{
#' # This code may take a bit longer to execute:
#' baltim = sf::read_sf(system.file("extdata/baltim.gpkg",package = "sshicm"))
#' sshic(baltim$PRICE,baltim$DWELL)
#' }
sshic = \(d, s, seed = 42,
          permutation_number = 999,
          bin_method = "Sturges") {
  s = as.integer(as.factor(s))
  res = IC_SSHICM(d,s,seed,permutation_number,bin_method)
  names(res) = c("Ic","Pv")
  return(res)
}
