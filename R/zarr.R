#' @include RcppExports.R helpers.R handles.R metadata.R factory.R
NULL

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
#' @param x A `group_handle`, `file_handle`, or `dataset_handle`
#' @param key The name of the dataset
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
                           as_zarr = TRUE) {

  compression_options <- complete_compression_options(compressor,
                                                      compression_options)

  if (is.character(fill_value) && fill_value == "auto") {
    fill_value <- TYPE_TO_AUTO_FILL_VALUE[[data_type]]
    if (is.null(fill_value)) { stop("unknown data_type") }
  }

  if (missing_value == "auto") { missing_value <- fill_value }

  if (inherits(x, "character")) {
    file_handle <- get_file_handle(x, file_mode)
    if (!FileHandleExists(file_handle))
      FileHandleCreate(x, as_zarr)
    res <- createDatasetFileHandle(file_handle, key, data_type, shape,
                                   chunk_shape, compressor, compression_options,
                                   as_zarr, fill_value)
  } else if (inherits(x, "group_handle")) {
    if (!GroupHandleExists(x))
      GroupHandleCreate(x, as_zarr)
    res <- createDatasetGroupHandle(x, key, data_type, shape, chunk_shape,
                                    compressor, compression_options, as_zarr,
                                    fill_value)
  } else if (inherits(x, "file_handle")) {
    if (!FileHandleExists(x))
      FileHandleCreate(x, as_zarr)
    res <- createDatasetFileHandle(x, key, data_type, shape, chunk_shape,
                                   compressor, compression_options, as_zarr,
                                   fill_value)
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

  return(res)
}


#' Open a Zarr dataset
#'
#' Open a Zarr dataset from a file, group or dataset handles
#'
#' @param x the handle
#' @param key internal path of the data set
#' @param mode file mode
#' @export
open_dataset <- function(x, key, mode = "a") {
  if (inherits(x, "character")) {
    if (missing(key)) {
      # TODO: open dataset from path only!
      # use constructur of z5::Filesystem::Handle::Dataset(path, FileMode)
    } else {
      fh <- getFileHandle(x, mode)
      res <- openDatasetFileHandle(fh, key)
    }
  } else if (inherits(x, "file_handle")) {
    res <- openDatasetFileHandle(x, key)
  } else if (inherits(x, "group_handle")) {
    res <- openDatasetGroupHandle(x, key)
  } else if (inherits(x, "dataset_handle")) {
    if (!missing(key)) warning("key will be ignored.")
    res <- openDatasetDatasetHandle(x)
  } else {
    stop("Cannot open dataset from class(x) = ", class(x))
  }

  class(res) <- "zarr_dataset"

  return(res)
}

#' @export
dim.dataset_metadata <- function(x) { as.integer(as.list(x)$shape) }

#' @export
dim.zarr_dataset <- function(x) { DatasetShape(x) }

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

  ## str(os$offset)
  ## str(os$shape)

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

  ## str(dim(value))
  ## str(os$offset)

  writeSubarray(x, value, os$offset)
  return(x)
}
