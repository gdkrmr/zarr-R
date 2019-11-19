#include "helpers.h"
#include "json.h"
#include <Rcpp.h>
#include <string>
#include <z5/dataset.hxx>
#include <z5/dataset_factory.hxx>

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset> openDataset(const std::string & path,
                                    const std::string & file_mode) {
  const auto mode = rfilemode_to_filemode(file_mode);
  auto d = z5::openDataset(path, mode);

  Rcpp::XPtr<z5::Dataset> ptr(d.release());

  return ptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::Dataset> createDataset(const std::string & path,
                                      const std::string & dtype,
                                      const Rcpp::IntegerVector & shape,
                                      const Rcpp::IntegerVector & chunkShape,
                                      const bool createAsZarr,
                                      const std::string & compressor,
                                      const Rcpp::List & compressionOptions,
                                      const double fillValue,
                                      const std::string file_mode) {

  auto mode_ = rfilemode_to_filemode(file_mode);
  auto shape_ = intvec_to_shapetype(shape);
  auto chunkShape_ = intvec_to_shapetype(chunkShape);
  auto compressionOptions_json = rlist_to_json(compressionOptions);
  z5::types::CompressionOptions compressionOptions_z5;
  auto compressor_ = string_to_compressor(compressor);

  z5::types::readZarrCompressionOptionsFromJson(compressor_,
                                                compressionOptions_json,
                                                compressionOptions_z5);

  auto d = z5::createDataset(path, dtype,
                             shape_, chunkShape_,
                             createAsZarr,
                             compressor, compressionOptions_z5,
                             fillValue, mode_);

  Rcpp::XPtr<z5::Dataset> ptr(d.release());
  return ptr;
}
