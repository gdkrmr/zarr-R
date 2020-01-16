create_dataset_metadata <- function(shape,
                                    chunk_shape,
                                    data_type = "float64",
                                    fill_value = "auto",
                                    create_as_zarr = TRUE,
                                    compressor = "raw",
                                    compression_options = list()) {
  if (fill_value == "auto") {
    fill_value <- type_to_auto_fill_value[[data_type]]
    if (is.null(fill_value)) { stop("unknown data_type") }
  }

  res <- createDatasetMetadata(data_type,
                               shape,
                               chunk_shape,
                               create_as_zarr,
                               compressor,
                               compression_options,
                               fill_value)
  class(res) <- "dataset_metadata"
  return(res)
}

get_dataset_metadata <- function(x) {
  if (inherits(x, "zarr_dataset")) {
    res <- getDatasetMetadataDataset(x)
  } else {
    stop("x must be of class \"zarr_dataset\"")
  }

  class(res) <- "dataset_metadata"

  return(res)
}

#' @export
as.list.dataset_metadata <- function(x) {
  MetadataToList(x)
}

as.dataset_metadata <- function(x) {
  if(inherits(x, "list")) {
    res <- ListToMetadata(x)
  } else {
    stop("x must be of class \"list\"")
  }

  class(res) <- "dataset_metadata"

  return(res)
}

