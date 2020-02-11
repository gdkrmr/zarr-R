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
                  const Rcpp::IntegerVector& subarray_shape,
                  const z5::Dataset& dataset) {

  if (offset.size() != subarray_shape.size() ||
      offset.size() != dataset.shape().size()) {
    Rf_error("iterators must be of the same length");
  }

  for (size_t i = 0; i < offset.size(); i++) {
    if (offset[i] + subarray_shape[i] > dataset.shape()[i] || offset[i] < 0)
    {
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
    int8_t fill_value = get_fill_value<int8_t, int8_t>(*ds);
    xt::xarray<int8_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<int8_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const int16_t x) {
                     return x == fill_value ? na<int>() : static_cast<int32_t>(x);
                   });

    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::int16: {
    int16_t fill_value = get_fill_value<int16_t, int16_t>(*ds);
    xt::xarray<int16_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<int16_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const int16_t x) {
                     return x == fill_value ? na<int>() : static_cast<int32_t>(x);
                   });

    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::int32: {
    // xt::rarray<int32_t> array(shape_);

    // z5::multiarray::readSubarray<int32_t>(*ds, array, offset_.begin());

    // res = Rcpp::wrap(array);
    // break;
    int32_t fill_value = get_fill_value<int32_t, int32_t>(*ds);
    xt::xarray<int32_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<int32_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const int32_t x) {
                     return x == fill_value ? na<int>() : x;
                   });

    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::int64: {
    // Rf_warning("WARNING: reading 64 bit integers, R cannot deal with this data "
    //            "type and you may loose information!");
    int64_t fill_value = get_fill_value<int64_t, int64_t>(*ds);
    xt::xarray<int64_t> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<int64_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const int64_t x) {
                     if(x == fill_value) {
                       return na<double>();
                     } else {
                       auto val = static_cast<double>(x);
                       if(static_cast<int64_t>(val) != x) {
                         Rf_error("Inexact error when reading from uint32.");
                       }
                       return val;
                     }
                   });

    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::uint8: {
    uint8_t fill_value = get_fill_value<uint8_t, uint8_t>(*ds);
    xt::xarray<uint8_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<uint8_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const uint8_t x) {
                     if (x == fill_value) {
                       return na<int>();
                     } else {
                       return static_cast<int32_t>(x);
                     }
                   });

    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::uint16: {
    uint16_t fill_value = get_fill_value<uint16_t, uint16_t>(*ds);
    xt::xarray<uint16_t> middle_array(shape_);
    xt::rarray<int32_t> out_array(shape_);

    z5::multiarray::readSubarray<uint16_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const uint16_t x) {
                     if(x == fill_value) {
                       return na<int>();
                     } else {
                       return static_cast<int32_t>(x);
                     }
                   });
    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::uint32: {
    uint32_t fill_value = get_fill_value<uint32_t, uint32_t>(*ds);
    xt::xarray<uint32_t> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<uint32_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const uint32_t x) {
                     if(x == fill_value) {
                       return na<double>();
                     } else {
                       auto val = static_cast<double>(x);
                       if(static_cast<uint32_t>(val) != x) {
                         Rf_error("Inexact error when reading from uint32.");
                       }
                       return val;
                     }
                   });
    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::uint64: {
    // Rf_warning("WARNING: reading 64 bit unsigned integers, R cannot deal with "
    //            "this data type and you may loose information!");
    uint64_t fill_value = get_fill_value<uint64_t, uint64_t>(*ds);
    xt::xarray<uint64_t> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<uint64_t>(*ds, middle_array, offset_.begin());
    std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                   [fill_value](const uint64_t x) {
                     if(x == fill_value) {
                       return na<double>();
                     } else {
                       auto val = static_cast<double>(x);
                       if (static_cast<uint64_t>(val) != x) {
                         // TODO: allow transform to finish and put out a
                         // warning in the end.
                         Rf_error("Inexact error when reading from uint64.");
                       }
                       return val;
                     }
                   });

    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::float32: {
    float fill_value = get_fill_value<float, float>(*ds);
    xt::xarray<float> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<float>(*ds, middle_array, offset_.begin());
    if(std::isnan(fill_value)) {
      std::transform(middle_array.begin(), middle_array.end(),
                     out_array.begin(), [](const float x) {
                       return std::isnan(x) ? na<double>()
                                            : static_cast<double>(x);
                     });
    } else {
      std::transform(middle_array.begin(), middle_array.end(),
                     out_array.begin(), [fill_value](const float x) {
                       return x == fill_value ? na<double>()
                                              : static_cast<double>(x);
                     });
    }

    res = Rcpp::wrap(out_array);
    break;
  };

  case z5::types::float64: {

    // xt::rarray<double> array(shape_);
    // z5::multiarray::readSubarray<double>(*ds, array, offset_.begin());
    // res = Rcpp::wrap(array);

    double fill_value = get_fill_value<double, double>(*ds);
    xt::xarray<double> middle_array(shape_);
    xt::rarray<double> out_array(shape_);

    z5::multiarray::readSubarray<double>(*ds, middle_array, offset_.begin());
    if(std::isnan(fill_value)) {
      std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                     [](const double x) {
                       return std::isnan(x) ? na<double>() : x;
                     });
    } else {
      std::transform(middle_array.begin(), middle_array.end(), out_array.begin(),
                     [fill_value](const double x) {
                       return x == fill_value ? na<double>() : x;
                     });
    }

    res = Rcpp::wrap(out_array);
    break;
  };

  default: { Rf_error("Wrong data type."); }
  }

  return res;
}
