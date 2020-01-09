get_file_handle <- function(x, mode = "a") {
  if (inherits(x, "character")) {
    if (length(x) != 1) stop("length(x) must be 1.")
    structure(getFileHandle(x, mode), class = "file_handle")
  } else {
    stop("x must be a character vector.")
  }
}

create_file <- function(x) {
  if (inherits(x, "file_handle")) {
    FileHandleCreate(x)
  } else {
    stop("cannot create file from ", class(x))
  }
}

get_group_handle <- function(x, key) {
  if (inherits(x, "file_handle")) {
    structure(getGroupHandleFileHandle(x, key), class = "group_handle")
  } else if (inherits(x, "group_handle")) {
    structure(getGroupHandleGroupHandle(x, key), class = "group_handle")
  } else {
    stop("cannot get group handle from ", class(x))
  }
}

create_group <- function(x) {
  if (inherits(x, "group_handle")) {
    GroupHandleCreate(x)
  } else {
    stop("x must be of class \"group_handle\"")
  }
}

get_dataset_handle <- function(x, key) {
  if (inherits(x, "file_handle")) {
    structure(getDatasetHandleFileHandle(x, key), class = "dataset_handle")
  } else if (inherits(x, "group_handle")) {
    structure(getDatasetHandleGroupHandle(x, key), class = "dataset_handle")
  } else {
    stop("cannot get dataset handle from ", class(x))
  }
}

### This is being handled by the main create_dataset function:

## create_dataset <- function(x) {
##   if (inherits(x, "dataset_handle")) {
##     DatasetHandleCreate(x)
##   } else {
##     stop("x must be of class \"dataset_handle\"")
##   }
## }
