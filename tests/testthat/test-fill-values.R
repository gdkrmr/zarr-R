context("fill values")


test_that("NA_integer_", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- NA_integer_
  data_type <- c("int32")
  test_data <- array(as.integer(runif(prod(shape)) * 100), dim = shape)


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
      dh[i, j] <- fill_value
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      expect_identical(dh[i, j, drop = TRUE], fill_value)
    }
  }

  zarr:::DatasetRemove(dh)
})

test_that("NA_real_", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- NA_real_
  data_type <- c("float64")
  test_data <- array(as.integer(runif(prod(shape)) * 100), dim = shape)


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
      expect_equal(dh[i, j, drop = TRUE], fill_value)
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      dh[i, j] <- fill_value
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      expect_equal(dh[i, j, drop = TRUE], fill_value)
    }
  }

  zarr:::DatasetRemove(dh)
})

test_that("Inf", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- Inf
  data_type <- c("float64")
  test_data <- array(as.integer(runif(prod(shape)) * 100), dim = shape)


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
      dh[i, j] <- fill_value
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      expect_identical(dh[i, j, drop = TRUE], fill_value)
    }
  }

  zarr:::DatasetRemove(dh)
})


test_that("-Inf", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- -Inf
  data_type <- c("float64")
  test_data <- array(as.integer(runif(prod(shape)) * 100), dim = shape)


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
      dh[i, j] <- fill_value
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      expect_identical(dh[i, j, drop = TRUE], fill_value)
    }
  }

  zarr:::DatasetRemove(dh)
})


test_that("NaN", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- NaN
  data_type <- c("float64")
  test_data <- array(as.integer(runif(prod(shape)) * 100), dim = shape)


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
      dh[i, j] <- fill_value
    }
  }

  for (i in 1:shape[1]) {
    for (j in 1:shape[2]) {
      expect_identical(dh[i, j, drop = TRUE], fill_value)
    }
  }

  zarr:::DatasetRemove(dh)
})
