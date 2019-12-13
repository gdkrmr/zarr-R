
testthat("file handle", {

  library(zarr)
  library(testthat)

  path <- tempfile()

  f <- zarr:::get_file_handle.character(path)
  expect_s3_class(f, "file_handle")
  expect_false(dir.exists(path))

  expect_false(zarr:::FileIsS3(f))
  expect_false(zarr:::FileIsGcs(f))
  expect_false(zarr:::FileExists(f))
  expect_equal(zarr:::FilePath(f), path)
  expect_error(zarr:::FileKeys(f), c())

  zarr:::create_file.file_handle(f)
  expect_true(dir.exists(path))
  expect_equal(zarr:::FileKeys(f), character(0))


  zarr:::FileDelete(f)
  expect_false(dir.exists(path))

  expect_error(zarr:::get_file_handle.character(1))
  expect_error(zarr:::get_file_handle.file_handle(1))
})


testthat("group handle", {
  path <- tempfile()
  f <- zarr:::get_file_handle.character(path)

  g1 <- zarr:::get_group_handle.file_handle(f, "group1")
  expect_false(dir.exists(paste0(path, "/group1")))

  zarr:::create_group(g1)
  expect_true(dir.exists(paste0(path, "/group1")))

  g2 <- zarr:::get_group_handle.group_handle(g1, "group2")
  expect_false(dir.exists(paste0(path, "/group1/group2")))

  zarr:::create_group(g2)
  expect_true(dir.exists(paste0(path, "/group1/group2")))

  expect_error(zarr:::get_group_handle.group_handle(1))
  expect_error(zarr:::get_group_handle.file_handle(1))
})


testthat("dataset handle", {
  path <- tempfile()
  f <- zarr:::get_file_handle(path)
  g <- zarr:::get_group_handle(f, "group")
  m <- zarr:::create_dataset_metadata(c(9,9), c(3,3))

  d1 <- zarr::get_dataset_handle(f, "d1")
  expect_false(dir.exists(paste0(path, "/d1")))

  zarr:::create_dataset(d1, m)
  expect_true(dir.exists(paste0(path, "/d1")))

  d2 <- zarr:::get_dataset_handle(g, "d2")
  expect_false(dir.exists(paste0(path, "/group/d1")))

  zarr:::create_dataset(d2, m)
  expect_true(dir.exists(paste0(path, "/group/d1")))
})

### TODO: keys
