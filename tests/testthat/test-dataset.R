context("dataset")

test_that("dataset", {

  path <- tempfile()

  f <- zarr:::get_file_handle(path)
  g1 <- zarr:::get_group_handle(f, "group1")
  g2 <- zarr:::get_group_handle(g1, "group2")

  zarr:::create_file(f)
  zarr:::create_group(g1)
  zarr:::create_group(g2)

  shape0 <- c(16, 16, 16)
  chunk_shape0 <- c(4, 4, 4)
  shape1 <- c(9, 9, 9)
  chunk_shape1 <- c(3, 3, 3)
  shape2 <- c(4, 4, 4)
  chunk_shape2 <- c(2, 2, 2)

  d0 <- zarr::create_dataset(f,  "dataset0", shape0, chunk_shape0)
  d1 <- zarr::create_dataset(g1, "dataset1", shape1, chunk_shape1)
  d2 <- zarr::create_dataset(g2, "dataset2", shape2, chunk_shape2)

  dd0 <- zarr::open_dataset(f, "dataset0")
  dd1 <- zarr::open_dataset(g1, "dataset1")
  dd2 <- zarr::open_dataset(g2, "dataset2")

  ddd1 <- zarr::open_dataset(f, "group1/dataset1")
  ddd2 <- zarr::open_dataset(f, "group1/group2/dataset2")

  dddd2 <- zarr::open_dataset(g1, "group2/dataset2")

  expect_equal(get_dataset_metadata(d1), get_dataset_metadata(dd1))
  expect_equal(get_dataset_metadata(d1), get_dataset_metadata(ddd1))

  expect_equal(get_dataset_metadata(d2), get_dataset_metadata(dd2))
  expect_equal(get_dataset_metadata(d2), get_dataset_metadata(ddd2))
  expect_equal(get_dataset_metadata(d2), get_dataset_metadata(dddd2))

  d_list <- list(d0, d1, d2, dd0, dd1, dd2, ddd1, ddd2, dddd2)

  lapply(d_list, function(x) expect_true(DatasetIsZarr(x)))
  lapply(d_list, function(x) expect_equal(DatasetGetDtype(x), "float64"))

  expect_equal(DatasetDimension(d0), length(shape0))
  expect_equal(DatasetDimension(dd0), length(shape0))
  expect_equal(DatasetDimension(d1), length(shape1))
  expect_equal(DatasetDimension(dd1), length(shape1))
  expect_equal(DatasetDimension(ddd1), length(shape1))
  expect_equal(DatasetDimension(d2), length(shape2))
  expect_equal(DatasetDimension(dd2), length(shape2))
  expect_equal(DatasetDimension(ddd2), length(shape2))
  expect_equal(DatasetDimension(dddd2), length(shape2))

  expect_equal(DatasetShape(d0), shape0)
  expect_equal(DatasetShape(dd0), shape0)
  expect_equal(DatasetShape(d1), shape1)
  expect_equal(DatasetShape(dd1), shape1)
  expect_equal(DatasetShape(ddd1), shape1)
  expect_equal(DatasetShape(d2), shape2)
  expect_equal(DatasetShape(dd2), shape2)
  expect_equal(DatasetShape(ddd2), shape2)
  expect_equal(DatasetShape(dddd2), shape2)

  expect_equal(DatasetSize(d0), prod(shape0))
  expect_equal(DatasetSize(dd0), prod(shape0))
  expect_equal(DatasetSize(d1), prod(shape1))
  expect_equal(DatasetSize(dd1), prod(shape1))
  expect_equal(DatasetSize(ddd1), prod(shape1))
  expect_equal(DatasetSize(d2), prod(shape2))
  expect_equal(DatasetSize(dd2), prod(shape2))
  expect_equal(DatasetSize(ddd2), prod(shape2))
  expect_equal(DatasetSize(dddd2), prod(shape2))

  expect_equal(DatasetPath(d0), paste0(path, "/dataset0"))
  expect_equal(DatasetPath(dd0), paste0(path, "/dataset0"))
  expect_equal(DatasetPath(d1), paste0(path, "/group1/dataset1"))
  expect_equal(DatasetPath(dd1), paste0(path, "/group1/dataset1"))
  expect_equal(DatasetPath(ddd1), paste0(path, "/group1/dataset1"))
  expect_equal(DatasetPath(d2), paste0(path, "/group1/group2/dataset2"))
  expect_equal(DatasetPath(dd2), paste0(path, "/group1/group2/dataset2"))
  expect_equal(DatasetPath(ddd2), paste0(path, "/group1/group2/dataset2"))
  expect_equal(DatasetPath(dddd2), paste0(path, "/group1/group2/dataset2"))


  lapply(d_list, function(x) expect_true(is.na(DatasetGetFillValue(x))))
  lapply(d_list, function(x) expect_equal(DatasetGetCompressor(x), "raw"))
  lapply(d_list, function(x) expect_equal(DatasetGetFileMode(x), "a"))

  expect_equal(DatasetGetDefaultChunkShape(d0), chunk_shape0)
  expect_equal(DatasetGetDefaultChunkShape(dd0), chunk_shape0)
  expect_equal(DatasetGetDefaultChunkShape(d1), chunk_shape1)
  expect_equal(DatasetGetDefaultChunkShape(dd1), chunk_shape1)
  expect_equal(DatasetGetDefaultChunkShape(ddd1), chunk_shape1)
  expect_equal(DatasetGetDefaultChunkShape(d2), chunk_shape2)
  expect_equal(DatasetGetDefaultChunkShape(dd2), chunk_shape2)
  expect_equal(DatasetGetDefaultChunkShape(ddd2), chunk_shape2)
  expect_equal(DatasetGetDefaultChunkShape(dddd2), chunk_shape2)

  expect_equal(DatasetGetDefaultChunkSize(d0), prod(chunk_shape0))
  expect_equal(DatasetGetDefaultChunkSize(dd0), prod(chunk_shape0))
  expect_equal(DatasetGetDefaultChunkSize(d1), prod(chunk_shape1))
  expect_equal(DatasetGetDefaultChunkSize(dd1), prod(chunk_shape1))
  expect_equal(DatasetGetDefaultChunkSize(ddd1), prod(chunk_shape1))
  expect_equal(DatasetGetDefaultChunkSize(d2), prod(chunk_shape2))
  expect_equal(DatasetGetDefaultChunkSize(dd2), prod(chunk_shape2))
  expect_equal(DatasetGetDefaultChunkSize(ddd2), prod(chunk_shape2))
  expect_equal(DatasetGetDefaultChunkSize(dddd2), prod(chunk_shape2))

  DatasetRemove(d0)
  DatasetRemove(d1)
  DatasetRemove(d2)

  expect_error(DatasetRemove(dd0))
  expect_error(DatasetRemove(dd1))
  expect_error(DatasetRemove(dd2))
  expect_error(DatasetRemove(ddd1))
  expect_error(DatasetRemove(ddd2))
  expect_error(DatasetRemove(dddd2))
})
