## library(testthat)
## library(zarr)
## devtools::load_all("/home/gkraemer/progs/R/zarr")
## devtools::load_all("/home/gkraemer/progs/R/zarr", recompile = TRUE)

context("Zarr")

test_that("Zarr", {

  path <- tempfile()
  shape <- c(9, 9, 9)
  chunk_shape <- c(3, 3, 3)
  fill_value  <- 0

### DataSet
  data_set <- zarr::create_dataset(path, shape, chunk_shape, fill_value = fill_value)
  expect_s3_class(data_set, "zarr_dataset")
  expect_true(dir.exists(path))
  expect_equal(get_path(data_set), path)
  expect_equal(dim(data_set), shape)

  data_set_2 <- zarr::open_zarr(path)
  expect_s3_class(data_set_2, "zarr_dataset")

### Properties
  data_attr <- zarr::read_attributes(paste0(path, "/.zarray"))

  ## TODO:
  ## data_attr2 <- read_attributes(data_set)
  ## expect_equal(data_attr, data_attr2)

  ## NOTE: as json arrays can have anything as element type, we should do the
  ## same here, therefore we use list(...) instead of c(...) here!
  expect_equal(data_attr$shape, as.list(shape))
  expect_equal(data_attr$chunks, as.list(chunk_shape))

### Bound checking
  ## this used to cause a memory leak!!
  expect_error(data_set[0, 0, 0])
  expect_error(data_set[10, 10, 10])
  expect_error(data_set[1, 1, 0] <- 5)
  expect_error(data_set[0, 10, 10] <- 5)
  expect_error(data_set[c(1, 3), ,])
  expect_error(data_set[,])

### Fill value
  expect_equal(data_set[1, 1, 1, drop = TRUE], fill_value)

### write
  val1 <- 5
  data_set[1, 1, 1] <- array(val1, c(1, 1, 1))
  data_set[1, 1, 1] <- val1

  val2 <- array(as.numeric(1:4), c(2, 2, 2))
  data_set[2:3, 2:3, 1:2] <- val2

  val3 <- array(as.numeric(1:9), c(1, 1, 9))
  data_set[9, 9, 1:9] <- val3

  ## TODO:
  ## data_set[1, 1, ] <- val1

### read
  i  <- 1
  expect_equal(data_set[i, 1, 1,       drop = TRUE], val1)
  expect_equal(data_set[1, i, 1,       drop = TRUE], val1)
  expect_equal(data_set[1, 1, i,       drop = TRUE], val1)
  expect_equal(data_set[1, 1, 1,       drop = TRUE], val1)
  expect_equal(data_set[1:1, 1:1, 1:1, drop = TRUE], val1)
  expect_equal(data_set[i, 1, 1],       array(val1, c(1, 1, 1)))
  expect_equal(data_set[1, 1, 1],       array(val1, c(1, 1, 1)))
  expect_equal(data_set[1:1, 1:1, 1:1], array(val1, c(1, 1, 1)))
  expect_equal(data_set[2:3, 2:3, 1:2],        val2)
  expect_equal(data_set[9, 9, 1:9],            val3)
  expect_equal(data_set[9, 9,    ],            val3)

### cleanup
  unlink(path, recursive = TRUE)
  expect_false(dir.exists(path))

})

context("Helpers")

test_that("Helpers", {
  expect_equal(zarr:::range_to_offset_shape(1:3, 1:3, 1:3),
               list(offset = c(0, 0, 0), shape = c(3, 3, 3)))
  expect_equal(zarr:::range_to_offset_shape(2:4, 2:4, 2:4),
               list(offset = c(1, 1, 1), shape = c(3, 3, 3)))
  expect_equal(zarr:::range_to_offset_shape(1, 2:5, 1),
               list(offset = c(0, 1, 0), shape = c(1, 4, 1)))
})
