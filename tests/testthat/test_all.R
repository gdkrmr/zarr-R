## library(testthat)
## library(zarr)
## devtools::load_all("/home/gkraemer/progs/R/zarr")
## devtools::load_all("/home/gkraemer/progs/R/zarr", recompile = TRUE)


path <- tempfile()
shape <- c(9, 9, 9)
chunk_shape <- c(3, 3, 3)
fill_value <- 0

data_set <- create_dataset(
  path, shape, chunk_shape, fill_value = fill_value)

context("create_dataset")

test_that("zarr_dataset created a directory on disk",
          expect_true(dir.exists(path)))
test_that("zarr_dataset is the correct s3 class",
          expect_s3_class(data_set, "zarr_dataset"))
test_that("the path from zarr_dataset object is correct",
          expect_equal(get_path(data_set), path))
test_that("zarr_dataset has the correct dimensions",
          expect_equal(dim(data_set), shape))

context("open_zarr")
data_set_2 <- zarr::open_zarr(path)

test_that("zarr_dataset is the correct s3 class",
          expect_s3_class(data_set_2, "zarr_dataset"))
test_that("the path from zarr_dataset object is correct",
          expect_equal(get_path(data_set_2), path))
test_that("zarr_dataset has the correct dimensions",
          expect_equal(dim(data_set_2), shape))


context("Attributes")
data_attr <- zarr::read_attributes(paste0(path, "/.zarray"))

  ## TODO:
  ## data_attr2 <- read_attributes(data_set)
  ## expect_equal(data_attr, data_attr2)

  ## NOTE: as json arrays can have anything as element type, we should do the
  ## same here, therefore we use list(...) instead of c(...) here!

test_that("attribute shape is correct",
          expect_equal(data_attr$shape, as.list(shape)))
test_that("attribute chunking is correct",
          expect_equal(data_attr$chunks, as.list(chunk_shape)))
test_that("the fill value is correct",
          expect_equal(data_set[1, 1, 1, drop = TRUE], fill_value))

context("Bounds checking")

## NOTE: this causes a memory leak if not properly checked!!
test_that("`[` lower bounds are checked",
          expect_error(data_set[0, 0, 0]))
test_that("`[`upper bounds are checked",
          expect_error(data_set[10, 10, 10]))
test_that("`[<-` lower bounds are checked",
          expect_error(data_set[1, 1, 0] <- 5))
test_that("`[<-` upper bounds are checked",
          expect_error(data_set[0, 10, 10] <- 5))
test_that("`[` with vectors fails",
          expect_error(data_set[c(1, 3), ,]))
test_that("`[` with the wrong number of parameters fails",
          expect_error(data_set[,]))
test_that("`[` lower bounds are checked", {
  i <- 0
  expect_error(data_set[i, 0, 0])
  expect_error(data_set[0, i, 0])
  expect_error(data_set[0, 0, i])
  expect_error(data_set[i, i, i])
})

context("reading and writing")

test_that("writing an array length 1", {
  val <- rnorm(1)
  dim(val) <- c(1, 1, 1)
  i <- 1
  data_set[1, 1, 1] <- val
  expect_equal(data_set[1, 1, 1], val)
  expect_equal(data_set[i, 1, 1], val)
  expect_equal(data_set[1, i, 1], val)
  expect_equal(data_set[1, 1, i], val)
  expect_equal(data_set[i, i, i], val)
})
test_that("writing an scalar", {
  val <- rnorm(1)
  i <- 1
  data_set[1, 1, 1] <- val
  expect_equal(data_set[1, 1, 1, drop = TRUE], val)
  expect_equal(data_set[1, 1, 1, drop = TRUE], val)
  expect_equal(data_set[i, 1, 1, drop = TRUE], val)
  expect_equal(data_set[1, i, 1, drop = TRUE], val)
  expect_equal(data_set[1, 1, i, drop = TRUE], val)
  expect_equal(data_set[i, i, i, drop = TRUE], val)
})
test_that("writing a bigger array", {
  val <- rnorm(2^3)
  i <- 2:3
  j <- 2:3
  k <- 1:2
  i2 <- 1
  j2 <- 2
  k2 <- i2:j2
  dim(val) <- c(2, 2, 2)
  data_set[2:3, 2:3, 1:2] <- val
  expect_equal(data_set[2:3, 2:3, 1:2], val)
  expect_equal(data_set[2:3, 2:3, 1:2, drop = TRUE], val)
  expect_equal(data_set[i, 2:3, 1:2], val)
  expect_equal(data_set[2:3, j, 1:2], val)
  expect_equal(data_set[2:3, 2:3, k], val)
  expect_equal(data_set[2:3, 2:3, k2], val)
  expect_equal(data_set[i, 2:3, 1:2, drop = TRUE], val)
  expect_equal(data_set[2:3, j, 1:2, drop = TRUE], val)
  expect_equal(data_set[2:3, 2:3, k, drop = TRUE], val)
  expect_equal(data_set[2:3, 2:3, k2, drop = TRUE], val)
})

