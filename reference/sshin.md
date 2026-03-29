# Measurement of Spatial Stratified Heterogeneity Based on Information Consistency for Nominal Variables

Measurement of Spatial Stratified Heterogeneity Based on Information
Consistency for Nominal Variables

## Usage

``` r
sshin(d, s, seed = 42, permutation_number = 999)
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

## Value

A two-element numerical vector.

## Examples

``` r
cinc = sf::read_sf(system.file("extdata/cinc.gpkg",package = "sshicm"))
sshin(cinc$THEFT_D,cinc$MALE)
#>         In         Pv 
#> 0.03671401 0.00000000 
```
