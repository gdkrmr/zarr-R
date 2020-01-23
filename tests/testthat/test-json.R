context("json")

test_set <- list(
  list('{"hello":"a"}', list(hello = "a")),
  list("null", list()),
  list('{"hello":1.0}', list(hello = 1)),
  list('{"hello you":1.0}', list(`hello you` = 1)),
  list('{"hello":null}', list(hello = NULL)),
  list("[1.0,2.0,3.0]", list(1, 2, 3)),
  list('{"hello":[null,2.0,3.0]}', list(hello = list(NULL, 2, 3))),
  list('{"hello":[1.0,2.0,3.0]}', list(hello = list(1, 2, 3)))
)

for (i in seq_along(test_set)) {
  str_par <- test_set[[i]]
  str  <- str_par[[1]]
  par <- str_par[[2]]
  expect_equal(zarr:::json_to_list(str), par)
  expect_equal(zarr:::list_to_json(par), str)
}

expect_equal(zarr:::json_to_list("{}"), list())
expect_equal(zarr:::json_to_list("[]"), list())
expect_error(zarr:::json_to_list(""))

expect_failure(expect_equal(zarr:::json_to_list('{"hello": [null, "NaN", 3]}'),
                            list(hello = list(NULL, NaN, 3))))
