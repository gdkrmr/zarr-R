context("Int32")

path <- tempfile()
shape <- c(9, 9)
chunk_shape <- c(3, 3)
fill_value <- 0L
data_type <- c("int32")

data_set <- create_dataset(
  path, shape, chunk_shape,
  data_type = data_type, fill_value = fill_value
)

expect_identical(data_set[1, 1, drop = TRUE], fill_value)

test_data <- structure(as.integer(rnorm(prod(shape))), dim = shape)

data_set[,] <- test_data

recovered_data <- data_set[, ]

expect_equal(test_data, recovered_data)

expect_true(is.integer(recovered_data))

unlink(path, recursive = TRUE)


if (FALSE) {

  path

  for (i in 1:dim(data_set)[1]){
    for (j in 1:dim(data_set)[2]){
      data_set[i, j] <- 1
    }
  }

  data_set[1:3, 1:3] <- test_data[1:3, 1:3]

  data_set[1:3, 1:3] <- matrix(1:9, 3)

  data_set[1:9,1] <- 11:19

  data_set[1, 1:9] <- 11:19


  data_set[,] <- test_data

  data_set[,]

  test_data

  data_set[,]

data_set[1, 1] <- -2L

data_set[1, 1]

data_set[2, 1] <- -4L

data_set[2, 1]

data_set[4, 2] <- 5L

data_set[4:5, 2:3]

test_data[4:5, 1:3]

test_data[4:6, 2:4]

test_data[1:5, 1:3]

data_set[,] <- test_data

data_set[3, 2]

data_set[,]

recovered_data <- data_set[,]

test_data

data_set[,][1:3, 1:3]


}
