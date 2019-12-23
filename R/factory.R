#' @include metadata.R
NULL

### create group ###################################

## create_group <- function(x) {
##   UseMethod("create_group")
## }

## create_group.default <- function(x) {
##   stop("cannot create group from ", class(x))
## }

create_group.group_handle <- function(x) {
  GroupHandleCreate(x)
}


### create file ####################################

## create_file <- function(x) {
##   UseMethod("create_file")
## }

## create_file.default <- function(x) {
##   stop("cannot create file from ", class(x))
## }

## create_file.file_handle <- function(x) {
##   FileHandleCreate(x)
## }

### open dataset ###################################

## open_dataset <- function(x, key, ...) {
##   UseMethod("open_dataset")
## }

## this one should work for all group/file handle types
open_dataset.default <- function(x, key) {
  if (inherits(x, "file_handle")) {
    res <- openDatasetFile(x, key)
  } else if (inherits(x, "group_handle")) {
    res <- openDatasetGroup(x, key)
  } else {
    stop("open_dataset doesn't work for the provided class.")
  }
  structure(res, class = "dataset")
}

## this one is for z5::filesystem::handle::dataset
open_dataset.dataset_handle <- function(x) {
  structure(openDatasetDataset(x),
    class = "dataset"
  )
}

### create dataset #################################

### TODO: export all the z5::dataset methods
### TODO: implement create_dataset.group_handle
### TODO: implement create_dataset.file_handle
### TODO: implement create_dataset.dataset_handle

## create_dataset <- function(x, m) {
##   UseMethod("create_dataset")
## }

## create_dataset.default <- function(x, m) {
##   stop("cannot create dataset from ", class(x))
## }

## create_dataset.dataset_handle <- function(x, m) {
##   createDataset(x, m)
## }
