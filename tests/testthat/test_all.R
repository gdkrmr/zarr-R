## library(testthat)
## library(zarr)
## devtools::load_all("/home/gkraemer/progs/R/zarr")

context("Zarr")

test_that("Zarr", {

  path <- "test_array"

### DataSet
  data_set <- zarr::create_dataset(path, c(9, 9, 9), c(3, 3, 3))
  expect_s3_class(data_set, "zarr_dataset")
  expect_true(dir.exists(path))

  data_set_2 <- zarr::open_zarr(path)
  expect_s3_class(data_set_2, "zarr_dataset")

### Properties
  ## zarr::read_attributes(path)

### write
  val1 <- 5
  data_set[1, 1, 1] <- val1

  val2 <- array(c(1, 2, 3, 4, 5, 6, 7, 8), c(2, 2, 2))
  data_set[2:3, 2:3, 2:3] <- val2

### read
  expect_equal(data_set[1, 1, 1, drop = TRUE], val1)
  expect_equal(data_set[1, 1, 1], array(val1, c(1, 1, 1)))
  expect_equal(data_set[2:3, 2:3, 2:3], val2)

### cleanup
  unlink(path, recursive = TRUE)
  expect_false(dir.exists(path))

})

context("Helpers")

test_that("Helpers", {
  expect_equal(zarr:::range_to_offset_shape(1:3, 1:3, 1:3),
               list(offset = c(1, 1, 1), shape = c(3, 3, 3)))
  expect_equal(zarr:::range_to_offset_shape(2:4, 2:4, 2:4),
               list(offset = c(2, 2, 2), shape = c(3, 3, 3)))
  expect_equal(zarr:::range_to_offset_shape(1, 2:5, 1),
               list(offset = c(1, 2, 1), shape = c(1, 4, 1)))
})
