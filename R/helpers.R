## thanks to Roland
## https://stackoverflow.com/questions/58856063/r-function-calls-and-missing-values/58857798#58857798
is_empty_symbol <- function(x) is.symbol(x) && identical("", as.character(x))
is_range <- function(x) is.language(x) && !is.symbol(x) && identical(":", as.character(x[[1]]))
is_singleton <- function(x) is.atomic(x) && length(x) == 1

range_to_offset_shape <- function(...) {
  idxs <- list(...)

  ## zarr arrays have 0 based indexing!!!
  offset <- vapply(idxs, function(x) x[1] - 1, FUN.VALUE = 0)
  shape <- vapply(idxs, function(x) x[length(x)] - x[1] + 1, FUN.VALUE = 0)

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

