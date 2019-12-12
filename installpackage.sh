#! /bin/sh

echo "== Do Rcpp stuff ====================================="
Rscript --vanilla \
        -e 'Rcpp::compileAttributes(verbose = FALSE)'

echo "== BUILDING DOCUMENTATION ============================"
Rscript --vanilla \
        --default-packages=methods,utils \
        -e 'devtools::document()'

echo "== R CMD build ======================================="
R CMD build --compact-vignettes .


pkgversion=$(cat DESCRIPTION | grep Version | sed 's|Version: \(.*\)|\1|')

echo "== INSTALLING version $pkgversion ===================="
R CMD INSTALL --strip zarr_$pkgversion.tar.gz
