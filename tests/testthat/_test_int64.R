context("Int64")

path <- tempfile()
shape <- c(9, 9)
chunk_shape <- c(3, 3)
fill_value <- 0
data_type <- c("int64")

data_set <- create_dataset(
  path, shape, chunk_shape,
  data_type = data_type, fill_value = fill_value
)

expect_identical(data_set[1, 1, drop = TRUE], fill_value)

test_data <- structure(as.integer(rnorm(prod(shape))), dim = shape)

data_set[,] <- test_data

recovered_data <- data_set[, ]

expect_equal(test_data, recovered_data)

expect_true(is.double(recovered_data))

unlink(path, recursive = TRUE)


if (FALSE) {

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
