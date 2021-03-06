#ifndef INCLUDE_ZARR_HELPER_HEADER
#define INCLUDE_ZARR_HELPER_HEADER

#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

#include <Rcpp.h>
#include <limits>
#include <xtensor-r/rarray.hpp>
#include <z5/attributes.hxx>
#include <z5/factory.hxx>
#include <z5/multiarray/xtensor_access.hxx>

// TODO: these are all inline to avoid duplicate definitions, they probably
// shouldn't be inlined!

/////////////////////////////////////////////////////////////////////////
// json disk IO
/////////////////////////////////////////////////////////////////////////

inline void read_json_from_disk(fs::path &path, nlohmann::json &j) {

#ifdef WITH_BOOST_FS
  fs::ifstream file(path);
#else
  std::ifstream file(path);
#endif

  file >> j;
  file.close();
}

inline void write_json_to_disk(fs::path &path, nlohmann::json &j) {

#ifdef WITH_BOOST_FS
  fs::ofstream file(path);
#else
  std::ofstream file(path);
#endif

  file << j;
  file.close();
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

inline std::string compressor_to_string(const z5::types::Compressor c) {
  return z5::types::Compressors::compressorToZarr().at(c);
}

/////////////////////////////////////////////////////////////////////////
// shapetype
/////////////////////////////////////////////////////////////////////////

inline z5::types::ShapeType intvec_to_shapetype(const Rcpp::IntegerVector & int_vec) {
  auto sizet_vec = Rcpp::as<std::vector<size_t> >(int_vec);
  return z5::types::ShapeType(sizet_vec);
}

inline Rcpp::IntegerVector shapetype_to_intvec(const z5::types::ShapeType& shape) {
  auto sizet_vec = std::vector<size_t>(shape);
  return Rcpp::wrap(sizet_vec);
}


/////////////////////////////////////////////////////////////////////////
// dtype
/////////////////////////////////////////////////////////////////////////

inline z5::types::Datatype string_to_datatype(const std::string & dtype) {
  return z5::types::Datatypes::n5ToDtype()[dtype];
}

inline std::string datatype_to_string(const z5::types::Datatype dtype) {
  return z5::types::Datatypes::dtypeToN5()[dtype];
}


////////////////////////////////////////////////////////////////////////////////
// NAs and NaNs
//
// NA_real_ is actually a NaN value and therefore has to be treated separately
////////////////////////////////////////////////////////////////////////////////

template <typename T> inline T             na();
template <>           inline double        na() { return NA_REAL;    };
template <>           inline int           na() { return NA_INTEGER; };
// This function is wrong but without it the thing won't compile.
// TODO: figure out where it is being used!
template <>           inline unsigned char na() { return NA_LOGICAL; };
// template <>           inline int           na() { return NA_LOGICAL; };

template <typename T> inline bool isna(T x)        { return x == na<T>(); };
template <>           inline bool isna(float x)    { return std::isnan(x); };
template <>           inline bool isna(double x)   { return std::isnan(x); };
// template <>           inline bool isna(rlogical x) { return x == NA_LOGICAL; };


////////////////////////////////////////////////////////////////////////////////
// narrow:
//
// This is function is adapted from the Microsoft GSL library, MIT licensed.
////////////////////////////////////////////////////////////////////////////////

template <typename T, typename U>
inline T narrow(U u) {
  T t = static_cast<T>(u);
  if(static_cast<U>(t) != u)
    Rf_error("Narrow: Conversion failed");
  return t;
}

// We need to actually allow loss of precision when converting from double to
// float
template <>
inline float narrow(double u) {
  return static_cast<float>(u);
}

/////////////////////////////////////////////////////////////////////////
// fill value
/////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
inline T2 get_fill_value(const z5::Dataset &d) {
  T1 fv;
  d.getFillValue(&fv);
  return static_cast<T2>(fv);
}

/////////////////////////////////////////////////////////////////////////
// metadata
/////////////////////////////////////////////////////////////////////////

inline nlohmann::json getDatasetMetadataDatasetJson(z5::Dataset &d, nlohmann::json &j) {
  nlohmann::json compressionOptsJson;
  z5::types::CompressionOptions compressionOptions;

  d.getCompressionOptions(compressionOptions);
  z5::types::writeZarrCompressionOptionsToJson(d.getCompressor(), compressionOptions, compressionOptsJson);

  j["compressor"] = compressionOptsJson;
  j["dtype"] = z5::types::Datatypes::dtypeToZarr().at(d.getDtype());
  j["shape"] = d.shape();
  j["chunks"] = d.defaultChunkShape();

  switch (d.getDtype()) {
  case z5::types::Datatype::uint8:   { j["fill_value"] = get_fill_value<uint8_t,   int>(d); break; };
  case z5::types::Datatype::uint16:  { j["fill_value"] = get_fill_value<uint16_t,  int>(d); break; };
  case z5::types::Datatype::uint32:  { j["fill_value"] = get_fill_value<uint32_t,  int>(d); break; };
  case z5::types::Datatype::uint64:  { j["fill_value"] = get_fill_value<uint64_t,  int>(d); break; };
  case z5::types::Datatype::int8:    { j["fill_value"] = get_fill_value<int8_t,    int>(d); break; };
  case z5::types::Datatype::int16:   { j["fill_value"] = get_fill_value<int16_t,   int>(d); break; };
  case z5::types::Datatype::int32:   { j["fill_value"] = get_fill_value<int32_t,   int>(d); break; };
  case z5::types::Datatype::int64:   { j["fill_value"] = get_fill_value<int64_t,   int>(d); break; };
  case z5::types::Datatype::float32: { j["fill_value"] = get_fill_value<float,  double>(d); break; };
  case z5::types::Datatype::float64: { j["fill_value"] = get_fill_value<double, double>(d); break; };
  default: Rf_error("wrong data type");
  }

  j["filters"] = nullptr;
  j["order"] = "C";
  j["zarr_format"] = 2;


  return j;
}

#endif // INCLUDE_ZARR_HELPER_HEADER
