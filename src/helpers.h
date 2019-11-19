#pragma once

#include <Rcpp.h>
#include <z5/attributes.hxx>
#include <z5/dataset_factory.hxx>

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
