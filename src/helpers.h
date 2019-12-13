#ifndef INCLUDE_ZARR_HELPER_HEADER
#define INCLUDE_ZARR_HELPER_HEADER

#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

#include <Rcpp.h>
#include <z5/attributes.hxx>
#include <z5/factory.hxx>
#include <xtensor-r/rarray.hpp>
#include <z5/multiarray/xtensor_access.hxx>

#ifdef WITH_BOOST_FS
  #ifndef BOOST_FILESYSTEM_NO_DEPERECATED
    #define BOOST_FILESYSTEM_NO_DEPERECATED
  #endif
  #include <boost/filesystem.hpp>
  namespace fs = boost::filesystem;
#else
  #if __GCC__ > 7
    #include <filesystem>
    namespace fs = std::filesystem;
  #else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
  #endif
#endif


// TODO: these are all inline to avoid duplicate definitions, they probably
// shouldn't be inlined!

/////////////////////////////////////////////////////////////////////////
// file mode
/////////////////////////////////////////////////////////////////////////


inline z5::FileMode::modes rfilemode_to_filemode(const std::string & rmode) {
  if (rmode == "r")  return z5::FileMode::r;
  if (rmode == "r+") return z5::FileMode::r_p;
  if (rmode == "w")  return z5::FileMode::w;
  if (rmode == "w-") return z5::FileMode::w_m;
  if (rmode == "a")  return z5::FileMode::a;
  Rf_error("Unsupported file mode");
}

/////////////////////////////////////////////////////////////////////////
// compressor
/////////////////////////////////////////////////////////////////////////

inline z5::types::Compressor string_to_compressor(const std::string & compressor) {
  z5::types::Compressor compressor_;

  try {
    compressor_ = z5::types::Compressors::stringToCompressor().at(compressor);
  } catch (const std::out_of_range & __ex__) {
    forward_exception_to_r(__ex__);
  }

  return compressor_;
}

/////////////////////////////////////////////////////////////////////////
// shapetype
/////////////////////////////////////////////////////////////////////////

inline z5::types::ShapeType intvec_to_shapetype(const Rcpp::IntegerVector & int_vec) {
  auto sizet_vec = Rcpp::as<std::vector<size_t> >(int_vec);
  return z5::types::ShapeType(sizet_vec);
}

/////////////////////////////////////////////////////////////////////////
// bounds checking
/////////////////////////////////////////////////////////////////////////

inline void check_bounds(const Rcpp::IntegerVector& offset,
                         const Rcpp::IntegerVector& subarray_shape,
                         const z5::Dataset& dataset) {

  // TODO: Not sure but I think this is really slow, because it has to be read
  // from disk every time.
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

inline void check_bounds(const Rcpp::IntegerVector& offset,
                         const xt::rarray<double>& subarray,
                         const z5::Dataset& dataset) {

  // TODO: Not sure but I think this is really slow, because it has to be read
  // from disk every time.
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

/////////////////////////////////////////////////////////////////////////
// conversions
/////////////////////////////////////////////////////////////////////////

template <typename T> inline T             const na();
template <>           inline double        const na() { return NA_REAL;    };
template <>           inline int           const na() { return NA_INTEGER; };
template <>           inline unsigned char const na() { return NA_LOGICAL; };

// template <typename FROM, typename TO>
// auto cast_l = [](const FROM x) { return static_cast<TO>(x); };

// template <typename FROM, typename TO>
// auto cast_na_l =
//   [](const FROM x) { return R_IsNA(x) ? na<TO>() : static_cast<TO>(x); };

template <typename TO_T, typename INNER_FROM_T, typename FROM_T>
inline void transform_write(z5::Dataset &out_data, xt::rarray<FROM_T> &in_data,
                            z5::types::ShapeType &offset) {
  std::vector<std::size_t> in_shape;
  for (auto& s : in_data.shape()) { in_shape.push_back(s); }
  xt::xarray<TO_T> middle_data(in_shape);
  std::transform(in_data.begin(), in_data.end(), middle_data.begin(),
                 [](const INNER_FROM_T x) { return static_cast<TO_T>(x); });
  z5::multiarray::writeSubarray<TO_T>(out_data, middle_data, offset.begin());
}

// template <typename TO_T, typename INNER_FROM_T, typename FROM_T>
// inline void subarray_transform_na(z5::Dataset &out_data,
//                                         xt::rarray<FROM_T> &in_data,
//                                         z5::types::ShapeType &offset) {
//   xt::xarray<TO_T> middle_data(in_data.dimension());
//   std::transform(in_data.begin(), in_data.end(), middle_data.begin(),
//                  cast_na_l<INNER_FROM_T, TO_T>);
//   /* z5::multiarray::writeSubarray<int8_t>(out_data, middle_data, offset.begin()); */
// }

/////////////////////////////////////////////////////////////////////////
// fill value
/////////////////////////////////////////////////////////////////////////

template <typename T> inline T get_fill_value(z5::Dataset &ds) {
  T *fp;
  ds.getFillValue(fp);
  T fw = *fp;
  return fw;
}


// // [[Rcpp::export]]
// std::string getPath(const Rcpp::XPtr<z5::filesystem::handle::Dataset> ds) {
//   std::string s(ds->path());

//   return s;
// }

#endif // INCLUDE_ZARR_HELPER_HEADER
