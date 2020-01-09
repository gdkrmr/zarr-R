#include "helpers.h"
#include "json.h"
#include "z5/metadata.hxx"
#include <z5/filesystem/dataset.hxx>
#include <z5/filesystem/factory.hxx>
#include <z5/dataset.hxx>
#include <z5/filesystem/handle.hxx>
#include <Rcpp.h>
#include <z5/factory.hxx>
#include <z5/types/types.hxx>

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
openDatasetDatasetHandle(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  // NOTE: this one does not work for z5::openDatabase
  auto dptr = z5::filesystem::openDataset(*d);
  return Rcpp::XPtr<z5::Dataset>(dptr.release(), true);
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
openDatasetFileHandle(const Rcpp::XPtr<z5::filesystem::handle::File> f,
                const std::string &key) {
  auto dptr = z5::openDataset(*f, key);
  return Rcpp::XPtr<z5::Dataset>(dptr.release(), true);
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
openDatasetGroupHandle(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
                       const std::string &key) {
  auto dptr = z5::openDataset(*g, key);
  return Rcpp::XPtr<z5::Dataset>(dptr.release(), true);
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
createDatasetFileHandle(const Rcpp::XPtr<z5::filesystem::handle::File> f,
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
  return Rcpp::XPtr<z5::Dataset>(d.release(), true);
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
createDatasetGroupHandle(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
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
  return Rcpp::XPtr<z5::Dataset>(d.release(), true);
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset>
createDatasetDatasetHandle(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d,
                           const std::string &dtype,
                           const Rcpp::IntegerVector &shape,
                           const Rcpp::IntegerVector &chunkShape,
                           const std::string &compressor,
                           const Rcpp::List &compressionOptions,
                           const bool isZarr,
                           const double fillValue) {
  auto shape_ = intvec_to_shapetype(shape);
  auto chunkShape_ = intvec_to_shapetype(chunkShape);

  auto compressionOptions_json = rlist_to_json(compressionOptions);
  z5::types::CompressionOptions compressionOptions_z5;
  auto compressor_ = string_to_compressor(compressor);

  z5::types::readZarrCompressionOptionsFromJson(
      compressor_, compressionOptions_json, compressionOptions_z5);

  z5::DatasetMetadata m;
  z5::createDatasetMetadata(dtype, shape_, chunkShape_, isZarr, compressor,
                            compressionOptions_z5, fillValue, m);

  auto d2 = z5::filesystem::createDataset(*d, m);
  return Rcpp::XPtr<z5::Dataset>(d2.release(), true);
}
