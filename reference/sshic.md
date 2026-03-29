# Measurement of Spatial Stratified Heterogeneity Based on Information Consistency for Continuous Variables

Measurement of Spatial Stratified Heterogeneity Based on Information
Consistency for Continuous Variables

## Usage

``` r
sshic(d, s, seed = 42, permutation_number = 999, bin_method = "Sturges")
```

## Arguments

- d:

  The target variable.

- s:

  The stratification.

- seed:

  (optional) Random number seed, default is `42`.

- permutation_number:

  (optional) Number of Random Permutations, default is `999`.

- bin_method:

  (optional) Histogram binning method for probability density
  estimation, default is `Sturges`.

## Value

A two-element numerical vector.

## Examples

``` r
baltim = sf::read_sf(system.file("extdata/baltim.gpkg",package = "sshicm"))
sshic(baltim$PRICE,baltim$DWELL)
#>        Ic        Pv 
#> 0.1239052 0.7157157 
```
