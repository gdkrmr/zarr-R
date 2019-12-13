### file handle

## get_file_handle <- function(x, ...) {
##   UseMethod("get_file_handle")
## }

## get_file_handle.default <- function(x, ...) {
##   stop("cannot get file handle from ", class(x))
## }

get_file_handle.character <- function(x, mode = "a") {
  if (!inherits(x, "character")) stop("x must be a character vector.")
  if (length(x) != 1) stop("length(x) must be 1.")
  structure(getFileHandle(path, mode), class = "file_handle")
}

get_file_handle.file_handle <- function(x, ...) {
  x
}

### group handle

## get_group_handle <- function(x, key) {
##   UseMethod("get_group_handle")
## }

## get_group_handle.default <- function(x, key) {
##   stop("cannot get group handle from ", class(x))
## }

get_group_handle.file_handle <- function(x, key) {
  structure(getGroupHandleFile(x, key), class = "group_handle")
}

get_group_handle.group_handle <- function(x, key) {
  structure(getGroupHandleGroup(x, key), class = "group_handle")
}

### dataset handle

## get_dataset_handle <- function(x, key) {
##   UseMethod("get_dataset_handle")
## }

## get_dataset_handle.default <- function(x, key) {
##   stop("cannot get group handle from ", class(x))
## }

get_dataset_handle.file_handle <- function(x, key) {
  structure(getDatasetHandleFile(x, key), class = "dataset_handle")
}

get_dataset_handle.group_handle <- function(x, key) {
  structure(getDatasetHandleGroup(x, key), class = "dataset_handle")
}
