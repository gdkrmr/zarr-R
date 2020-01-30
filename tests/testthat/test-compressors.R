context("compressors")

test_that("autocompletion of options", {
  expect_equal(zarr:::complete_compression_options("blosc", list()), zarr:::DEFAULT_COMPRESSOR_OPTIONS[["blosc"]])
  expect_equal(zarr:::complete_compression_options("zlib", list()), zarr:::DEFAULT_COMPRESSOR_OPTIONS[["zlib"]])
  expect_equal(zarr:::complete_compression_options("bzip2", list()), zarr:::DEFAULT_COMPRESSOR_OPTIONS[["bzip2"]])
  expect_mapequal(zarr:::complete_compression_options("blosc", list(clevel = 4)), list(cname = "lzf", clevel = 4, shuffle = 1))
  expect_equal(zarr:::complete_compression_options("raw", list()), list())
})

test_that("compressors work", {

  path <- tempfile()
  shape <- c(9, 9)
  chunk_shape <- c(3, 3)
  fill_value <- 0
  data_type <- c("float64")
  test_data <- array(runif(prod(shape)) * 100, dim = shape)

  f <- zarr:::get_file_handle(path)
  g <- zarr:::get_group_handle(f, "group1")
  d <- zarr:::get_dataset_handle(f, "dataset3")

  dh1 <- zarr::create_dataset(d, shape = shape, chunk_shape = chunk_shape, compressor = "blosc")
  dh2 <- zarr::create_dataset(f, key = "dataset5", shape = shape, chunk_shape = chunk_shape, compressor = "blosc")
  dh3 <- zarr::create_dataset(g, key = "dataset2", shape = shape, chunk_shape = chunk_shape, compressor = "blosc")

  dh1[1, 1] <- 1
  dh2[1, 1] <- 1
  dh3[1, 1] <- 1

  dh1[]

  zarr:::MetadataToList(zarr:::getDatasetMetadataDataset(dh1))

})
