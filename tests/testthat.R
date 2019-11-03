library(testthat)
library(zarr)


test_check("zarr", reporter = LocationReporter)
