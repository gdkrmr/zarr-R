#ifndef INCLUDE_JSON_HELPER_HEADER
#define INCLUDE_JSON_HELPER_HEADER

#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

// TODO: for some reason this file must be included *after* helpers.h

// #include "Rinternals.h"
#include <Rcpp.h>
#include <nlohmann/json.hpp>
#include <z5/common.hxx>

// #ifdef WITH_BOOST_FS
//   #ifndef BOOST_FILESYSTEM_NO_DEPERECATED
//     #define BOOST_FILESYSTEM_NO_DEPERECATED
//   #endif
//   #include <boost/filesystem.hpp>
//   namespace fs = boost::filesystem;
// #else
//   #if __GCC__ > 7
//     #include <filesystem>
//     namespace fs = std::filesystem;
//   #else
//     #include <experimental/filesystem>
//     namespace fs = std::experimental::filesystem;
//   #endif
// #endif

inline void read_json_from_disk(fs::path& path, nlohmann::json& j) {

  #ifdef WITH_BOOST_FS
    fs::ifstream file(path);
  #else
    std::ifstream file(path);
  #endif

  file >> j;
  file.close();
}

inline void write_json_to_disk(fs::path& path, nlohmann::json& j) {

  #ifdef WITH_BOOST_FS
    fs::ofstream file(path);
  #else
    std::ofstream file(path);
  #endif

  file << j;
  file.close();
}

/////////////////////////////////////////////////////////////////////////
// json <-> list
/////////////////////////////////////////////////////////////////////////

Rcpp::List json_array_to_rlist(const nlohmann::json& j);
Rcpp::List json_object_to_rlist(const nlohmann::json &j);
Rcpp::List json_to_rlist(const nlohmann::json& j);

inline Rcpp::List json_array_to_rlist(const nlohmann::json& j) {

  Rcpp::List l(j.size());

  for (size_t i = 0; i < j.size(); i++) {
    try {
      if      (j[i].is_null())            l[i] = R_NilValue;
      else if (j[i].is_boolean())         l[i] = j[i].get<bool>();
      else if (j[i].is_string())          l[i] = j[i].get<std::string>();
      else if (j[i].is_number_integer())  l[i] = j[i].get<int>();
      else if (j[i].is_number_unsigned()) l[i] = j[i].get<unsigned int>();
      else if (j[i].is_number_float())    l[i] = j[i].get<float>();
      else if (j[i].is_array())           l[i] = json_array_to_rlist(j[i]);
      else if (j[i].is_object())          l[i] = json_object_to_rlist(j[i]);
      else Rf_error("json_to_rlist error.");
    }
    catch (std::exception &__ex__) {
      forward_exception_to_r(__ex__);
    }
  }

  return l;
}

inline Rcpp::List json_object_to_rlist(const nlohmann::json& j) {

  // TODO: figure out if this can be pre-allocated!
  Rcpp::List l(0);

  for (auto& it : j.items()) {
    auto& key = it.key();
    auto& val = it.value();

    try {
      if      (val.is_null())            l[key] = R_NilValue;
      else if (val.is_boolean())         l[key] = val.get<bool>();
      else if (val.is_string())          l[key] = val.get<std::string>();
      else if (val.is_number_integer())  l[key] = val.get<int>();
      else if (val.is_number_unsigned()) l[key] = val.get<unsigned int>();
      else if (val.is_number_float())    l[key] = val.get<float>();
      else if (val.is_array())           l[key] = json_array_to_rlist(val);
      else if (val.is_object())          l[key] = json_object_to_rlist(val);
      else Rf_error("json_to_rlist error.");
    }
    catch (std::exception &__ex__) {
      forward_exception_to_r(__ex__);
    }
  }

  return l;
}

inline Rcpp::List json_to_rlist(const nlohmann::json& j) {
  if (j.is_array()) {
    return json_array_to_rlist(j);
  } else {
    return json_object_to_rlist(j);
  }
}

inline nlohmann::json rlist_to_json(const Rcpp::List &l);

inline nlohmann::json rlist_unnamed_to_json(const Rcpp::List &l) {
  nlohmann::json j;

  for (int i = 0; i < l.length(); i++) {
      SEXP new_val = l[i];

      switch ((TYPEOF)(new_val)) {
      case NILSXP: {
        nlohmann::json jnull;
        j.push_back(jnull);
        break;
      }
      case LGLSXP: {
        j.push_back(Rcpp::as<bool>(new_val));
        break;
      }
      case INTSXP: {
        j.push_back(Rcpp::as<int>(new_val));
        break;
      }
      case REALSXP: {
        j.push_back(Rcpp::as<double>(new_val));
        break;
      }
      case STRSXP: {
        j.push_back(Rcpp::as<std::string>(new_val));
        break;
      }
      case VECSXP: {
        j.push_back(rlist_to_json(new_val));
        break;
      }
      default: {
        Rf_error("wrong type");
      }
      };
  }

  return j;
}

inline nlohmann::json rlist_named_to_json(const Rcpp::List& l) {
  nlohmann::json j;

  Rcpp::CharacterVector l_names(l.names());

  for (int i = 0; i < l.length(); i++) {
    std::string new_name = Rcpp::as<std::string>(l_names[i]);
    SEXP new_val = l[i];

    switch ((TYPEOF)(new_val)) {
    case NILSXP: {
      nlohmann::json jnull;
      j += nlohmann::json::object_t::value_type(new_name, jnull);
      break;
    }
    case LGLSXP: {
      j += nlohmann::json::object_t::value_type(new_name,
                                                Rcpp::as<bool>(new_val));
      break;
    }
    case INTSXP: {
      j += nlohmann::json::object_t::value_type(new_name,
                                                Rcpp::as<int>(new_val));
      break;
    }
    case REALSXP: {
      j += nlohmann::json::object_t::value_type(new_name,
                                                Rcpp::as<double>(new_val));
      break;
    }
    case STRSXP: {
      j += nlohmann::json::object_t::value_type(new_name,
                                                Rcpp::as<std::string>(new_val));
      break;
    }
    case VECSXP: {
      j += nlohmann::json::object_t::value_type(new_name,
                                                rlist_to_json(new_val));
      break;
    }
    default: {
      Rf_error("wrong type");
    }
    }
  }

  return j;
}

inline nlohmann::json rlist_to_json(const Rcpp::List & l) {

  if ((TYPEOF)(l.names()) != NILSXP) {
    return rlist_named_to_json(l);
  } else {
    return rlist_unnamed_to_json(l);
  }
}

#endif // INCLUDE_JSON_HELPER_HEADER
