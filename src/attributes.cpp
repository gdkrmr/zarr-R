#include "helpers.h"
#include "json.h"

#include <Rcpp.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <z5/attributes.hxx>

// [[Rcpp::export]]
Rcpp::List readAttributesSubset(const std::string & path,
                                const std::vector<std::string> & keys) {
  nlohmann::json j;
  fs::path attributes_path(path);

  z5::attrs_detail::readAttributes(attributes_path, keys, j);

  return json_to_rlist(j);
}


// [[Rcpp::export]]
Rcpp::List readAttributes(const std::string & path) {
  nlohmann::json j;
  fs::path attributes_path(path);

  z5::attrs_detail::readAttributes(attributes_path, j);

  return json_to_rlist(j);
}


// [[Rcpp::export]]
void writeAttributes(const std::string & path, const Rcpp::List & l) {
  fs::path attributes_path(path);

  nlohmann::json j = rlist_to_json(l);

  z5::attrs_detail::readAttributes(attributes_path, j);
}

// [[Rcpp::export]]
std::string getPath(const Rcpp::XPtr<z5::Dataset> ds) {
  std::string s(ds->handle().path());

  return s;
}
