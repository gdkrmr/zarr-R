#' @include metadata.R
NULL


### open dataset ###################################

# TODO: let go of S3 class dispatch?????

## open_dataset <- function(x, key, ...) {
##   UseMethod("open_dataset")
## }

## this one should work for all group/file handle types
open_dataset <- function(x, key) {
  if (inherits(x, "file_handle")) {
    structure(openDatasetFile(x, key), class = "dataset")
  } else if (inherits(x, "group_handle")) {
    structure(openDatasetGroup(x, key), class = "dataset")
  } else if (inherits(x, "dataset_handle")) {
    if (!missing(key)) warning("key provided but not used!")
    structure(openDatasetDataset(x))
  } else {
    stop("open_dataset doesn't work for ", class(x))
  }
}

## this one is for z5::filesystem::handle::dataset
## open_dataset.dataset_handle <- function(x) {
##   structure(openDatasetDataset(x),
##     class = "dataset"
##   )
## }

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
