

test_that("metadata", {
  md <- zarr:::create_dataset_metadata(c(9, 9), c(3, 3))
  expect_s3_class(md, "dataset_metadata_handle")
})

test_that("metadata list conversions", {
  md <- zarr:::create_dataset_metadata(c(9, 9), c(3, 3))
  ml <- as.list(md)
  mld <- as.dataset_metadata_handle(ml)
  mldl <- as.list(mld)
  expect_s3_class(md, "dataset_metadata_handle")
  expect_s3_class(ml, "list")
  expect_s3_class(mld, "dataset_metadata_handle")
  expect_s3_class(mldl, "list")
  expect_equal(ml, mldl)
})
