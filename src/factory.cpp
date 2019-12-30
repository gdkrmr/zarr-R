#include "helpers.h"
#include "json.h"
#include <z5/filesystem/dataset.hxx>
#include <z5/filesystem/factory.hxx>
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

// [[Rcpp::export]]
int DatasetDimension(Rcpp::XPtr<z5::Dataset> d) {
  return static_cast<int>(d->dimension());
}

// [[Rcpp::export]]
Rcpp::IntegerVector DatasetShape(Rcpp::XPtr<z5::Dataset> d) {
  return shapetype_to_intvec(d->shape());
}

// [[Rcpp::export]]
int DatasetSize(const Rcpp::XPtr<z5::Dataset> d) {
  return static_cast<int>(d->size());
}

// [[Rcpp::export]]
std::string DatasetPath(const Rcpp::XPtr<z5::Dataset> d) {
  return std::string(d->path());
}

template <typename T1, typename T2>
inline SEXP get_fill_value(const z5::Dataset &d) {
  T1 fv;
  d.getFillValue(&fv);
  T2 fv2 = static_cast<T2>(fv);
  return Rcpp::wrap(fv2);
}

// [[Rcpp::export]]
SEXP DatasetGetFillValue(const Rcpp::XPtr<z5::Dataset> d) {
  switch(d->getDtype()) {
  case z5::types::Datatype::uint8:   return get_fill_value<uint8_t, int>(*d);
  case z5::types::Datatype::uint16:  return get_fill_value<uint16_t, int>(*d);
  case z5::types::Datatype::uint32:  return get_fill_value<uint32_t, int>(*d);
  case z5::types::Datatype::uint64:  return get_fill_value<uint64_t, int>(*d);
  case z5::types::Datatype::int8:    return get_fill_value<int8_t, int>(*d);
  case z5::types::Datatype::int16:   return get_fill_value<int16_t, int>(*d);
  case z5::types::Datatype::int32:   return get_fill_value<int32_t, int>(*d);
  case z5::types::Datatype::int64:   return get_fill_value<int64_t, int>(*d);
  case z5::types::Datatype::float32: return get_fill_value<float, double>(*d);
  case z5::types::Datatype::float64: return get_fill_value<double, double>(*d);
  default: Rf_error("wrong data type");
  }
}

// [[Rcpp::export]]
std::string DatasetGetCompressor(const Rcpp::XPtr<z5::Dataset> d) {
  std::string s;
  d->getCompressor(s);
  return s;
}

// [[Rcpp::export]]
std::string DatasetGetFileMode(const Rcpp::XPtr<z5::Dataset> d) {
  return filemode_to_rfilemode(d->mode());
}
