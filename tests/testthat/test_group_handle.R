testthat("group handle", {

  library(testthat)
  library(zarr)

  path <- tempfile()

  f <- zarr:::get_file_handle.character(path)

  g1 <- zarr:::get_group_handle.file_handle(f, "group1")
  expect_false(dir.exists(paste0(path, "/group1")))
  expect_false(zarr::GroupHandleExists(g1))

  zarr:::create_group.group_handle(g1)
  expect_true(dir.exists(paste0(path, "/group1")))
  expect_true(zarr::GroupHandleExists(g1))

  g2 <- zarr:::get_group_handle.group_handle(g1, "group2")
  expect_false(dir.exists(paste0(path, "/group1/group2")))
  expect_false(zarr::GroupHandleExists(g2))

  zarr:::create_group.group_handle(g2)
  expect_true(dir.exists(paste0(path, "/group1/group2")))
  expect_true(zarr:::GroupHandleExists(g2))

  expect_false(zarr:::GroupHandleIsS3(g1))
  expect_false(zarr:::GroupHandleIsS3(g2))
  expect_false(zarr:::GroupHandleIsGcs(g1))
  expect_false(zarr:::GroupHandleIsGcs(g2))
  expect_failure(
    expect_true(zarr:::GroupHandleIsZarr(g1))
  )
  expect_failure(
    expect_true(zarr:::GroupHandleIsZarr(g2))
  )

  expect_equal(zarr:::GroupHandlePath(g1), paste0(path, "/group1"))
  expect_equal(zarr:::GroupHandlePath(g2), paste0(path, "/group1/group2"))

  expect_equal(zarr:::GroupHandleKeys(g1), c("group2"))
  expect_equal(zarr:::GroupHandleKeys(g2), character(0))

  expect_true(zarr:::GroupHandleIn(g1, "group2"))

  expect_equal(zarr:::GroupHandleBucket(g1), "")
  expect_equal(zarr:::GroupHandleBucket(g2), "")

  expect_equal(zarr:::GroupHandleNameInBucket(g1), "")
  expect_equal(zarr:::GroupHandleNameInBucket(g2), "")
  expect_equal(zarr:::GroupHandleBucket(g1), "")
  expect_equal(zarr:::GroupHandleBucket(g2), "")

  ## NOTE: It is possible to delete this the other way round!
  zarr:::GroupHandleDelete(g2)

  expect_false(zarr:::GroupHandleExists(g2))
  expect_false(dir.exists(paste0(path, "/group1/group2")))

  zarr:::GroupHandleDelete(g1)
  expect_false(zarr:::GroupHandleExists(g1))
  expect_false(dir.exists(paste0(path, "/group1")))

  zarr:::FileHandleDelete(f)
  expect_false(zarr:::FileHandleExists(f))
  expect_false(dir.exists(path))

  expect_error(zarr:::get_group_handle.group_handle(1))
  expect_error(zarr:::get_group_handle.file_handle(1))

})
