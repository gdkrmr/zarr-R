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
SEXP readSubarray(const Rcpp::XPtr<z5::Dataset> ds,
                  const Rcpp::IntegerVector& offset,
                  const Rcpp::IntegerVector& shape) {
  // NOTE: not checking bounds may lead to memory leaks
  check_bounds(offset, shape, *ds);

  auto offset_ = intvec_to_shapetype(offset);
  auto shape_ = intvec_to_shapetype(shape);
  auto dtype = ds->getDtype();
  SEXP res;

  switch (dtype) {

  case z5::types::int8: {
    xt::xarray<int8_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<int8_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [](const int8_t x) { return static_cast<int32_t>(x); });

    res = wrap(out_array);
    break;
  };

  case z5::types::int16: {
    xt::xarray<int16_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<int16_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [](const int16_t x) { return static_cast<int32_t>(x); });
    res = wrap(out_array);
    break;
  };

  case z5::types::int32: {
    xt::rarray<int32_t> array(shape_);

    z5::multiarray::readSubarray<int32_t>(*ds, array, offset_.begin());

    res = wrap(array);
    break;
  };

  case z5::types::int64: {
    // Rf_warning("WARNING: reading 64 bit integers, R cannot deal with this data "
    //            "type and you may loose information!");
    xt::xarray<int64_t> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<int64_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [](const int64_t x) { return static_cast<double>(x); });

    res = wrap(out_array);
    break;
  };

  case z5::types::uint8: {
    xt::rarray<uint8_t> array(shape_);
    z5::multiarray::readSubarray<uint8_t>(*ds, array, offset_.begin());
    res = wrap(array);
    break;
  };

  case z5::types::uint16: {
    xt::xarray<uint16_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<uint16_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [](const uint16_t x) { return static_cast<int32_t>(x); });
    res = wrap(out_array);
    break;
  };

  case z5::types::uint32: {
    xt::xarray<uint32_t> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<uint32_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [](const uint32_t x) { return static_cast<double>(x); });
    res = wrap(out_array);
    break;
  };

  case z5::types::uint64: {
    Rf_warning("WARNING: reading 64 bit unsigned integers, R cannot deal with "
               "this data type and you may loose information!");
    xt::xarray<uint64_t> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<uint64_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [](const uint64_t x) { return static_cast<double>(x); });

    res = wrap(out_array);
    break;
  };

  case z5::types::float32: {
    xt::xarray<float> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<float>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [](const float x) { return static_cast<double>(x); });

    res = wrap(out_array);
    break;
  };

  case z5::types::float64: {
    xt::rarray<double> array(shape_);
    z5::multiarray::readSubarray<double>(*ds, array, offset_.begin());
    res = wrap(array);
    break;
  };

  default: { Rf_error("Wrong data type."); }
  }

  return res;
}
