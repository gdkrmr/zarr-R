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

echo "== R CMD check: version $pkgversion ==================="
R CMD check zarr_$pkgversion.tar.gz --as-cran --timings
