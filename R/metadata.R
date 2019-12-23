create_dataset_metadata <- function(shape,
                                    chunk_shape,
                                    data_type = "float64",
                                    fill_value = "auto",
                                    create_as_zarr = TRUE,
                                    compressor = "raw",
                                    compression_options = list()) {
  if (fill_value == "auto") {
    if      (data_type == "int8")    { fill_value <- 0L          }
    else if (data_type == "int16")   { fill_value <- 0L          }
    else if (data_type == "int32")   { fill_value <- NA_integer_ }
    else if (data_type == "int64")   { fill_value <- 0L          }
    else if (data_type == "uint8")   { fill_value <- 0L          }
    else if (data_type == "uint16")  { fill_value <- 0L          }
    else if (data_type == "uint32")  { fill_value <- 0L          }
    else if (data_type == "uint64")  { fill_value <- 0L          }
    else if (data_type == "float32") { fill_value <- NaN         }
    else if (data_type == "float64") { fill_value <- NA_real_    }
    else stop("wrong data_type!")
  }
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
