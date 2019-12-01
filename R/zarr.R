#' zarr: A package for Zarr arrays
#'
#' This package wraps C++ Code for Zarr Arrays
#'
#'
#' @docType package
#' @name zarr
NULL

#' @importFrom Rcpp evalCpp
#' @useDynLib zarr
NULL

#' @include RcppExports.R
#' @include helpers.R

.onUnload <- function (libpath) {
  library.dynam.unload("zarr", libpath)
}


### interface

#' @export
read_attributes <- function(x, ...) {
  UseMethod("read_attributes", x)
}

#' @export
read_attributes.character <- function(path) {
  if (!dir.exists(path) && file.exists(path)) {
    att <- readAttributes(path)
    class(att) <- "zarr_attributes"
    return(att)
  } else {
    stop("path does not exist")
  }
}

#' @export
read_attributes.zarr_dataset <- function(dataset) {
  read_attributes(paste(get_path(dataset), ".zarray", sep = "/"))
}

#' @export
write_attributes <- function(x, ...) {
  UseMethod("write_attributes", x)
}

#' @export
write_attributes.character <- function(path, attributes) {
  writeAttributes(path, attributes)
}

#' @export
write_attributes.zarr_dataset <- function(dataset, attributes, ...) {
  write_attributes(paste(get_path(dataset), ".zarray", sep = "/"),
                   attributes)
}

#' @export
open_zarr <- function(path, file_mode = "a") {
  ds <- openDataset(path, file_mode)
  class(ds) <- "zarr_dataset"
  return(ds)
}


#' // the (python / h5py) I/O modes:
#' // r: can only read, file must exist
#' // r+: can read and write, file must existinline nlohmann::json rlist_to_json(const Rcpp::List & l)
#' // w: can read and write, if file exists, will be overwritten
#' // w-: can read and write, file must not exist
#' // x: can read and write, file must not exist (same as w- ?!, so omitted here)
#' // a: can read and write


#' possible values for data_type:
#' "int8", "int16", "int32", "int64",
#' "uint8", "uint16", "uint32", "uint64",
#' "float32", "float64",

#' missing_value can be c("auto") or any number, should probably not be NA

#' @export
create_dataset <- function(path,
                           shape, chunk_shape,
                           data_type = "float64",
                           fill_value = "auto", missing_value = "auto",
                           file_mode = "a",
                           compressor = "raw", compression_options = list(),
                           as_zarr = TRUE) {
  if (fill_value == "auto") {
    if      (data_type == "int8")    { fill_value <- 0L         }
    else if (data_type == "int16")   { fill_value <- 0L         }
    else if (data_type == "int32")   { fill_value <- NA_integer }
    else if (data_type == "int64")   { fill_value <- 0L         }
    else if (data_type == "uint8")   { fill_value <- NA         }
    else if (data_type == "uint16")  { fill_value <- 0L         }
    else if (data_type == "uint32")  { fill_value <- 0L         }
    else if (data_type == "uint64")  { fill_value <- 0L         }
    else if (data_type == "float32") { fill_value <- 0          }
    else if (data_type == "float64") { fill_value <- NA_real_   }
    else stop("unknown data_type")
  }

  if (missing_value == "auto") { missing_value <- fill_value }

  res <- createDataset(path, data_type, shape, chunk_shape,
                       as_zarr, compressor, compression_options,
                       fill_value, file_mode)
  class(res) <- "zarr_dataset"

  res_attr <- read_attributes(res)
  res_attr$missing_value <- missing_value
  write_attributes(res, res_attr)

  return(res)
}

#' TODO: better function for this? S3 method?
#' @export
get_path <- function(x, ...) {
  UseMethod("get_path", x)
}

#' @export
get_path.zarr_dataset <- function(x, ...) {
  return(getPath(x))
}

#' @export
dim.zarr_attributes <- function(x) {
  return(as.numeric(x$shape))
}

#' @export
dim.zarr_dataset <- function(x) {
  attribs <- read_attributes(paste0(get_path(x), "/.zarray"))
  return(dim(attribs))
}

#' @export
`dim<-.zarr_dataset` <- function(x, value) {
  stop("you cannot change the dimensions of a `zarr_dataset` object after creation.")
}

#' @export
`[.zarr_dataset` <- function(x, ..., drop = FALSE) {

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
    if      (missing(e_i))      { ellipsis_args[[i]] <- c(1, dim_x[i]) }
    else if (is_range(e_i))     { ellipsis_args[[i]] <- c(e_i[[2]], e_i[[3]]) }
    else if (is.symbol(e_i))    { ellipsis_args[[i]] <- eval(e_i, envir = parent.frame(1)) }
    else if (is_singleton(e_i)) {  }
    else stop("The dimensions must be specified as ",
              "empty arguments, single digits, or ranges")
  }

  ## quote = TRUE prevents 1:i from expanding
  os <- do.call(range_to_offset_shape, ellipsis_args)
  res <- readSubarray(x, os$offset, os$shape)
  if (drop) { res <- drop_dim(res) }

  return(res)
}

#' @export
`[<-.zarr_dataset` <- function(x, ..., value) {

  ellipsis_args <- as.list(match.call())[-1:-2]
  ellipsis_args$value <- NULL
  dim_x <- dim(x)

  for (i in seq_along(ellipsis_args)) {
    e_i <- ellipsis_args[[i]]
    if      (missing(e_i))   { ellipsis_args[[i]] <- c(1, dim_x[i]) }
    else if (is_range(e_i))  { ellipsis_args[[i]] <- c(e_i[[2]], e_i[[3]]) }
    else if (is.symbol(e_i)) { ellipsis_args[[i]] <- eval(e_i, envir = parent.frame(1))  }
    else if (is_singleton(e_i)) {  }
    else stop("The dimensions must be specified as ",
              "empty arguments, single digits, or ranges")
  }

  os <- do.call(range_to_offset_shape, ellipsis_args)

  if (length(value) == 1) {
    dim(value) <- rep(1, length(os$shape))
  } else if (is.null(dim(value))) {
    dim(value) <- length(value)
  }

  dim(value) <- match_shape(dim(value), os$shape)

  writeSubarray(x, value, os$offset)
  return(x)
}
