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
#'
#' @export
read_attributes <- function(x) {
  res <- if (inherits(x, "file_handle")) {
    readAttributesFileHandle(x)
  } else if (inherits(x, "group_handle")) {
    readAttributesGroupHandle(x)
  } else if (inherits(x, "dataset_handle")) {
    readAttributesDatasetHandle(x)
  ## } else if (inherits(x, "zarr_dataset")) {
  ##   readAttributesDataset(x)
  } else {
    stop("cannot use class(x) ", class(x))
  }
  class(res) <- "zarr_attributes"

  return(res)
}


#' Write attributes
#'
#' Write the attributes of an object.
#'
#' @param x The object
#' @param attributes A list of attributes
#' @export
write_attributes <- function(x, attributes) {
  if (inherits(x, "file_handle")) {
    writeAttributesFileHandle(x, attributes)
  } else if (inherits(x, "group_handle")) {
    writeAttributesGroupHandle(x, attributes)
  } else if (inherits(x, "dataset_handle")) {
    writeAttributesDatasetHandle(x, attributes)
  } else {
    stop("cannot use class(x) ", class(x))
  }
}

#' Remove attributes
#'
#' Remove attributes from an object.
#'
#' @param x The object
#' @param attributes the attribute to removeAttribute
#'
#' @export
remove_attributes <- function(x, attributes) {
  if (inherits(x, "file_handle")) {
    for (a in attributes) { removeAttributeFileHandle(x, a) }
  } else if (inherits(x, "group_handle")) {
    for (a in attributes) { removeAttributeGroupHandle(x, a) }
  } else if (inherits(x, "dataset_handle")) {
    for (a in attributes) { removeAttributeDatasetHandle(x, a) }
  } else {
    stop("cannot use class(x) ", class(x))
  }
}
