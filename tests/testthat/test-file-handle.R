context("file handle")

test_that("file handle", {

  path <- tempfile()

  f <- zarr:::get_file_handle(path)
  expect_s3_class(f, "file_handle")
  expect_false(dir.exists(path))

  expect_false(zarr:::FileHandleIsS3(f))
  expect_false(zarr:::FileHandleIsGcs(f))
  ## TODO: until when does this fail?
  expect_error(expect_false(zarr:::FileHandleIsZarr(f)))
  expect_false(zarr:::FileHandleExists(f))
  expect_equal(zarr:::FileHandlePath(f), path)
  expect_error(zarr:::FileHandleKeys(f), c())
  expect_equal(zarr:::FileHandleNameInBucket(f), "")
  expect_equal(zarr:::FileHandleBucket(f), "")

  zarr:::create_file(f)
  ## TODO: until when does this fail?
  expect_error(expect_true(zarr:::FileHandleIsZarr(f)))
  expect_true(dir.exists(path))
  expect_equal(zarr:::FileHandleKeys(f), character(0))


  zarr:::FileHandleDelete(f)
  expect_false(dir.exists(path))

  expect_error(zarr:::get_file_handle(1))
  expect_error(zarr:::get_file_handle(1))
  expect_error(create_file(1))


})
