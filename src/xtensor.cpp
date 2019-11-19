#include "xtensor-r/rcontainer.hpp"
#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

// [[Rcpp::plugins(cpp14)]]
// [[Rcpp::depends(xtensor)]]

#include "helpers.h"
#include <Rcpp.h>
#include <xtensor-r/rarray.hpp>
#include <xtensor/xtensor.hpp>
#include <z5/multiarray/xtensor_access.hxx>
#include <iostream>

void check_bounds(const Rcpp::IntegerVector& offset,
                  const Rcpp::IntegerVector& subarray_shape,
                  const z5::Dataset& dataset) {

  if (offset.size() != subarray_shape.size() ||
      offset.size() != dataset.shape().size()) {
    Rf_error("iterators must be of the same length");
  }

  for (size_t i = 0; i < offset.size(); i++) {
    if (offset[i] + subarray_shape[i] > dataset.shape()[i] || offset[i] < 0) {
      Rf_error("out of bounds error");
    }
  }

}

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
    //   xt::rarray<int8_t> array(shape_);
    //   z5::multiarray::readSubarray<int8_t>(*ds, array, offset_.begin());
    //   res = wrap(array);
    Rf_error("data type int8 not supported.");
  };
  case z5::types::int16: {
    //   xt::rarray<int16_t> array(shape_);
    //   z5::multiarray::readSubarray<int16_t>(*ds, array, offset_.begin());
    //   res = wrap(array);
    Rf_error("data type int16 not supported.");
  };
  case z5::types::int32: {
    //   xt::rarray<int32_t> array(shape_);
    //   z5::multiarray::readSubarray<int32_t>(*ds, array, offset_.begin());
    //   res = wrap(array);
    Rf_error("data type int32 not supported.");
  };
  case z5::types::int64: {
    //   xt::rarray<int64_t> array(shape_);
    //   z5::multiarray::readSubarray<int64_t>(*ds, array, offset_.begin());
    //   res = wrap(array);
    Rf_error("data type uint16 not supported.");
  };
  case z5::types::uint8: {
    xt::rarray<rlogical> array(shape_);
    z5::multiarray::readSubarray<uint8_t>(*ds, array, offset_.begin());
    res = wrap(array);
  };
  case z5::types::uint16: {
    //   xt::rarray<uint16_t> array(shape_);
    //   z5::multiarray::readSubarray<uint16_t>(*ds, array, offset_.begin());
    //   res = wrap(array);
    Rf_error("data type uint16 not supported.");
  };
  case z5::types::uint32: {
    xt::rarray<int> array(shape_);
    z5::multiarray::readSubarray<int>(*ds, array, offset_.begin());
    res = wrap(array);
  };
  case z5::types::uint64: {
    //   xt::rarray<uint64_t> array(shape_);
    //   z5::multiarray::readSubarray<uint64_t>(*ds, array, offset_.begin());
    //   res = wrap(array);
    Rf_error("data type uint64 not supported.");
  };
  case z5::types::float32: {
    xt::xarray<float> array_(shape_);
    xt::rarray<double> array(shape_);
    z5::multiarray::readSubarray<float>(*ds, array_, offset_.begin());

    for (size_t i = 0; i < array_.size(); i++) {
      array.data()[i] = static_cast<double>(array_.data()[i]);
    }

    res = wrap(array);
  };
  case z5::types::float64: {
    xt::rarray<double> array(shape_);
    z5::multiarray::readSubarray<double>(*ds, array, offset_.begin());
    res = wrap(array);
  };
  }

  return res;
}

// [[Rcpp::export]]
void writeSubarray(const Rcpp::XPtr<z5::Dataset> ds,
                   const xt::rarray<double> data,
                   const Rcpp::IntegerVector & offset) {

  // xt::rcontainer con(data);
  // con.
  // data.r

  // NOTE: not checking bounds may lead to memory leaks
  check_bounds(offset, data , *ds);

  auto offset_ = intvec_to_shapetype(offset);

  z5::multiarray::writeSubarray<double>(*ds, data, offset_.begin());
}
