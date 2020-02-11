#include "helpers.h"

#include <Rcpp.h>
#include <z5/dataset.hxx>
#include <z5/types/types.hxx>

std::string filemode_to_rfilemode(const z5::FileMode m) {
  return m.printMode();
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
  return d->path().string();
}

// [[Rcpp::export]]
SEXP DatasetGetFillValue(const Rcpp::XPtr<z5::Dataset> d) {
  switch(d->getDtype()) {
  case z5::types::Datatype::uint8:   return Rcpp::wrap(get_fill_value<uint8_t,   int>(*d));
  case z5::types::Datatype::uint16:  return Rcpp::wrap(get_fill_value<uint16_t,  int>(*d));
  case z5::types::Datatype::uint32:  return Rcpp::wrap(get_fill_value<uint32_t,  int>(*d));
  case z5::types::Datatype::uint64:  return Rcpp::wrap(get_fill_value<uint64_t,  int>(*d));
  case z5::types::Datatype::int8:    return Rcpp::wrap(get_fill_value<int8_t,    int>(*d));
  case z5::types::Datatype::int16:   return Rcpp::wrap(get_fill_value<int16_t,   int>(*d));
  case z5::types::Datatype::int32:   return Rcpp::wrap(get_fill_value<int32_t,   int>(*d));
  case z5::types::Datatype::int64:   return Rcpp::wrap(get_fill_value<int64_t,   int>(*d));
  case z5::types::Datatype::float32: return Rcpp::wrap(get_fill_value<float,  double>(*d));
  case z5::types::Datatype::float64: return Rcpp::wrap(get_fill_value<double, double>(*d));
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

// [[Rcpp::export]]
int DatasetGetDefaultChunkSize(const Rcpp::XPtr<z5::Dataset> d) {
  return static_cast<int>(d->defaultChunkSize());
}

// [[Rcpp::export]]
Rcpp::IntegerVector DatasetGetDefaultChunkShape(const Rcpp::XPtr<z5::Dataset> d) {
  return shapetype_to_intvec(d->defaultChunkShape());
}

// [[Rcpp::export]]
void DatasetRemove(const Rcpp::XPtr<z5::Dataset> d) {
  d->remove();
}
