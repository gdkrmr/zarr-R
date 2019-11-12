range_to_offset_shape <- function(...) {
  i <- list(...)

  ## zarr arrays have 0 based indexing!!!
  offset <- sapply(i, function(x) x[1]) - 1
  shape <- sapply(i, function(x) x[length(x)] - x[1] + 1)
  return(list(offset = offset, shape = shape))
}


drop_dim <- function(x) {
  dim_x <- dim(x)[dim(x) != 1]
  if (length(dim_x) == 0) {
    dim(x) <- NULL
  } else {
    dim(x) <- dim_x
  }
  return(x)
}

