## thanks to Roland
## https://stackoverflow.com/questions/58856063/r-function-calls-and-missing-values/58857798#58857798
is_empty_symbol <- function(x) is.symbol(x) && identical("", as.character(x))
is_range <- function(x)
  is.language(x) && !is.symbol(x) && identical(":", as.character(x[[1]]))
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
  if (length(dim_x) <= 1) { dim(x) <- NULL }
  else                    { dim(x) <- dim_x }
  return(x)
}

match_shape <- function(dim_value, shape_target) {
  new_dim_value <- rep(1, length(shape_target))
  dim_value_idx <- 1
  for (i in seq_along(shape_target)) {
    if (dim_value_idx <= length(dim_value) &&
      dim_value[dim_value_idx] == shape_target[i]) {
      new_dim_value[i] <- shape_target[i]
      dim_value_idx <- dim_value_idx + 1
    } else if (shape_target[i] == 1) {
      new_dim_value[i] <- 1
    }
  }

  if (!identical(new_dim_value, shape_target)) {
    stop(
      "shapes don't match, ",
      "value_shape: c(", paste(dim_value, collapse = ", "), "), ",
      "target_shape: c(", paste(shape_target, collapse = ", "), ")"
    )
  }

  return(new_dim_value)
}

dt_to_zarrdt <- list(
  int8    = "|i1",
  int16   = "<i2",
  int32   = "<i4",
  int64   = "<i8",
  uint8   = "|u1",
  uint16  = "<u2",
  uint32  = "<u4",
  uint64  = "<u8",
  float32 = "<f4",
  float64 = "<f8"
)

zarrdt_to_dt <- list(
  `|i1` = "int8",
  `<i2` = "int16",
  `<i4` = "int32",
  `<i8` = "int64",
  `|u1` = "uint8",
  `<u2` = "uint16",
  `<u4` = "uint32",
  `<u8` = "uint64",
  `<f4` = "float32",
  `<f8` = "float64"
)
