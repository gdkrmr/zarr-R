library(testthat)
library(zarr)


## test_check("zarr", reporter = LocationReporter)
test_check("zarr", reporter = SummaryReporter, filter = "^test_.*.R$")
