#include "helpers.h"
#include "json.h"
#include "z5/filesystem/handle.hxx"
#include "z5/types/types.hxx"
#include <Rcpp.h>
#include <z5/metadata.hxx>

// [[Rcpp::export]]
Rcpp::XPtr<z5::DatasetMetadata> ListToMetadata(const Rcpp::List& l) {
  nlohmann::json j = rlist_to_json(l);

  if (j["compressor"] == "raw") {
    j["compressor"] = nullptr;
  }

  auto m = new z5::DatasetMetadata();
  m->fromJson(j, true);

  Rcpp::XPtr<z5::DatasetMetadata> mptr(m, true);
  return mptr;
}

// [[Rcpp::export]]
Rcpp::List MetadataToList(Rcpp::XPtr<z5::DatasetMetadata> mptr) {
  nlohmann::json j;

  mptr->toJson(j);
  if (j["compressor"].is_null()) {
    j["compressor"] = "raw";
  }

  return json_to_rlist(j);
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

  auto m = new z5::DatasetMetadata();
  z5::createDatasetMetadata(dtype, shape_, chunkShape_, createAsZarr,
                            compressor, compressionOptions_z5, fillValue,
                            *m);

  Rcpp::XPtr<z5::DatasetMetadata> mptr(m, true);
  return mptr;
}


// [[Rcpp::export]]
Rcpp::XPtr<z5::DatasetMetadata> getDatasetMetadataDataset(Rcpp::XPtr<z5::Dataset> d) {

  z5::types::CompressionOptions compressionOptions;
  d->getCompressionOptions(compressionOptions);

  z5::types::Datatype data_type = d->getDtype();
  double fill_value;
  switch (data_type) {
  case z5::types::Datatype::uint8:   { fill_value = get_fill_value<uint8_t,  double>(*d); break; };
  case z5::types::Datatype::uint16:  { fill_value = get_fill_value<uint16_t, double>(*d); break; };
  case z5::types::Datatype::uint32:  { fill_value = get_fill_value<uint32_t, double>(*d); break; };
  case z5::types::Datatype::uint64:  { fill_value = get_fill_value<uint64_t, double>(*d); break; };
  case z5::types::Datatype::int8:    { fill_value = get_fill_value<int8_t,   double>(*d); break; };
  case z5::types::Datatype::int16:   { fill_value = get_fill_value<int16_t,  double>(*d); break; };
  case z5::types::Datatype::int32:   { fill_value = get_fill_value<int32_t,  double>(*d); break; };
  case z5::types::Datatype::int64:   { fill_value = get_fill_value<int64_t,  double>(*d); break; };
  case z5::types::Datatype::float32: { fill_value = get_fill_value<float,    double>(*d); break; };
  case z5::types::Datatype::float64: { fill_value = get_fill_value<double,   double>(*d); break; };
  default: Rf_error("wrong data type");
  }

  auto m = new z5::DatasetMetadata(data_type, d->shape(), d->defaultChunkShape(),
                                   d->isZarr(), d->getCompressor(),
                                   compressionOptions, fill_value);

  Rcpp::XPtr<z5::DatasetMetadata> mptr(m, true);
  return mptr;
}
