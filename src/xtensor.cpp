#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

// [[Rcpp::plugins(cpp14)]]
// [[Rcpp::depends(xtensor)]]

#include "helpers.hxx"
#include <Rcpp.h>
#include <xtensor-r/rarray.hpp>
#include <xtensor/xexpression.hpp>
#include <z5/multiarray/xtensor_access.hxx>

// [[Rcpp::export]]
xt::rarray<double> readSubarray(const Rcpp::XPtr<z5::Dataset> ds,
                                const Rcpp::IntegerVector & offset,
                                const Rcpp::IntegerVector & shape) {
  auto offset_ = intvec_to_shapetype(offset);
  auto shape_ = intvec_to_shapetype(shape);
  xt::rarray<double> array(shape_);

  z5::multiarray::readSubarray<double>(*ds, array, offset_.begin());

  return array;
}


// [[Rcpp::export]]
xt::rarray<double> writeSubarray(const Rcpp::XPtr<z5::Dataset> ds,
                                 const xt::rarray<double> data,
                                 const Rcpp::IntegerVector & offset) {
  auto offset_ = intvec_to_shapetype(offset);

  z5::multiarray::writeSubarray<double>(*ds, data, offset_.begin());

  // TODO: is this necessary for zarr[1:10] <- 1:10 to return 1:10?
  return data;
}
