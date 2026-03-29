# sshicm

logo by Sibyl

**Information Consistency-Based Measures for Spatial Stratified
Heterogeneity**

## Installation

- Install from [CRAN](https://CRAN.R-project.org/package=sshicm) with:

``` r
install.packages("sshicm", dep = TRUE)
```

- Install binary version from
  [R-universe](https://stscl.r-universe.dev/sshicm) with:

``` r
install.packages("sshicm",
                 repos = c("https://stscl.r-universe.dev",
                           "https://cloud.r-project.org"),
                 dep = TRUE)
```

- Install from source code on [GitHub](https://github.com/stscl/sshicm)
  with:

``` r
if (!requireNamespace("devtools")) {
    install.packages("devtools")
}
devtools::install_github("stscl/sshicm",
                         build_vignettes = TRUE,
                         dep = TRUE)
```
