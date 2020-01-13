
context("attributes")

test_that("attributes", {

  path <- tempfile()

  f <- zarr:::get_file_handle(path)
  g <- zarr:::get_group_handle(f, "group1")
  d1 <- zarr:::get_dataset_handle(f, "dataset1")
  d2 <- zarr:::get_dataset_handle(g, "dataset2")

  zarr:::create_group(g)
  dd1 <- zarr:::create_dataset(d1, shape = c(9, 9, 9), chunk_shape = c(3, 3, 3))
  dd2 <- zarr:::create_dataset(d2, shape = c(9, 9, 9), chunk_shape = c(3, 3, 3))

  empty_attr <- structure(list(), class = "zarr_attributes")
  expect_equal(read_attributes(f), empty_attr)
  expect_equal(read_attributes(g), empty_attr)
  expect_equal(read_attributes(d1), empty_attr)
  expect_equal(read_attributes(d2), empty_attr)

  attr_list <- structure(list(hello = "hello"), class = "zarr_attributes")
  write_attributes(f, attr_list)
  expect_equal(read_attributes(f), attr_list)
  write_attributes(g, attr_list)
  expect_equal(read_attributes(g), attr_list)
  write_attributes(d1, attr_list)
  expect_equal(read_attributes(d1), attr_list)
  write_attributes(d2, attr_list)
  expect_equal(read_attributes(d2), attr_list)

  remove_attributes(f, "hello")
  expect_equal(read_attributes(f), empty_attr)
  remove_attributes(g, "hello")
  expect_equal(read_attributes(g), empty_attr)
  remove_attributes(d1, "hello")
  expect_equal(read_attributes(d1), empty_attr)
  remove_attributes(d2, "hello")
  expect_equal(read_attributes(d2), empty_attr)

  zarr:::DatasetHandleDelete(d1)
  zarr:::DatasetHandleDelete(d2)
  zarr:::GroupHandleDelete(g)
  zarr:::FileHandleDelete(f)

})

