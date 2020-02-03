#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

// TODO: handle missing values correctly

// [[Rcpp::plugins(cpp14)]]
// [[Rcpp::depends(xtensor)]]

#include "helpers.h"
#include <Rcpp.h>
// #include <xtensor-r/roptional.hpp>
#include <xtensor-r/rarray.hpp>
#include <xtensor/xtensor.hpp>
#include <z5/multiarray/xtensor_access.hxx>
#include <z5/types/types.hxx>


void check_bounds(const Rcpp::IntegerVector& offset,
                  const xt::rarray<double>& subarray,
                  const z5::Dataset& dataset) {

  if (offset.size() != subarray.dimension() ||
      offset.size() != dataset.shape().size()) {
    Rf_error("iterators must be of the same length");
  }

  for (size_t i = 0; i < offset.size(); i++) {
    if (offset[i] + subarray.shape()[i] > dataset.shape()[i] || offset[i] < 0) {
      Rf_error("out of bounds error");
    }
  }
}

template <typename TO_T, typename INNER_FROM_T, typename FROM_T>
void transform_write(z5::Dataset &out_data, xt::rarray<FROM_T> &in_data,
                            z5::types::ShapeType &offset) {
  std::vector<std::size_t> in_shape;
  for (auto &s : in_data.shape()) {
    in_shape.push_back(s);
  }
  xt::xarray<TO_T> middle_data(in_shape);
  std::transform(in_data.begin(), in_data.end(), middle_data.begin(),
                 [](const INNER_FROM_T x) {
                   if (std::numeric_limits<TO_T>::min() <= x &&
                       x <= std::numeric_limits<TO_T>::max()) {
                     return static_cast<TO_T>(x);
                   }

                   Rf_error("Cannot convert to target value, out of range.");
                 });
  z5::multiarray::writeSubarray<TO_T>(out_data, middle_data, offset.begin());
}

