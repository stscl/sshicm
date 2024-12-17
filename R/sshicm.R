#' Information Consistency-Based Measures for Spatial Stratified Heterogeneity
#'
#' @param formula A formula.
#' @param data A `data.frame`, `tibble` or `sf` object of observation data.
#' @param type (optional) Measure type, default is `IC`.
#' @param seed (optional) Random number seed, default is `42`.
#' @param permutation_number (optional) Number of Random Permutations, default is `999`.
#' @param bin_method (optional) Histogram binning method for probability density estimation, default is
#' `Sturges`.
#'
#' @return A `tibble`.
#' @export
#'
#' @examples
#' \dontrun{
#' # This code may take a bit longer to execute:
#' baltim = sf::read_sf(system.file("extdata/baltim.gpkg",package = "sshicm"))
#' sshicm(PRICE ~ .,baltim,type = "IC")
#' cinc = sf::read_sf(system.file("extdata/cinc.gpkg",package = "sshicm"))
#' sshicm(THEFT_D ~ .,cinc,type = "IN")
#' }
sshicm = \(formula, data, type = c("IC","IN"), seed = 42,
           permutation_number = 999, bin_method = "Sturges"){
  formulavar = sdsfun::formula_varname(formula,data)
  yvec = data[,formulavar[[1]],drop = TRUE]

  if (inherits(data,"sf")){
    data = sf::st_drop_geometry(data)
  }
  xtbl = dplyr::select(data,dplyr::all_of(formulavar[[2]]))

  type = match.arg(type)
  if (type == "IC"){
    res = purrr::map_dfr(xtbl,
                         \(.x) sshic(yvec,.x,seed,
                                     permutation_number,
                                     bin_method)) |>
      dplyr::mutate(Variable = names(xtbl)) |>
      dplyr::select(Variable,Ic,Pv) |>
      dplyr::arrange(dplyr::desc(Ic))
  } else {
    res = purrr::map_dfr(xtbl,
                         \(.x) sshin(yvec,.x,seed,
                                     permutation_number)) |>
      dplyr::mutate(Variable = names(xtbl)) |>
      dplyr::select(Variable,In,Pv) |>
      dplyr::arrange(dplyr::desc(In))
  }
  return(res)
}
