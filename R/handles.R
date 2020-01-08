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
  structure(getFileHandle(x, mode), class = "file_handle")
}

get_file_handle.file_handle <- function(x, ...) {
  ## The problem here is that s3 classes need to be exported and I want to keep
  ## this part of the internals for now and only expose a minimal interface.
  ## Therefore there is no "free" type checking here.
  if (!inherits(x, "file_handle")) stop("x must be of class \"file_handle\"")
  x
}
### create file ####################################

## create_file <- function(x) {
##   UseMethod("create_file")
## }

## create_file.default <- function(x) {
##   stop("cannot create file from ", class(x))
## }

create_file.file_handle <- function(x) {
  if (!inherits(x, "file_handle")) stop("cannot create file from ", class(x))
  FileHandleCreate(x)
}


### group handle

### create group ###################################

## create_group <- function(x) {
##   UseMethod("create_group")
## }

## create_group.default <- function(x) {
##   stop("cannot create group from ", class(x))
## }

create_group.group_handle <- function(x) {
  if(!inherits(x, "group_handle")) stop("x must be of class \"group_handle\"")
  GroupHandleCreate(x)
}

## get_group_handle <- function(x, key) {
##   UseMethod("get_group_handle")
## }

## get_group_handle.default <- function(x, key) {
##   stop("cannot get group handle from ", class(x))
## }

get_group_handle.file_handle <- function(x, key) {
  structure(getGroupHandleFileHandle(x, key), class = "group_handle")
}

get_group_handle.group_handle <- function(x, key) {
  structure(getGroupHandleGroupHandle(x, key), class = "group_handle")
}

### dataset handle
### NOTE: Try to avoid using z5::filesystem::handle::dataset, use z5::dataset instead

## get_dataset_handle <- function(x, key) {
##   UseMethod("get_dataset_handle")
## }

## get_dataset_handle.default <- function(x, key) {
##   stop("cannot get group handle from ", class(x))
## }

## get_dataset_handle.file_handle <- function(x, key) {
##   structure(getDatasetHandleFileHandle(x, key), class = "dataset_handle")
## }

## get_dataset_handle.group_handle <- function(x, key) {
##   structure(getDatasetHandleGroupHandle(x, key), class = "dataset_handle")
## }
