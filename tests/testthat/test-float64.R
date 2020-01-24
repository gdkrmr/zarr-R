context("Float64")


test_that("single access", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- 0
  data_type <- c("float64")
  test_data <- array(runif(prod(shape)) * 100, dim = shape)


  f <- zarr:::get_file_handle(path)
  d <- zarr:::get_dataset_handle(f, "dataset1")
  dh <- zarr::create_dataset(d, shape = shape, chunk_shape = chunk_shape,
                             fill_value = fill_value, data_type = data_type)

  expect_equal(zarr:::readSubarray(dh, c(1, 1), c(1, 1)),
               array(fill_value, dim = rep(1, length(shape))))
  expect_equal(dim(get_dataset_metadata(dh)), shape)
  expect_equal(dim(dh), shape)

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      expect_identical(dh[i, j, drop = TRUE], fill_value)
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      dh[i, j] <- test_data[i, j]
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      expect_identical(dh[i, j, drop = TRUE], test_data[i, j])
    }
  }

  zarr:::DatasetRemove(dh)
})

test_that("row access", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- 0
  data_type <- c("float64")
  test_data <- array(runif(prod(shape)) * 100, dim = shape)


  f <- zarr:::get_file_handle(path)
  d <- zarr:::get_dataset_handle(f, "dataset1")
  dh <- zarr::create_dataset(d, shape = shape, chunk_shape = chunk_shape, fill_value = fill_value, data_type = data_type)

  expect_equal(zarr:::readSubarray(dh, c(1, 1), c(1, 1)), array(fill_value, dim = rep(1, length(shape))))
  expect_equal(dim(get_dataset_metadata(dh)), shape)
  expect_equal(dim(dh), shape)

  for (j in 1:shape[2]) {
    expect_identical(dh[, j, drop = TRUE], rep(fill_value, shape[2]))
    expect_identical(dh[, j], array(fill_value, dim = c(shape[2], 1)))
  }

  for (j in 1:shape[2]) {
    dh[, j] <- test_data[, j]
  }

  for (j in 1:shape[2]) {
    expect_identical(dh[, j, drop = TRUE], test_data[, j])
    expect_identical(dh[, j], test_data[, j, drop = FALSE])
  }

  zarr:::DatasetRemove(dh)
})

test_that("column access", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- 0
  data_type <- c("float64")
  test_data <- array(runif(prod(shape)) * 100, dim = shape)


  f <- zarr:::get_file_handle(path)
  d <- zarr:::get_dataset_handle(f, "dataset1")
  dh <- zarr::create_dataset(d, shape = shape, chunk_shape = chunk_shape,
                             fill_value = fill_value, data_type = data_type)

  expect_equal(zarr:::readSubarray(dh, c(1, 1), c(1, 1)),
               array(fill_value, dim = rep(1, length(shape))))
  expect_equal(dim(get_dataset_metadata(dh)), shape)
  expect_equal(dim(dh), shape)

  for (j in 1:shape[2]) {
    expect_identical(dh[j, , drop = TRUE], rep(fill_value, shape[2]))
    expect_identical(dh[j, ], array(fill_value, dim = c(1, shape[2])))
  }

  for (j in 1:shape[2]) {
    dh[j, ] <- test_data[j, ]
  }

  for (j in 1:shape[2]) {
    expect_identical(dh[j, , drop = TRUE], test_data[j, ])
    expect_identical(dh[j, ], test_data[j, , drop = FALSE])
  }

  zarr:::DatasetRemove(dh)
})
