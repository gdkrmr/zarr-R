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

    // NOTE: This is how to get the fill value.
    // int8_t* fillValue;
    // ds->getFillValue(fillValue);
    // int8_t fillValue_ = *fillValue;

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
      // NOTE: We use na conversion if R supports both types, else we don't care about preserving NAs
    // case z5::types::int8:    { write_subarray_transform<int8_t, uint8_t>    (*ds, data_, offset_); };
    // case z5::types::int16:   { write_subarray_transform<int16_t, int16_t>   (*ds, data_, offset_); };
    // case z5::types::int32:   { write_subarray_transform_na<int32_t, int32_t>(*ds, data_, offset_); };
    // case z5::types::int64:   { write_subarray_transform<int64_t, int64_t>   (*ds, data_, offset_); };
    case z5::types::uint8: {
      z5::multiarray::writeSubarray<uint8_t>(*ds, data_, offset_.begin());
      break;
    };
    // case z5::types::uint16:  { write_subarray_transform<uint16_t, uint16_t> (*ds, data_, offset_); };
    // case z5::types::uint32:  { write_subarray_transform<uint32_t, uint32_t> (*ds, data_, offset_); };
    // case z5::types::uint64:  { write_subarray_transform<uint64_t, uint64_t> (*ds, data_, offset_); };
    // case z5::types::float32: { write_subarray_transform<float, float>       (*ds, data_, offset_); };
    // case z5::types::float64: { write_subarray_transform_na<double, double>  (*ds, data_, offset_); };
    default: { Rf_error("Error: Type of R array (Logical) and zarr array do not match."); }
    }
    break;
  };

  case INTSXP: {
    // NOTE: not checking bounds may lead to memory leaks
    xt::rarray<int32_t> data_(data);
    check_bounds(offset, data_, *ds);

    switch (target_type) {
    // case z5::types::int8:    { write_subarray_transform<int8_t, int8_t>     (*ds, data_, offset_); };
    // case z5::types::int16:   { write_subarray_transform<int16_t, int16_t>   (*ds, data_, offset_); };
    case z5::types::int32: {
      Rprintf("writing INTSXP to int32 zarr");
      z5::multiarray::writeSubarray<int32_t>(*ds, data_, offset_.begin());
      break;
    };
    case z5::types::int64: {
      Rprintf("writing INTSXP to int64 zarr");
      subarray_transform<int64_t, int32_t>(*ds, data_, offset_);
      break;
    };
    // case z5::types::uint8:   { write_subarray_transform<uint8_t, uint8_t>   (*ds, data_, offset_); };
    // case z5::types::uint16:  { write_subarray_transform<uint16_t, uint16_t> (*ds, data_, offset_); };
    // case z5::types::uint32:  { write_subarray_transform<uint32_t, uint32_t> (*ds, data_, offset_); };
    // case z5::types::uint64:  { write_subarray_transform<uint64_t, uint64_t> (*ds, data_, offset_); };
    // case z5::types::float32: { write_subarray_transform<float, float>       (*ds, data_, offset_); };
    // case z5::types::float64: { write_subarray_transform_na<double, double>  (*ds, data_, offset_); };
    default: { Rf_error("Error: Type of R array (Integer) and zarr array do not match."); }
    }
    break;
  };

  case REALSXP: {
    // NOTE: not checking bounds may lead to memory leaks
    xt::rarray<double> data_(data);
    check_bounds(offset, data_, *ds);

    switch (target_type) {
    // case z5::types::int8:    { write_subarray_transform<int8_t, int8_t>     (*ds, data_, offset_); };
    // case z5::types::int16:   { write_subarray_transform<int16_t, int16_t>   (*ds, data_, offset_); };
    // case z5::types::int32:   { write_subarray_transform<int32_t, int32_t>   (*ds, data_, offset_); };
    case z5::types::int64: {
      subarray_transform<int64_t, double>(*ds, data_, offset_);
      break;
    };
    // case z5::types::uint8:   { write_subarray_transform<uint8_t, uint8_t>   (*ds, data_, offset_); };
    // case z5::types::uint16:  { write_subarray_transform<uint16_t, uint16_t> (*ds, data_, offset_); };
    case z5::types::uint32: {
      subarray_transform<uint32_t, double>(*ds, data_, offset_);
      break;
    };
    // case z5::types::uint64:  { write_subarray_transform<uint64_t, uint64_t> (*ds, data_, offset_); };
    case z5::types::float32: {
      subarray_transform<float, double>(*ds, data_, offset_);
      break;
    };
    case z5::types::float64: {
      z5::multiarray::writeSubarray<double>(*ds, data_, offset_.begin());
      break;
    };
    default: { Rf_error("Error: Type of R array (Real) and zarr array do not match."); }
    }
    break;
  };

  case RAWSXP: {
    // NOTE: not checking bounds may lead to memory leaks
    xt::rarray<uint8_t> data_(data);
    check_bounds(offset, data_, *ds);

    switch (target_type) {
    // case z5::types::int8:  { z5::multiarray::writeSubarray<uint8_t>(*ds, data_, offset_.begin()); };
    // case z5::types::int8:    {write_subarray_transform<int8_t, int8_t>(*ds, data_, offset_);};
    // case z5::types::int16:   {write_subarray_transform<int16_t, int16_t>(*ds, data_, offset_);};
    // case z5::types::int32:   {write_subarray_transform<int32_t, int32_t>(*ds, data_, offset_);};
    // case z5::types::int64:   { write_subarray_transform<int64_t, int64_t>(*ds, data_, offset_); };
    case z5::types::uint8: {
      z5::multiarray::writeSubarray<uint8_t>(*ds, data_, offset_.begin());
      break;
    };
    // case z5::types::uint16:  { write_subarray_transform<uint16_t, uint16_t> (*ds, data_, offset_); };
    // case z5::types::uint32:  {write_subarray_transform<uint32_t, uint32_t> (*ds, data_, offset_); };
    // case z5::types::uint64:  { write_subarray_transform<uint64_t, uint64_t> (*ds, data_, offset_); };
    // case z5::types::float32: {write_subarray_transform<float, float>       (*ds, data_, offset_); };
    // case z5::types::float64: { write_subarray_transform_na<double, double>(*ds, data_, offset_); };
    default: { Rf_error("Error: Type of R array (Raw) and zarr array do not match."); }
    }
    break;
  };

  default: {
    Rf_error("Value must be Logical, Integer, Real, or Raw.");
  };
  }
}
