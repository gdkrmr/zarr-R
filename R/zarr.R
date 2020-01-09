#' @include RcppExports.R helpers.R handles.R metadata.R factory.R
NULL


#' Read attributes
#'
#' @description
#'
#' Read the attributes of an object or path
#'
#' @details
#'
#' This is an S3 generic to read the attributes of zarr arrays.
#'
#' @param x The object
#' @param ... currently unused
#'
#' @export
read_attributes <- function(x, ...) {
  UseMethod("read_attributes", x)
}

#' @export
read_attributes.character <- function(x, ...) {
  if (!dir.exists(x) && file.exists(x)) {
    att <- readAttributes(x)
    class(att) <- "zarr_attributes"
    return(att)
  } else {
    stop("path does not exist")
  }
}

#' @export
read_attributes.zarr_dataset <- function(x, ...) {
  read_attributes(paste(get_path(x), ".zarray", sep = "/"))
}

#' Write attributes
#'
#' Write the attributes of an object or path
#'
#' @param x The object
#' @param attributes A list of attributes
#' @param ... Currently unused
#' @export
write_attributes <- function(x, attributes, ...) {
  UseMethod("write_attributes", x)
}

#' @export
write_attributes.character <- function(x, attributes, ...) {
  writeAttributes(x, attributes)
}

#' @export
write_attributes.zarr_dataset <- function(x, attributes, ...) {
  write_attributes(paste(get_path(x), ".zarray", sep = "/"),
                   attributes)
}

#' Open a zarr object.
#'
#' Opens a zarr array saved on disk.
#'
#' @param path The path on disk
#' @param file_mode File mode to open the object
#' @export
open_zarr <- function(path, file_mode = "a") {
  ds <- openDataset(path, file_mode)
  class(ds) <- "zarr_dataset"
  return(ds)
}

#' Data type
#'
#' Get the data type of an object.
#'
#' @param x The object
#' @param ... Unused
#' @export
data_type <- function(x, ...) {
  UseMethod("data_type", x)
}

#' @export
data_type.zarr_dataset <- function(x, ...) {
  data_type(read_attributes(x))
}

#' @export
data_type.zarr_attributes <- function(x, ...) {
  zarrdt_to_dt[[x$dtype]]
}


# // the (python / h5py) I/O modes:
# // r: can only read, file must exist
# // r+: can read and write, file must exist
# // w: can read and write, if file exists, will be overwritten
# // w-: can read and write, file must not exist
# // x: can read and write, file must not exist (same as w- ?!, so omitted here)
# // a: can read and write
#
# possible values for data_type:
# "int8", "int16", "int32", "int64",
# "uint8", "uint16", "uint32", "uint64",
# "float32", "float64",
#
# {int8 , "|i1"},
# {int16, "<i2"},
# {int32, "<i4"},
# {int64, "<i8"},
# {uint8 , "|u1"},
# {uint16, "<u2"},
# {uint32, "<u4"},
# {uint64,"<u8"},
# {float32, "<f4"},
# {float64,"<f8"}}});
#
# missing_value can be c("auto") or any number, should probably not be NA


#' Create a zarr dataset
#'
#' Creat a dataset in zarr format on disk
#'
#' @param path The path on disk
#' @param shape The size of the data set
#' @param chunk_shape The size of the chunks
#' @param data_type The data type to store the data
#' @param fill_value The default value of data
#' @param missing_value The value to be interpreted as NA
#' @param file_mode How to open the file
#' @param compressor The compression to be used
#' @param compression_options The options for compression
#' @param as_zarr Save as zarr?
#' @export
create_dataset <- function(x, key, shape, chunk_shape,
                           data_type = "float64",
                           fill_value = "auto", missing_value = "auto",
                           file_mode = "a",
                           compressor = "raw", compression_options = list(),
                           as_zarr = TRUE, ...) {

  if (fill_value == "auto") {
    if      (data_type == "int8")    { fill_value <- 0L          }
    else if (data_type == "int16")   { fill_value <- 0L          }
    else if (data_type == "int32")   { fill_value <- NA_integer_ }
    else if (data_type == "int64")   { fill_value <- 0L          }
    else if (data_type == "uint8")   { fill_value <- NA          }
    else if (data_type == "uint16")  { fill_value <- 0L          }
    else if (data_type == "uint32")  { fill_value <- 0L          }
    else if (data_type == "uint64")  { fill_value <- 0L          }
    else if (data_type == "float32") { fill_value <- 0           }
    else if (data_type == "float64") { fill_value <- NA_real_    }
    else                             { stop("unknown data_type") }
  }

  if (missing_value == "auto") { missing_value <- fill_value }


  if (inherits(x, "group_handle")) {
    res <- createDatasetGroupHandle(x, key, data_type, shape, chunk_shape,
                                    compressor, compression_options, fill_value)
  } else if (inherits(x, "file_handle")) {
    res <- createDatasetFileHandle(x, key, data_type, shape, chunk_shape,
                                   compressor, compression_options, fill_value)
  } else if (inherits(x, "dataset_handle")) {
    if (!missing(key))
      warning("creating dataset from dataset handle, `key` will be ignored!")
    res <- createDatasetDatasetHandle(x, data_type, shape, chunk_shape,
                                      compressor, compression_options, as_zarr,
                                      fill_value)
  } else {
    stop("Cannot create dataset from class(x) ", class(x))
  }


  class(res) <- "zarr_dataset"

  ## res_attr <- read_attributes(res)
  ## res_attr$missing_value <- missing_value
  ## write_attributes(res, res_attr)

  return(res)
}


#' Open a Zarr dataset
#'
#' Open a Zarr dataset from a file, group or dataset handles
#'
#' @param x the handle
#' @param key internal path of the data set
#' @export
open_dataset <- function(x, key, ...) {
  UseMethod("open_dataset", x)
}

#' @export
open_dataset.default <- function(x, key, ...) {
  stop("no method for class(x) ", class(x))
}

#' @export
open_dataset.file_handle <- function(x, key) {
  structure(openDatasetFile(x, key), class = "dataset")
}

#' @export
open_dataset.group_handle <- function(x, key) {
  structure(openDatasetGroup(x, key), class = "dataset")
}

#' @export
open_dataset.dataset_handle <- function(x, key) {
  if (!missing(key)) warning("key will be ignored.")
  structure(openDatasetDataset(x), class = "dataset")
}

#' Get the path of an object.
#'
#' Gets the path of an object stored on disk.
#'
#' @param x The object
#' @param ... Currently unused
#'
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
