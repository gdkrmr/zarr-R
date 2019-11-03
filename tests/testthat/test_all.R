context("Zarr")

test_that("Zarr", {
### File
  file <- zarr::create_file("test_array", "a")
  expect_s3_class(file, "zarr_file")
  expect_true(dir.exists("test_array"))

### DataSet
  data_set <- zarr::create_dataset("test_array", c(3, 3, 3), c(3, 3, 3))

### write
  val1 <- 5
  data_set[1, 1, 1] <- val1

  val2 <- array(1:8, c(2, 2, 2))
  data_set[2:3, 2:3, 2:3] <- val2

### read
  expect_equal(data_set[1, 1, 1], val1)
  expect_equal(data_set[2:3, 2:3, 2:3], val2)

### cleanup
  unlink("test_array", recursive = TRUE)
  expect_false(dir.exists("test_array"))
})


