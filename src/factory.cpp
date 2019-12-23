#include "Rcpp/XPtr.h"
#include "helpers.h"
#include "json.h"
#include "z5/filesystem/dataset.hxx"
#include "z5/filesystem/factory.hxx"
#include <z5/dataset.hxx>
#include <z5/filesystem/handle.hxx>
#include <Rcpp.h>
#include <z5/factory.hxx>
#include <z5/types/types.hxx>

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
openDatasetDataset(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  auto dptr = z5::filesystem::openDataset(*d);
  return Rcpp::XPtr<z5::Dataset>(dptr.release());
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
openDatasetFile(const Rcpp::XPtr<z5::filesystem::handle::File> f,
                const std::string &key) {
  auto dptr = z5::openDataset(*f, key);
  return Rcpp::XPtr<z5::Dataset>(dptr.release());
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
openDatasetGroup(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
                 const std::string &key) {
  auto dptr = z5::openDataset(*g, key);
  return Rcpp::XPtr<z5::Dataset>(dptr.release());
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
createDatasetFile(const Rcpp::XPtr<z5::filesystem::handle::File> f,
                  const std::string &key,
                  const std::string &dtype,
                  const Rcpp::IntegerVector &shape,
                  const Rcpp::IntegerVector &chunkShape,
                  const std::string &compressor,
                  const Rcpp::List &compressionOptions,
                  const double fillValue) {
  auto d = z5::createDataset(*f, key, dtype, intvec_to_shapetype(shape),
                             intvec_to_shapetype(chunkShape), compressor,
                             rlist_to_json(compressionOptions), fillValue);
  return Rcpp::XPtr<z5::Dataset>(d.release());
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
createDatasetGroup(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
                   const std::string &key,
                   const std::string &dtype,
                   const Rcpp::IntegerVector &shape,
                   const Rcpp::IntegerVector &chunkShape,
                   const std::string &compressor,
                   const Rcpp::List &compressionOptions,
                   const double fillValue) {
  auto d = z5::createDataset(*g, key, dtype, intvec_to_shapetype(shape),
                             intvec_to_shapetype(chunkShape), compressor,
                             rlist_to_json(compressionOptions), fillValue);
  return Rcpp::XPtr<z5::Dataset>(d.release());
}

// [[Rcpp::export]]
bool DatasetIsZarr(Rcpp::XPtr<z5::Dataset> d) { return d->isZarr(); }

// [[Rcpp::export]]
std::string DatasetGetDtype(Rcpp::XPtr<z5::Dataset> d) {
  return datatype_to_string(d->getDtype());
}
