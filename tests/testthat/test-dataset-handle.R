context("dataset handle")

test_that("dataset handle", {

  path <- tempfile()

  f <- zarr:::get_file_handle(path)
  g <- zarr:::get_group_handle(f, "group1")

  d1 <- zarr:::get_dataset_handle(f, "dataset1")
  expect_false(dir.exists(paste0(path, "/dataset1")))
  expect_false(zarr:::DatasetHandleExists(d1))
  d2 <- zarr:::get_dataset_handle(g, "dataset2")
  expect_false(dir.exists(paste0(path, "/group1/dataset2")))
  expect_false(zarr:::DatasetHandleExists(d2))

  zarr:::create_group(g)
  zarr:::create_dataset(d1, shape = c(9, 9, 9), chunk_shape = c(3, 3, 3))
  expect_true(dir.exists(paste0(path, "/dataset1")))
  expect_true(zarr:::DatasetHandleExists(d1))

  zarr:::create_dataset(d2, shape = c(9, 9, 9), chunk_shape = c(3, 3, 3))
  expect_true(dir.exists(paste0(path, "/group1/dataset2")))
  expect_true(zarr:::DatasetHandleExists(d2))

  expect_false(zarr:::DatasetHandleIsS3(d1))
  expect_false(zarr:::DatasetHandleIsS3(d2))
  expect_false(zarr:::DatasetHandleIsGcs(d1))
  expect_false(zarr:::DatasetHandleIsGcs(d2))
  expect_true(zarr:::DatasetHandleIsZarr(d1))
  expect_true(zarr:::DatasetHandleIsZarr(d2))

  expect_equal(zarr:::DatasetHandlePath(d1), paste0(path, "/dataset1"))
  expect_equal(zarr:::DatasetHandlePath(d2), paste0(path, "/group1/dataset2"))

  expect_equal(zarr:::DatasetHandleBucket(d1), "")
  expect_equal(zarr:::DatasetHandleBucket(d2), "")

  expect_equal(zarr:::DatasetHandleNameInBucket(d1), "")
  expect_equal(zarr:::DatasetHandleNameInBucket(d2), "")

  zarr:::DatasetHandleDelete(d1)

  expect_false(zarr:::DatasetHandleExists(d1))
  expect_false(dir.exists(paste0(path, "/dataset1")))

  zarr:::DatasetHandleDelete(d2)
  expect_false(zarr:::DatasetHandleExists(d2))
  expect_false(dir.exists(paste0(path, "/group1/dataset2")))

  zarr:::FileHandleDelete(f)
  expect_false(zarr:::FileHandleExists(f))
  expect_false(dir.exists(path))

  expect_error(zarr:::get_dataset_handle(1))
  expect_error(zarr:::get_dataset_handle(1))
  expect_error(zarr:::create_group(1))
})
