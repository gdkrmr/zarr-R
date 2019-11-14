#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

// [[Rcpp::plugins(cpp14)]]
// [[Rcpp::depends(xtensor)]]

#include "helpers.h"
#include <Rcpp.h>
#include <xtensor-r/rarray.hpp>
#include <z5/multiarray/xtensor_access.hxx>
#include <iostream>

void check_bounds(const Rcpp::IntegerVector& offset,
                         const Rcpp::IntegerVector& subarray_shape,
                         const z5::Dataset& dataset) {

  std::cout << "offset: " << offset.size() <<
    "subarray_shape: " << subarray_shape.size() <<
    "dataset_shape: " << dataset.shape().size() << std::endl;

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
xt::rarray<double> readSubarray(const Rcpp::XPtr<z5::Dataset> ds,
                                const Rcpp::IntegerVector & offset,
                                const Rcpp::IntegerVector & shape) {
  // NOTE: not checking bounds may lead to memory leaks
  check_bounds(offset, shape, *ds);

  auto offset_ = intvec_to_shapetype(offset);
  auto shape_ = intvec_to_shapetype(shape);
  xt::rarray<double> array(shape_);


  z5::multiarray::readSubarray<double>(*ds, array, offset_.begin());

  return array;
}


// [[Rcpp::export]]
void writeSubarray(const Rcpp::XPtr<z5::Dataset> ds,
                   const xt::rarray<double> data,
                   const Rcpp::IntegerVector & offset) {
  std::cout << "data.size: " << data.size() << std::endl;

  // NOTE: not checking bounds may lead to memory leaks
  check_bounds(offset, data , *ds);

  auto offset_ = intvec_to_shapetype(offset);
  z5::multiarray::writeSubarray<double>(*ds, data, offset_.begin());
}
