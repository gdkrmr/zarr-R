#' zarr: A package for Zarr arrays
#'
#' This package wraps C++ Code for Zarr Arrays
#'
#'
#' @docType package
#' @name zarr
NULL

#' @useDynLib zarr
NULL

#' @include RcppExports.R
#' @include helpers.R

.onUnload <- function (libpath) {
  library.dynam.unload("zarr", libpath)
}


### interface

#' TODO: read_attributes.zarr_dataset
#' @export
read_attributes <- function(path) {
  att <- readAttributes(path)
  class(att) <- "zarr_attributes"
  return(att)
}

#' @export
write_attributes <- function(path, attributes) {
  writeAttributes(path, attributes)
}

#' @export
open_zarr <- function (path, file_mode = "a") {
  ds <- openDataset(path, file_mode)
  class(ds) <- "zarr_dataset"
  return(ds)
}


#' // the (python / h5py) I/O modes:
#' // r: can only read, file must exist
#' // r+: can read and write, file must exist
#' // w: can read and write, if file exists, will be overwritten
#' // w-: can read and write, file must not exist
#' // x: can read and write, file must not exist (same as w- ?!, so omitted here)
#' // a: can read and write


#' @export
create_dataset <- function(path, shape, chunk_shape, data_type = "float64",
                           fill_value = NULL, file_mode = "a",
                           compressor = "raw", compression_options = list(),
                           as_zarr = TRUE) {
  if (is.null(fill_value)) {
    if (data_type == "float64") { fill_value <- 0 }
    else stop("unknown data_type")
  }
  res <- createDataset(path, data_type, shape, chunk_shape,
                       as_zarr, compressor, compression_options,
                       fill_value, file_mode)
  class(res) <- "zarr_dataset"
  return(res)
}

#' TODO: better function for this? S3 method?
#' @export
get_path <- function (x) {
  if (inherits(x, "zarr_dataset")) return(getPath(x))
  else stop("x must be of type `zarr_dataset`")
}

#' @export
dim.zarr_attributes <- function (x) {
  return(as.numeric(x$shape))
}

#' @export
dim.zarr_dataset <- function (x) {
  attribs <- read_attributes(paste0(get_path(x), "/.zarray"))
  return(dim(attribs))
}

#' @export
`dim<-.zarr_dataset` <- function(x) {
  stop("you cannot change the dimensions of a `zarr_dataset` object after creation.")
}

#' @export
`[.zarr_dataset` <- function (x, ..., drop = FALSE) {

  ## handle weird cases, such as x[,,1]
  ## first element is the function name, then the arguments

  ## The following can also not be put into its own function, because the empty
  ## symbol get "lost" when passed to another function via ...

  ## thanks to Roland
  ## https://stackoverflow.com/questions/58856063/r-function-calls-and-missing-values/58857798#58857798

  ellipsis_args <- as.list(match.call())[-1:-2]
  ellipsis_args$drop <- NULL
  dim_x <- dim(x)

  for (i in seq_along(ellipsis_args)) {
    e_i <- ellipsis_args[[i]]
    if      (missing(e_i))   {ellipsis_args[[i]] <- c(1, dim_x[i])}
    else if (is_range(e_i))  {ellipsis_args[[i]] <- c(e_i[[2]], e_i[[3]])}
    else if (is.symbol(e_i)) {ellipsis_args[[i]] <- eval(e_i, envir = parent.frame(1))}
    else if (is.atomic(e_i) && length(e_i) == 1) {}
    else stop("The dimensions must be specified as empty arguments, single digits, or ranges")
  }

  ## quote = TRUE prevents 1:i from expanding
  os <- do.call(range_to_offset_shape, ellipsis_args)
  str(os)
  res <- readSubarray(x, os$offset, os$shape)
  if (drop) {res <- drop_dim(res)}

  return(res)
}

#' @export
`[<-.zarr_dataset` <- function (x, ..., value) {

  ellipsis_args <- as.list(match.call())[-1:-2]
  ellipsis_args$value <- NULL
  dim_x <- dim(x)

  for (i in seq_along(ellipsis_args)) {
    e_i <- ellipsis_args[[i]]
    if      (missing(e_i))   {ellipsis_args[[i]] <- c(1, dim_x[i])}
    else if (is_range(e_i))  {ellipsis_args[[i]] <- c(e_i[[2]], e_i[[3]])}
    else if (is.symbol(e_i)) {}
    else if (is.atomic(e_i) && length(e_i) == 1) {}
    else stop("The dimensions must be specified as empty arguments, single digits, or ranges")
  }

  os <- do.call(range_to_offset_shape, ellipsis_args)

  if (is.null(dim(value))) {
    dim(value) <- rep(1, length(os$shape))
  }

  if (!all.equal(dim(value), os$shape)) {
    stop("Shape does not match")
  }

  writeSubarray(x, value, os$offset)
  return(x)
}
