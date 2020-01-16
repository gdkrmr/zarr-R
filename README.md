# Zarr for R

[![Build Status](https://travis-ci.org/gdkrmr/zarr-R.svg?branch=master)](https://travis-ci.org/gdkrmr/zarr-R)


**NOT WORKING YET**, don't complain :-), help is welcome!


# Interface:

The final interface is supposed to look like this:

```R
library(zarr)

path <- "/path/to/the/data/set"
key <- "group/dataset"
shape <- c(9, 9, 9)
chunk_shape <- c(3, 3, 3)
dtype <- "double"

d <- create_dataset(path, key, shape = shape, chunk_shape = chunk_shape, dtype = dtype)
```

for opening:

```R
d <- open_dataset(path, key)
```

accessing the dataset should behave like a normal array:

```R
d[1, 2, 3]
d[ , 2, 2]
d[1, 1, 1] <- 3
```