// [[Rcpp::export]]
void writeSubarray(const Rcpp::XPtr<z5::Dataset> &ds, const SEXP data,
                   const Rcpp::IntegerVector &offset) {

  auto target_type = ds->getDtype();
  auto source_type = (TYPEOF)(data);
  auto source_dim = Rcpp::as<std::vector<size_t>>(Rf_getAttrib(data, R_DimSymbol));
  auto offset_ = intvec_to_shapetype(offset);

  switch (source_type) {
  case LGLSXP: {
    // NOTE: not checking bounds may lead to memory leaks
    xt::rarray<rlogical> data_(data);
    check_bounds(offset, data_, *ds);

    switch (target_type) {
    // case z5::types::int8:    { transform_write<  int8_t, rlogical>(*ds, data_, offset_); break; };
    // case z5::types::int16:   { transform_write< int16_t, rlogical>(*ds, data_, offset_); break; };
    // case z5::types::int32:   { z5::multiarray::writeSubarray<rlogical>(*ds, data_, offset_.begin()); break; };
    // case z5::types::int64:   { transform_write< int64_t, rlogical>(*ds, data_, offset_); break; };
    // case z5::types::uint16:  { transform_write<uint8_t,  rlogical>(*ds, data_, offset_); break;};
    // case z5::types::uint16:  { transform_write<uint16_t, rlogical>(*ds, data_, offset_); break;};
    // case z5::types::uint32:  { transform_write<uint32_t, rlogical>(*ds, data_, offset_); break; };
    // case z5::types::uint64:  { transform_write<uint64_t, rlogical>(*ds, data_, offset_); break; };
    // case z5::types::float32: { transform_write<   float, rlogical>(*ds, data_, offset_); break; };
    // case z5::types::float64: { transform_write<  double, rlogical>(*ds, data_, offset_); break; };
    // default: { Rf_error("Error: Type of R array (Logical) and zarr array do not match."); }
    default: { Rf_error("Rlogicals currently not supportet!"); }
    }
    break;
  };

  case INTSXP: {
    // NOTE: not checking bounds may lead to memory leaks
    xt::rarray<int32_t> data_(data);
    check_bounds(offset, data_, *ds);

    switch (target_type) {
    case z5::types::int8:    { transform_write<  int8_t, int32_t>(*ds, data_, offset_); break; };
    case z5::types::int16:   { transform_write< int16_t, int32_t>(*ds, data_, offset_); break; };
    case z5::types::int32:   { z5::multiarray::writeSubarray<int32_t>(*ds, data_, offset_.begin()); break;};
    case z5::types::int64:   { transform_write< int64_t, int32_t>(*ds, data_, offset_); break; };
    case z5::types::uint8:   { transform_write< uint8_t, int32_t>(*ds, data_, offset_); break; };
    case z5::types::uint16:  { transform_write<uint16_t, int32_t>(*ds, data_, offset_); break; };
    case z5::types::uint32:  { transform_write<uint32_t, int32_t>(*ds, data_, offset_); break; };
    case z5::types::uint64:  { transform_write<uint64_t, int32_t>(*ds, data_, offset_); break; };
    case z5::types::float32: { transform_write<   float, int32_t>(*ds, data_, offset_); break; };
    case z5::types::float64: { transform_write<  double, int32_t>(*ds, data_, offset_); break; };
    default: { Rf_error("Error: Type of R array (Integer) and zarr array do not match."); }
    }
    break;
  };

  case REALSXP: {
    // NOTE: not checking bounds may lead to memory leaks
    xt::rarray<double> data_(data);
    check_bounds(offset, data_, *ds);

    switch (target_type) {
    case z5::types::int8:    { transform_write<  int8_t, double>(*ds, data_, offset_); break; };
    case z5::types::int16:   { transform_write< int16_t, double>(*ds, data_, offset_); break; };
    case z5::types::int32:   { transform_write< int32_t, double>(*ds, data_, offset_); break; };
    case z5::types::int64:   { transform_write< int64_t, double>(*ds, data_, offset_); break; };
    case z5::types::uint8:   { transform_write< uint8_t, double>(*ds, data_, offset_); break; };
    case z5::types::uint16:  { transform_write<uint16_t, double>(*ds, data_, offset_); break; };
    case z5::types::uint32:  { transform_write<uint32_t, double>(*ds, data_, offset_); break; };
    case z5::types::uint64:  { transform_write<uint64_t, double>(*ds, data_, offset_); break; };
    case z5::types::float32: { transform_write<   float, double>(*ds, data_, offset_); break; };
    case z5::types::float64: { z5::multiarray::writeSubarray<double>(*ds, data_, offset_.begin()); break;};
    default: { Rf_error("Error: Type of R array (Real) and zarr array do not match."); }
    }
    break;
  };

  case RAWSXP: {
    // NOTE: not checking bounds may lead to memory leaks
    xt::rarray<uint8_t> data_(data);
    check_bounds(offset, data_, *ds);

    switch (target_type) {
    case z5::types::int8:    { transform_write<  int8_t, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::int16:   { transform_write< int16_t, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::int32:   { transform_write< int32_t, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::int64:   { transform_write< int64_t, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::uint8:   { z5::multiarray::writeSubarray<uint8_t>(*ds, data_, offset_.begin()); break; };
    case z5::types::uint16:  { transform_write<uint16_t, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::uint32:  { transform_write<uint32_t, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::uint64:  { transform_write<uint64_t, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::float32: { transform_write<   float, uint8_t>(*ds, data_, offset_); break; };
    case z5::types::float64: { transform_write<  double, uint8_t>(*ds, data_, offset_); break; };
    default: { Rf_error("Error: Type of R array (Raw) and zarr array do not match."); }
    }
    break;
  };

  default: {
    Rf_error("Value must be Logical, Integer, Real, or Raw.");
    // Rf_error("Value must be Logical, Integer, or Real.");
  };
  }
}
