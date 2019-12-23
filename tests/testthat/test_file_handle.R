testthat("file handle", {

  path <- tempfile()

  f <- zarr:::get_file_handle.character(path)
  expect_s3_class(f, "file_handle")
  expect_false(dir.exists(path))

  expect_false(zarr:::FileHandleIsS3(f))
  expect_false(zarr:::FileHandleIsGcs(f))
  expect_failure(
    expect_false(zarr:::FileHandleIsZarr(f))
  )
  expect_false(zarr:::FileHandleExists(f))
  expect_equal(zarr:::FileHandlePath(f), path)
  expect_error(zarr:::FileHandleKeys(f), c())
  expect_equal(zarr:::FileHandleNameInBucket(f), "")
  expect_equal(zarr:::FileHandleBucket(f), "")

  zarr:::create_file.file_handle(f)
  expect_true(zarr:::FileHandleIsZarr(f))
  expect_true(dir.exists(path))
  expect_equal(zarr:::FileHandleKeys(f), character(0))


  zarr:::FileHandleDelete(f)
  expect_false(dir.exists(path))

  expect_error(zarr:::get_file_handle.character(1))

  ### The problem here is that s3 classes need to be exported and I want to keep
  ### this part of the internals for now and only expose a minimal interface. Therefore there is no "free" type checking here.
  expect_failure(
    expect_error(zarr:::get_file_handle.file_handle(1))
  )


})
