#pragma once

#include <Rcpp.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
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


//////////////////////////////////////////////////////////////////////
// TODO: these are all inline to avoid duplicate definitions

inline void read_json_from_disk(fs::path &path, nlohmann::json &j) {

  #ifdef WITH_BOOST_FS
    fs::ifstream file(path);
  #else
    std::ifstream file(path);
  #endif

  file >> j;
  file.close();
}

inline void write_json_to_disk(fs::path & path, nlohmann::json & j) {

  #ifdef WITH_BOOST_FS
    fs::ofstream file(path);
  #else
    std::ofstream file(path);
  #endif

  file << j;
  file.close();
}

// inline Rcpp::List json_to_rlist(const nlohmann::json & j,
//                                 const Rcpp::CharacterVector & keys) {

//   Rcpp::List l(0);

//   for(auto& k : keys){
//     try {
//       l[Rcpp::as<std::string>(k)] = j[Rcpp::as<std::string>(k)];
//     }
//     catch (std::exception& __ex__) {
//       forward_exception_to_r(__ex__);
//     }
//   }

//   return l;
// }

// null value_t::null
// boolean value_t::boolean
// string value_t::string
// number(integer) value_t::number_integer
// number(unsigned integer) value_t::number_unsigned
// number(floating - point) value_t::number_float
// object value_t::object
// array value_t::array
// discarded value_t::discarded

inline Rcpp::List json_to_rlist(const nlohmann::json& j) {
  Rcpp::List l(0);

  for(auto& it : j.items()){
    auto& key = it.key();
    auto& val = it.value();
    try {
      if (val.is_null())            l[key] = R_NilValue;
      if (val.is_boolean())         l[key] = val.get<bool>();
      if (val.is_string())          l[key] = val.get<std::string>();
      if (val.is_number_integer())  l[key] = val.get<int>();
      if (val.is_number_unsigned()) l[key] = val.get<unsigned int>();
      if (val.is_number_float())    l[key] = val.get<float>();
      if (val.is_object())          l[key] = json_to_rlist(val);
      if (val.is_array())           l[key] = json_to_rlist(val);
    }
    catch (std::exception& __ex__) {
      forward_exception_to_r(__ex__);
    }
  }

  return l;
}

inline nlohmann::json rlist_to_json(const Rcpp::List & l) {
  nlohmann::json j;

  Rcpp::CharacterVector l_names(l.names());

  for (int i = 0; i < l.length(); i++) {
    std::string new_name = Rcpp::as<std::string>(l_names[i]);
    SEXP new_val = l[i];

    switch((TYPEOF)(new_val)) {
    case NILSXP:  {nlohmann::json jnull; j += {new_name, jnull}; };
    case LGLSXP:  j += {new_name, Rcpp::as<bool>(new_val)};
    case INTSXP:  j += {new_name, Rcpp::as<int>(new_val)};
    case REALSXP: j += {new_name, Rcpp::as<double>(new_val)};
    case STRSXP:  j += {new_name, Rcpp::as<std::string>(new_val)};
    default: Rf_error("wrong type");
    }

  }

  return j;
}

/////////////////////////////////////////////////////////////////////////


inline z5::FileMode::modes rfilemode_to_filemode(const std::string & rmode) {
  if (rmode == "r")  return z5::FileMode::r;
  if (rmode == "r+") return z5::FileMode::r_p;
  if (rmode == "w")  return z5::FileMode::w;
  if (rmode == "w-") return z5::FileMode::w_m;
  if (rmode == "a")  return z5::FileMode::a;
  Rf_error("Unsupported file mode");
}

inline z5::types::Compressor string_to_compressor(const std::string & compressor) {
  z5::types::Compressor compressor_;

  try {
    compressor_ = z5::types::Compressors::stringToCompressor().at(compressor);
  } catch (const std::out_of_range & __ex__) {
    forward_exception_to_r(__ex__);
  }

  return compressor_;
}

inline z5::types::ShapeType intvec_to_shapetype(const Rcpp::IntegerVector & int_vec) {
  auto sizet_vec = Rcpp::as<std::vector<size_t> >(int_vec);
  return z5::types::ShapeType(sizet_vec);
}