test_that("writing and reading of mixed indices", {
  val <- rnorm(9)
  dim(val) <- c(1, 1, 9)
  i <- 9
  j <- 9
  k <- 1:9
  i2 <- 1
  j2 <- 9
  k2 <- i2:j2
  data_set[9, 9, 1:9] <- val
  expect_equal(data_set[9, 9, 1:9], val)
  expect_equal(data_set[9, 9, 1:9, drop = TRUE], zarr:::drop_dim(val))
  expect_equal(data_set[i, 9, 1:9], val)
  expect_equal(data_set[9, j, 1:9], val)
  expect_equal(data_set[9, 9, k],  val)
  expect_equal(data_set[9, 9, k2], val)
  expect_equal(data_set[i, 9, 1:9, drop = TRUE], zarr:::drop_dim(val))
  expect_equal(data_set[9, j, 1:9, drop = TRUE], zarr:::drop_dim(val))
  expect_equal(data_set[9, 9, k,   drop = TRUE], zarr:::drop_dim(val))
  expect_equal(data_set[9, 9, k2,  drop = TRUE], zarr:::drop_dim(val))
})

## TODO:
## data_set[1, 1, ] <- val1
test_that("writing and reading empty indices", {
  val <- rnorm(9)
  i <- 9
  j <- 9
  data_set[9, 9, ] <- val
  expect_equal(data_set[9, 9, ], array(val, c(1, 1, 9)))
  expect_equal(data_set[9, 9, , drop = TRUE], val)
  expect_equal(data_set[i, 9, , drop = TRUE], val)
  expect_equal(data_set[9, j, , drop = TRUE], val)
  data_set[9, , 9] <- val
  expect_equal(data_set[9, , 9], array(val, c(1, 9, 1)))
  expect_equal(data_set[9, , 9, drop = TRUE], val)
  expect_equal(data_set[i, , 9, drop = TRUE], val)
  expect_equal(data_set[9, , j, drop = TRUE], val)
  data_set[, 9, 9] <- val
  expect_equal(data_set[, 9, 9], array(val, c(9, 1, 1)))
  expect_equal(data_set[, 9, 9, drop = TRUE], zarr:::drop_dim(val))
  expect_equal(data_set[, i, 9, drop = TRUE], val)
  expect_equal(data_set[, 9, j, drop = TRUE], val)
})

test_that("writing and reading empty indices value dims", {
  val <- rnorm(9)
  i <- 9
  j <- 9
  data_set[9, 9, ] <- array(val, c(1, 1, 9))
  expect_equal(data_set[9, 9, ], array(val, c(1, 1, 9)))
  expect_equal(data_set[9, 9, , drop = TRUE], val)
  expect_equal(data_set[i, 9, , drop = TRUE], val)
  expect_equal(data_set[9, j, , drop = TRUE], val)
  data_set[9, , 9] <- array(val, c(1, 9, 1))
  expect_equal(data_set[9, , 9], array(val, c(1, 9, 1)))
  expect_equal(data_set[9, , 9, drop = TRUE], val)
  expect_equal(data_set[i, , 9, drop = TRUE], val)
  expect_equal(data_set[9, , j, drop = TRUE], val)
  data_set[, 9, 9] <- array(val, c(9, 1, 1))
  expect_equal(data_set[, 9, 9], array(val, c(9, 1, 1)))
  expect_equal(data_set[, 9, 9, drop = TRUE], val)
  expect_equal(data_set[, i, 9, drop = TRUE], val)
  expect_equal(data_set[, 9, j, drop = TRUE], val)
})

test_that("writing and reading empty indices matrix", {
  val <- matrix(rnorm(81), nrow = 9, ncol = 9)
  i <- 9
  data_set[9, , ] <- val
  expect_equal(data_set[9, , ], array(val, c(1, 9, 9)))
  expect_equal(data_set[9, , , drop = TRUE], val)
  expect_equal(data_set[i, , , drop = TRUE], val)
  data_set[, 9, ] <- val
  expect_equal(data_set[, 9, ], array(val, c(9, 1, 9)))
  expect_equal(data_set[, 9, , drop = TRUE], val)
  expect_equal(data_set[, i, , drop = TRUE], val)
  data_set[, , 9] <- val
  expect_equal(data_set[, , 9], array(val, c(9, 9, 1)))
  expect_equal(data_set[, , 9, drop = TRUE], val)
  expect_equal(data_set[, , i, drop = TRUE], val)
})

test_that("cleanup", {
  ## TODO:
  ## unlink(data_set)
  unlink(path, recursive = TRUE)
  expect_false(dir.exists(path))
})

context("Helpers")

test_that("range_to_offset_shape", {
  expect_equal(zarr:::range_to_offset_shape(1:3, 1:3, 1:3),
               list(offset = c(0, 0, 0), shape = c(3, 3, 3)))
  expect_equal(zarr:::range_to_offset_shape(2:4, 2:4, 2:4),
               list(offset = c(1, 1, 1), shape = c(3, 3, 3)))
  expect_equal(zarr:::range_to_offset_shape(1, 2:5, 1),
               list(offset = c(0, 1, 0), shape = c(1, 4, 1)))
})

test_that("match_shape", {
  expect_equal(zarr:::match_shape(c(9, 9, 9), c(9, 9, 9)), c(9, 9, 9))
  expect_equal(zarr:::match_shape(9, c(1, 1, 9)), c(1, 1, 9))
  expect_equal(zarr:::match_shape(9, c(1, 9, 1)), c(1, 9, 1))
  expect_equal(zarr:::match_shape(9, c(9, 1, 1)), c(9, 1, 1))
  expect_equal(zarr:::match_shape(1, c(1, 1, 1)), c(1, 1, 1))
  expect_equal(zarr:::match_shape(c(2, 3), c(1, 2, 3)), c(1, 2, 3))
  expect_equal(zarr:::match_shape(c(2, 3), c(2, 3, 1)), c(2, 3, 1))
  expect_error(zarr:::match_shape(9, c(9, 9)))
})
