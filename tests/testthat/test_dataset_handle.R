testthat("dataset handle", {

  library(testthat)

  path <- tempfile()
  f <- zarr:::get_file_handle.character(path)
  g <- zarr:::get_group_handle.file_handle(f, "group")

  m <- zarr:::create_dataset_metadata(c(9, 9), c(3, 3))

  d1 <- zarr::get_dataset_handle.file_handle(f, "d1")
  expect_false(dir.exists(paste0(path, "/d1")))

  zarr:::create_dataset.dataset_handle(d1, m)
  expect_true(dir.exists(paste0(path, "/d1")))

  d2 <- zarr:::get_dataset_handle(g, "d2")
  expect_false(dir.exists(paste0(path, "/group/d1")))

  zarr:::create_dataset(d2, m)
  expect_true(dir.exists(paste0(path, "/group/d1")))
})
