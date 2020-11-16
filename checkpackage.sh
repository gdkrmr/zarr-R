#! /bin/sh

RSCRIPT="/home/gkraemer/bin/Rscript-4.0.3"
R="/home/gkraemer/bin/R-4.0.3"

echo "== Do Rcpp stuff ====================================="
$RSCRIPT --vanilla \
        -e 'Rcpp::compileAttributes(verbose = FALSE)'


echo "== BUILDING DOCUMENTATION ============================"
$RSCRIPT --vanilla \
        --default-packages=methods,utils \
        -e 'devtools::document()'


echo "== R CMD build ======================================="
$R CMD build --compact-vignettes .

pkgversion=$(cat DESCRIPTION | grep Version | sed 's|Version: \(.*\)|\1|')


echo "== R CMD check: version $pkgversion ==================="
$R CMD check zarr_$pkgversion.tar.gz --as-cran --timings
