#! /bin/sh

# RSCRIPT="/home/gkraemer/bin/Rscript-3.6.2"
# R="/home/gkraemer/bin/R-3.6.2"

RSCRIPT="Rscript"
R=R

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

echo "== INSTALLING version $pkgversion ===================="
$R CMD INSTALL --strip zarr_$pkgversion.tar.gz
