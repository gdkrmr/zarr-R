context("metadata")

test_that("metadata", {
  md <- zarr:::create_dataset_metadata(c(9, 9), c(3, 3))
  expect_s3_class(md, "dataset_metadata")
})

test_that("metadata list conversions", {
  md <- zarr:::create_dataset_metadata(c(9, 9), c(3, 3))
  ml <- as.list(md)
  mld <- zarr:::as.dataset_metadata(ml)
  mldl <- as.list(mld)
  expect_s3_class(md, "dataset_metadata")
  expect_equal(class(ml), "list")
  expect_s3_class(mld, "dataset_metadata")
  expect_equal(class(mldl), "list")
  expect_equal(ml, mldl)
})


test_that("metadata from dataset", {

  path <- tempfile()

  f <- zarr:::get_file_handle(path)
  d <- zarr:::get_dataset_handle(f, "dataset")
  dd <- zarr:::create_dataset(d, shape = c(9, 9, 9), chunk_shape = c(3, 3, 3))

  m <- get_dataset_metadata(dd)

  expect_s3_class(m, "dataset_metadata")

  m2 <- zarr:::create_dataset_metadata(c(9, 9, 9), c(3, 3, 3))
  expect_equal(as.list(m), as.list(m2))

  zarr:::DatasetHandleDelete(d)
  zarr:::FileHandleDelete(f)
})
