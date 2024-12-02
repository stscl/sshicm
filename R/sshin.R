#' Measurement of Spatial Stratified Heterogeneity Based on Information Consistency for Nominal Variables
#'
#' @param d The target variable.
#' @param s The stratification.
#' @param seed (optional) Random number seed, default is `42`.
#' @param permutation_number (optional) Number of Random Permutations, default is `999`.
#'
#' @return A two-element numerical vector.
#' @export
#'
#' @examples
#' \donttest{
#' # This code may take a bit longer to execute:
#' cinc = sf::read_sf(system.file("extdata/cinc.gpkg",package = "sshicm"))
#' sshin(cinc$THEFT_D,cinc$MALE)
#' }
sshin = \(d, s, seed = 42,
          permutation_number = 999) {
  d = as.integer(as.factor(d))
  s = as.integer(as.factor(s))
  res = IN_SSHICM(d,s,seed,permutation_number)
  names(res) = c("In","Pv")
  return(res)
}
