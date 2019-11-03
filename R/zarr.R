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
open_zarr <- function (path, mode) {
  ds <- openDataset(path, mode)
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
create_file <- function (path, mode) {
  res <- createFile(path, mode)
  class(res) <- "zarr_file"
  return(res)
}

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

#' @export
`[.zarr_dataset` <- function (x, ...) {
  readSubarray(x, range_to_offset_shape(...))
}

#' @export
`[<-.zarr_dataset` <- function (x, ..., value) {
  os <- range_to_offset_shape(dim(value))
  if(!all.equal(dim(value), os$shape))
    stop("Shape does not match")
  writeSubarray(x, value, os$offset)
}
