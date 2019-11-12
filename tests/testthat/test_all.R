## library(testthat)
## library(zarr)
## devtools::load_all("/home/gkraemer/progs/R/zarr")

context("Zarr")

test_that("Zarr", {

  path <- "test_array"

### File
  file <- zarr::create_file(path, "a")
  expect_s3_class(file, "zarr_file")
  expect_true(dir.exists(path))

### DataSet
  unlink("test_array3", recursive = TRUE)
  data_set <- zarr::create_dataset("test_array3", c(9, 9, 9), c(3, 3, 3))
  data_set <- zarr::open_zarr("test_array3", "a")

### Properties
  zarr::read_attributes(path)


### write
  val1 <- 5
  data_set[1, 1, 1] <- val1

  val2 <- array(1:8, c(2, 2, 2))
  data_set[2:3, 2:3, 2:3] <- val2

### read
  expect_equal(data_set[1, 1, 1], val1)
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
