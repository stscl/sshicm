# Information Consistency-Based Measures for Spatial Stratified Heterogeneity

Information Consistency-Based Measures for Spatial Stratified
Heterogeneity

## Usage

``` r
sshicm(
  formula,
  data,
  type = c("IC", "IN"),
  seed = 42,
  permutation_number = 999,
  bin_method = "Sturges"
)
```

## Arguments

- formula:

  A formula.

- data:

  A `data.frame`, `tibble` or `sf` object of observation data.

- type:

  (optional) Measure type, default is `IC`.

- seed:

  (optional) Random number seed, default is `42`.

- permutation_number:

  (optional) Number of Random Permutations, default is `999`.

- bin_method:

  (optional) Histogram binning method for probability density
  estimation, default is `Sturges`.

## Value

A `tibble`.

## Examples

``` r
if (FALSE) { # \dontrun{
# This code may take a bit longer to execute:
baltim = sf::read_sf(system.file("extdata/baltim.gpkg",package = "sshicm"))
sshicm(PRICE ~ .,baltim,type = "IC")
cinc = sf::read_sf(system.file("extdata/cinc.gpkg",package = "sshicm"))
sshicm(THEFT_D ~ .,cinc,type = "IN")
} # }
```
