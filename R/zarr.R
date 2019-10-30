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


.onUnload <- function (libpath) {
  library.dynam.unload("zarr", libpath)
}


### interface

#' @export
create_file <- function (path) {

}

#' @export
open_zarr <- function (path) {

}

#' @export
create_dataset <- function(file, name, shape, chunks, dtype = "float64") {

}

#' @export
close_zarr <- function (x) {

}

#' @export
close.zarr <- function (x) {
  close_zarr(x)
}

#' @export
`[.zarr` <- function (x, ...) {

}

#' @export
`[<-.zarr` <- function (x, ...) {

}
