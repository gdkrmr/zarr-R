create_dataset_metadata <- function(shape,
                                    chunk_shape,
                                    data_type = "float64",
                                    fill_value = "auto",
                                    create_as_zarr = TRUE,
                                    compressor = "raw",
                                    compression_options = list()) {
  structure(
    createDatasetMetadata(data_type,
                          shape,
                          chunk_shape,
                          create_as_zarr,
                          compressor,
                          compression_options,
                          fill_value),
    class = "dataset_metadata_handle"
  )
}

as.list.dataset_metadata_handle <- function(x) {
  MetadataToList(x)
}

as.dataset_metadata_handle <- function(x) {
  structure(ListToMetadata(x),
            class = "dataset_metadata_handle")
}
