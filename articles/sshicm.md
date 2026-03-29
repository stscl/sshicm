# Information Consistency-Based Measures for Spatial Stratified Heterogeneity

 

## 1. Introduction to `sshicm` package

### 1.1 The `sshicm` package can be used to address following issues:

- Information consistency-based measures of spatial stratified
  heterogeneity intensity for continuous and nominal variables.

- Strength of spatial pattern associations based on information
  consistency measures.

### 1.2 Example data in the `sshicm` package

#### baltim data

“baltim” consists of [Baltimore home sale prices and
hedonics](https://geodacenter.github.io/data-and-lab/baltim/). In total,
there are 221 instances in “baltim” data. The explanatory variables are
whether it is a detached unit (DWELL), whether it has a patio (PATIO),
whether it has a fireplace (FIREPL), whether it has air conditioning
(AC), and whether the dwelling is in Baltimore County (CITCOU, while the
target variable is the sale price of the home (PRICE).

#### cinc data

“cinc” is derived from [the 2008 Cincinnati Crime + Socio-Demographics
dataset](https://geodacenter.github.io/data-and-lab/walnut_hills/). It
includes spatial data on 457 objects located on an irregular lattice.
The explanatory variables are male population (MALE), female population
(FEMALE), median age (MEDIAN_AGE), average family size (AVG_FAMSIZ), and
population density (DENSITY), while the target variable is the existence
of theft (THEFT_D).

![Figure 1. Maps of the baltim and cinc data sets. (Bai et
al. 2023)](../reference/figures/sshicm/sshicm_example_data.jpg)

**Figure 1**. Maps of the baltim and cinc data sets. ([Bai et
al. 2023](https://doi.org/10.1080/24694452.2023.2223700))

### 1.3 Functions in the `sshicm` package

#### Two functions for vector-type inputs of dependent and independent variables.

- [`sshic()`](https://stscl.github.io/sshicm/reference/sshic.md) for
  continuous dependent variable

- [`sshin()`](https://stscl.github.io/sshicm/reference/sshin.md) for
  continuous nominal variable

#### Regression-style data frame modeling function

A function
[`sshicm()`](https://stscl.github.io/sshicm/reference/sshicm.md) that
yields all results in a single line, with the `type` parameter set to
`IC` (Continuous) or `IN` (Nominal) to specify whether the dependent
variable is a continuous or nominal variable.

## 2. The principle of measuring spatial stratified heterogeneity based on information consistency

**Note: All explanatory variables must be discretized in advance or
inherently be discrete nominal variables.**

### 2.1 When the dependent variable is a continuous variable:

\\ I\_{C}\left(d,s\right) = \sum\_{s\_{i} \in
S}p\left(s\_{i}\right)\frac{ \arctan \left(\textbf{RelE} \left(
f\_{d\_{i}} \mid \mid f \right) \right)}{\pi / 2} \\

where \\d_i\\ is the random variable corresponding to the target
variable in stratum \\s_i\\ , and \\f\_{d_i}\\ and \\f\\ are the density
functions of \\d_i\\ and \\d\\, respectively. Additionally,
\\\textbf{RelE} \left( f\_{d\_{i}} \mid \mid f \right)\\ is the relative
entropy of \\f\_{d_i}\\ and \\f\\.

\\ \textbf{RelE} \left( f\_{d\_{i}} \mid \mid f \right) = H
\left(f\_{d\_{i}} , f\right) - H \left(f\_{d\_{i}}\right) = \sum\_{i =
1}^{n} f\_{d\_{i}} \log \frac{1}{f} - \sum\_{i = 1}^{n} f\_{d\_{i}} \log
\frac{1}{f\_{d\_{i}}} = \sum\_{i = 1}^{n} f\_{d\_{i}} \log
\frac{f\_{d\_{i}}}{f} \\

### 2.2 When the dependent variable is a nominal variable:

\\ I\_{N}\left(d,s\right) = \frac{I \left(d,s\right)}{I \left(d\right)}
= \frac{I \left(d\right) - I \left(d \mid s\right)}{I \left(d\right)} =
1 - \frac{\sum\_{s_i \in S}\sum\_{x \in V_d} p\left(s_i,x\right) \log
p\left(x \mid s_i\right)}{\sum\_{x \in V_d} p\left(x\right) \log
p\left(x\right)} \\

where \\p\left(x\right)\\ is the probability of observing \\x\\ in
\\U\\, \\p\left(s_i,x\right)\\ is the probability of observing \\s_i\\
and \\x\\ in \\U\\, and \\p\left(x \mid s_i\right)\\ is the probability
of observing \\x\\ given that the stratum is \\s_i\\.

## 3. Examples of the `sshicm` package

``` r
install.packages("sshicm", dep = TRUE)
```

``` r
library(sshicm)
```

``` r
baltim = sf::read_sf(system.file("extdata/baltim.gpkg",package = "sshicm"))
sshicm(PRICE ~ .,baltim,type = "IC")
## # A tibble: 5 × 3
##   Variable    Ic      Pv
##   <chr>    <dbl>   <dbl>
## 1 CITCOU   0.703 0.00200
## 2 AC       0.223 0.0821 
## 3 PATIO    0.162 0.668  
## 4 FIREPL   0.135 0.680  
## 5 DWELL    0.124 0.735
```

``` r
cinc = sf::read_sf(system.file("extdata/cinc.gpkg",package = "sshicm"))
sshicm(THEFT_D ~ .,cinc,type = "IN")
## # A tibble: 5 × 3
##   Variable        In      Pv
##   <chr>        <dbl>   <dbl>
## 1 DENSITY    0.776   0.0681 
## 2 MEDIAN_AGE 0.228   0.0230 
## 3 MALE       0.0367  0      
## 4 AVG_FAMSIZ 0.0205  0.00300
## 5 FEMALE     0.00584 0.0190
```

``` r
ntds = gdverse::NTDs
sshicm(incidence ~ watershed + elevation + soiltype,data = ntds)
## # A tibble: 3 × 3
##   Variable     Ic     Pv
##   <chr>     <dbl>  <dbl>
## 1 watershed 0.281 0.0911
## 2 elevation 0.238 0.103 
## 3 soiltype  0.118 0.126
```

## Reference

Wang, J., Haining, R., Zhang, T., Xu, C., Hu, M., Yin, Q., … Chen, H.
(2024). Statistical Modeling of Spatially Stratified Heterogeneous Data.
Annals of the American Association of Geographers, 114(3), 499–519.
<https://doi.org/10.1080/24694452.2023.2289982>.

Bai, H., Wang, H., Li, D., & Ge, Y. (2023). Information
Consistency-Based Measures for Spatial Stratified Heterogeneity. Annals
of the American Association of Geographers, 113(10), 2512–2524.
<https://doi.org/10.1080/24694452.2023.2223700>.

Wang, J., Li, X., Christakos, G., Liao, Y., Zhang, T., Gu, X., & Zheng,
X. (2010). Geographical Detectors‐Based Health Risk Assessment and its
Application in the Neural Tube Defects Study of the Heshun Region,
China. International Journal of Geographical Information Science, 24(1),
107–127. <https://doi.org/10.1080/13658810802443457>.

Wang, J. F., Zhang, T. L., & Fu, B. J. A measure of spatial stratified
heterogeneity. Ecological indicators, 2016. 67, 250-256.
<https://doi.org/10.1016/j.ecolind.2016.02.052>.

 

   
