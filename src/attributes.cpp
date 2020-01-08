#include "helpers.h"
#include "json.h"

#include <Rcpp.h>
#include <z5/attributes.hxx>

///////////////////////////////////////////////////////////////////////////////////////
////// readAttributes
///////////////////////////////////////////////////////////////////////////////////////


// [[Rcpp::export]]
Rcpp::List readAttributesFileHandle(const Rcpp::XPtr<z5::filesystem::handle::File> f) {
  nlohmann::json j;
  z5::readAttributes(*f, j);
  return json_to_rlist(j);
}

// [[Rcpp::export]]
Rcpp::List readAttributesGroupHandle(const Rcpp::XPtr <z5::filesystem::handle::Group> g) {
  nlohmann::json j;
  z5::readAttributes(*g, j);
  return json_to_rlist(j);
}

// [[Rcpp::export]]
Rcpp::List readAttributesDatasetHandle(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  nlohmann::json j;
  z5::readAttributes(*d, j);
  return json_to_rlist(j);
}

///////////////////////////////////////////////////////////////////////////////////////
////// writeAttributes
///////////////////////////////////////////////////////////////////////////////////////

// [[Rcpp::export]]
void writeAttributesFileHandle(const Rcpp::XPtr<z5::filesystem::handle::File> f,
                               const Rcpp::List &l) {
  nlohmann::json j = rlist_to_json(l);
  z5::writeAttributes(*f, j);
}

// [[Rcpp::export]]
void writeAttributesGroupHandle(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
                                const Rcpp::List &l) {
  nlohmann::json j = rlist_to_json(l);
  z5::writeAttributes(*g, j);
}

// [[Rcpp::export]]
void writeAttributesDatasetHandle(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d,
                                  const Rcpp::List &l) {
  nlohmann::json j = rlist_to_json(l);
  z5::writeAttributes(*d, j);
}

///////////////////////////////////////////////////////////////////////////////////////
////// removeAttributes
///////////////////////////////////////////////////////////////////////////////////////

// [[Rcpp::export]]
void removeAttributesFileHandle(const Rcpp::XPtr<z5::filesystem::handle::File> f,
                                const std::string &key) {
  z5::removeAttribute(*f, key);
}

// [[Rcpp::export]]
void removeAttributesGroupHandle(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
                                const std::string &key) {
  z5::removeAttribute(*g, key);
}

// [[Rcpp::export]]
void removeAttributesDatasetHandle(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d,
                                const std::string &key) {
  z5::removeAttribute(*d, key);
}
