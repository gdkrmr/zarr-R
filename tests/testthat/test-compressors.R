context("compressors")

test_that("autocompletion of options", {
  expect_equal(zarr:::complete_compression_options("blosc", list()), zarr:::DEFAULT_COMPRESSOR_OPTIONS[["blosc"]])
  expect_equal(zarr:::complete_compression_options("zlib", list()), zarr:::DEFAULT_COMPRESSOR_OPTIONS[["zlib"]])
  expect_equal(zarr:::complete_compression_options("bzip2", list()), zarr:::DEFAULT_COMPRESSOR_OPTIONS[["bzip2"]])
  expect_mapequal(zarr:::complete_compression_options("blosc", list(clevel = 4)), list(cname = "lz4", clevel = 4, shuffle = 1))
  expect_equal(zarr:::complete_compression_options("raw", list()), list())
})

test_that("blosc compressor works", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- 0
  data_type <- c("float64")
  test_data <- array(runif(prod(shape)) * 100, dim = shape)

  f <- zarr:::get_file_handle(path)
  g <- zarr:::get_group_handle(f, "group1")
  d <- zarr:::get_dataset_handle(f, "dataset1")

  dh1 <- zarr::create_dataset(d, shape = shape, chunk_shape = chunk_shape, compressor = "blosc")
  dh2 <- zarr::create_dataset(f, key = "dataset2", shape = shape, chunk_shape = chunk_shape,
                              compressor = "blosc", compression_options = list(cname = "blosclz"))
  dh3 <- zarr::create_dataset(g, key = "dataset3", shape = shape, chunk_shape = chunk_shape,
                              compressor = "blosc", compression_options = list(cname = "lz4"))
  dh4 <- zarr::create_dataset(g, key = "dataset4", shape = shape, chunk_shape = chunk_shape,
                              compressor = "blosc", compression_options = list(cname = "lz4hc"))
  dh5 <- zarr::create_dataset(g, key = "dataset5", shape = shape, chunk_shape = chunk_shape,
                              compressor = "blosc", compression_options = list(cname = "snappy"))
  dh6 <- zarr::create_dataset(g, key = "dataset6", shape = shape, chunk_shape = chunk_shape,
                              compressor = "blosc", compression_options = list(cname = "zlib"))
  dh7 <- zarr::create_dataset(g, key = "dataset7", shape = shape, chunk_shape = chunk_shape,
                              compressor = "blosc", compression_options = list(cname = "zstd"))

  expect_invisible(dh1[1, 1] <- 1)
  expect_invisible(dh2[1, 1] <- 1)
  expect_invisible(dh3[1, 1] <- 1)
  expect_invisible(dh4[1, 1] <- 1)
  expect_invisible(dh5[1, 1] <- 1)
  expect_invisible(dh6[1, 1] <- 1)
  ## TODO: find a way to use expect_failure here!
  ## expect_silent(dh7[1, 1] <- 1)

  expect_equal(dh1[1, 1, drop = TRUE], 1)
  expect_equal(dh2[1, 1, drop = TRUE], 1)
  expect_equal(dh3[1, 1, drop = TRUE], 1)
  expect_equal(dh4[1, 1, drop = TRUE], 1)
  expect_equal(dh5[1, 1, drop = TRUE], 1)
  expect_equal(dh6[1, 1, drop = TRUE], 1)
  expect_failure(expect_equal(dh7[1, 1, drop = TRUE], 1))

  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh1))$compressor$cname, zarr:::DEFAULT_COMPRESSOR_OPTIONS$blosc$cname)
  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh2))$compressor$cname, "blosclz")
  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh3))$compressor$cname, "lz4")
  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh4))$compressor$cname, "lz4hc")
  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh5))$compressor$cname, "snappy")
  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh6))$compressor$cname, "zlib")
  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh7))$compressor$cname, "zstd")

})


test_that("zlib compressor works", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- 0
  data_type <- c("float64")
  test_data <- array(runif(prod(shape)) * 100, dim = shape)

  f <- zarr:::get_file_handle(path)
  g <- zarr:::get_group_handle(f, "group1")
  d1 <- zarr:::get_dataset_handle(f, "dataset1")
  dh1 <- zarr::create_dataset(d1, shape = shape, chunk_shape = chunk_shape, compressor = "zlib")

  dh2 <- zarr::create_dataset(g, "dataset2", shape = shape, chunk_shape = chunk_shape, compressor = "zlib")

  expect_invisible(dh1[1, 1] <- 1)
  expect_invisible(dh2[1, 1] <- 1)

  expect_equal(dh1[1, 1, drop = TRUE], 1)
  expect_equal(dh2[1, 1, drop = TRUE], 1)

  expect_equal(zarr:::MetadataToList(zarr:::get_dataset_metadata(dh1))$compressor$cname, zarr:::DEFAULT_COMPRESSOR_OPTIONS$blosc$cname)

})
