library(testthat)
## library(zarr)
devtools::load_all("/home/gkraemer/progs/R/zarr")

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

  data_set_2 <- zarr::open_zarr(path)
  expect_s3_class(data_set_2, "zarr_dataset")

### Properties
  data_attr <- zarr::read_attributes(paste0(path, "/.zarray"))

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

### Fill value
  expect_equal(data_set[1, 1, 1, drop = TRUE], fill_value)

### write
  val1 <- 5
  data_set[1, 1, 1] <- array(val1, c(1, 1, 1))
  data_set[1, 1, 1] <- val1

  data_set[1, 1, ] <- val1

  data_set[1, 3, ]
  data_set[1, 3, , drop = TRUE]


  list(1, 2, NULL = NULL)
  c(1, 2, NULL)
  str(list(1, 2, ))
  `[`
  `[.data.frame`
  `[.`
  x[]
  x <- array(1:9, c(3, 3, 3))
  class(x) <- "test"

  `[.test` <- function(x, ...) {
    list(...)
    x
  }

  x[1,,]

  f <- function (a, b = 2, c = 3,...) {
    list(a = a, b = b, c = c)
    list(...)
  }

  f(1, ,, ,)

  g <- function (...) {
    as.list(match.call())
  }
  h <- function(){1}
  
  args(g)
  args(h)

  g(,,)[2]
  c(,,)

  val2 <- array(c(1, 2, 3, 4, 5, 6, 7, 8), c(2, 2, 2))
  data_set[2:3, 2:3, 2:3] <- val2

  val3 <- array(c(1, 2, 3), c(1, 1, 3))
  data_set[9, 9, 7:9] <- val3

### read
  expect_equal(data_set[1, 1, 1, drop = TRUE], val1)
  expect_equal(data_set[1, 1, 1],              array(val1, c(1, 1, 1)))
  expect_equal(data_set[2:3, 2:3, 2:3],        val2)
  expect_equal(data_set[9, 9, 7:9],            val3)

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
