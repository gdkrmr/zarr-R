#include "helpers.h"
#include "json.h"
#include <Rcpp.h>
#include <z5/metadata.hxx>

// [[Rcpp::export]]
Rcpp::XPtr<z5::DatasetMetadata> ListToMetadata(const Rcpp::List& l) {
  nlohmann::json j = rlist_to_json(l);

  z5::DatasetMetadata* m;
  m->fromJson(j, true);

  Rcpp::XPtr<z5::DatasetMetadata> mptr(m);
  return mptr;
}

// [[Rcpp::export]]
Rcpp::List MetadataToList(Rcpp::XPtr<z5::DatasetMetadata> mptr) {
  nlohmann::json j;
  mptr->toJson(j);
  Rcpp::List l = json_to_rlist(j);
  return l;
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::DatasetMetadata>
createDatasetMetadata(const std::string &dtype,
                      const Rcpp::IntegerVector &shape,
                      const Rcpp::IntegerVector &chunkShape,
                      const bool createAsZarr,
                      const std::string &compressor,
                      const Rcpp::List &compressionOptions,
                      const double fillValue) {
  auto shape_ = intvec_to_shapetype(shape);
  auto chunkShape_ = intvec_to_shapetype(chunkShape);

  auto compressionOptions_json = rlist_to_json(compressionOptions);
  z5::types::CompressionOptions compressionOptions_z5;
  auto compressor_ = string_to_compressor(compressor);

  z5::types::readZarrCompressionOptionsFromJson(compressor_,
                                                compressionOptions_json,
                                                compressionOptions_z5);

  z5::DatasetMetadata m;
  z5::createDatasetMetadata(dtype, shape_, chunkShape_, createAsZarr,
                            compressor, compressionOptions_z5, fillValue,
                            m);

  Rcpp::XPtr<z5::DatasetMetadata> mptr(&m);
  return mptr;
}
