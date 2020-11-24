if(FALSE) {
## devtools::load_all(".")

path <- tempfile()
shape <- c(9, 9)
chunk_shape <- c(3, 3)
fill_value <- NA_real_
data_type <- c("float64")
test_data <- array(runif(prod(shape)) * 100, dim = shape)

f <- zarr:::get_file_handle(path)
g <- zarr:::get_group_handle(f, "group1")
d <- zarr:::get_dataset_handle(f, "dataset3")

dh <- zarr::create_dataset(d, shape = shape, chunk_shape = chunk_shape, compressor = "blosc")
dh <- zarr::create_dataset(f, key = "dataset5", shape = shape, chunk_shape = chunk_shape, compressor = "blosc")
dh <- zarr::create_dataset(g, key = "dataset2", shape = shape, chunk_shape = chunk_shape, compressor = "blosc")

  dim(dh)

  dh[1,1] <- NA_real_
  dh[,]

  dh[,2L] <- array(4, dim = c(9))
  dh[,2L, drop = TRUE]

  dh[,]  <- array(2, dim = c(9,9))

  for (i in 1:9)
    for (j in 1:9)
      dh[i, j] <- 2




zarr:::MetadataToList(zarr:::create_dataset_metadata(shape, chunk_shape, compressor = "blosc"))
zarr:::MetadataToList(zarr:::getDatasetMetadataDataset(dh1))

zarr:::create_dataset_metadata(shape, chunk_shape, compressor = "zlib")


zarr:::DatasetRemove(d)
zarr:::FileHandleDelete(f)
zarr:::GroupHandleDelete(g)

zarr:::DatasetRemove(dh)

zarr:::list_to_json(list(compressoj = "raw"))

l <- list(compressor = "")
zarr:::ListToMetadata(list(compressor = "raw"))

is.null(list()[["sdf"]])

  ## devtools::install_github("gdkrmr/zarr-R")

  ds <- zarr::open_dataset("/run/user/1001/gvfs/sftp:host=atacama/scratch/gkraemer/fftpca/results/cfft_pca.cube", "layer", mode = "r")

  ds <- zarr::open_dataset("/run/user/1001/gvfs/sftp:host=atacama/scratch/DataCube/v2.0.0/",
                           "esdc-8d-0.25deg-184x90x90-2.0.0.zarr/air_temperature_2m",
                           mode = "r")


  dim(ds)

  184 * 2160 * 4320 * 8 / 1024 / 1024

  zarr:::MetadataToList(zarr:::getDatasetMetadataDataset(ds))

  res <- ds[1105:1288,,]

  which(ds[,300,300] > 0)

  res[res == -9999] <- NA

  921 / 184
  7*184

  image(res[103,,])

  ds[1, 1, 1]
  dim(res)

  1:9 + 9

  array(1:9, dim = c(3, 3))[1, , drop = FALSE]

  # TODO: stuff like this needs to work:
  ds[1:(184 + 184 * 1), 1:(270 + 270 * 1), 1:(270 + 270 * 1)]
}
