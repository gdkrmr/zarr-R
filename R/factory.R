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
  createGroup(x)
}


### create file ####################################

## create_file <- function(x) {
##   UseMethod("create_file")
## }

## create_file.default <- function(x) {
##   stop("cannot create file from ", class(x))
## }

create_file.file_handle <- function(x) {
  createFile(x)
}


### create dataset #################################

## create_dataset <- function(x, m) {
##   UseMethod("create_dataset")
## }

## create_dataset.default <- function(x, m) {
##   stop("cannot create dataset from ", class(x))
## }

create_dataset.dataset_handle <- function(x, m) {
  createDataset(x, m)
}
