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
    default: { Rf_error("Error: Type of R array (Logical) and zarr array do not match."); }
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

  // case RAWSXP: {
  //   // NOTE: not checking bounds may lead to memory leaks
  //   xt::rarray<uint8_t> data_(data);
  //   check_bounds(offset, data_, *ds);

  //   switch (target_type) {
  //   // case z5::types::int8:  { z5::multiarray::writeSubarray<uint8_t>(*ds, data_, offset_.begin()); };
  //   // case z5::types::int8:    {write_subarray_transform<int8_t, int8_t>(*ds, data_, offset_);};
  //   // case z5::types::int16:   {write_subarray_transform<int16_t, int16_t>(*ds, data_, offset_);};
  //   // case z5::types::int32:   {write_subarray_transform<int32_t, int32_t>(*ds, data_, offset_);};
  //   // case z5::types::int64:   { write_subarray_transform<int64_t, int64_t>(*ds, data_, offset_); };
  //   case z5::types::uint8: {
  //     z5::multiarray::writeSubarray<uint8_t>(*ds, data_, offset_.begin());
  //     break;
  //   };
  //   // case z5::types::uint16:  { write_subarray_transform<uint16_t, uint16_t> (*ds, data_, offset_); };
  //   // case z5::types::uint32:  {write_subarray_transform<uint32_t, uint32_t> (*ds, data_, offset_); };
  //   // case z5::types::uint64:  { write_subarray_transform<uint64_t, uint64_t> (*ds, data_, offset_); };
  //   // case z5::types::float32: {write_subarray_transform<float, float>       (*ds, data_, offset_); };
  //   // case z5::types::float64: { write_subarray_transform_na<double, double>(*ds, data_, offset_); };
  //   default: { Rf_error("Error: Type of R array (Raw) and zarr array do not match."); }
  //   }
  //   break;
  // };

  default: {
    // Rf_error("Value must be Logical, Integer, Real, or Raw.");
    Rf_error("Value must be Logical, Integer, or Real.");
  };
  }
}
