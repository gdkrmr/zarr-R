range_to_offset_shape <- function(...) {
  i <- list(...)
  offset <- sapply(i, function(x) x[1])
  shape <- sapply(i, function(x) x[length(x)] - x[1] + 1)
  return(list(offset = offset, shape = shape))
}


