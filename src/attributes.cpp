#include "helpers.h"
#include "json.h"

#include <Rcpp.h>
#include <z5/attributes.hxx>

// [[Rcpp::export]]
Rcpp::List readAttributesGroup(const Rcpp::XPtr <z5::filesystem::handle::Group> g) {
  nlohmann::json j;
  z5::readAttributes(*g, j);
  return json_to_rlist(j);
}

// [[Rcpp::export]]
Rcpp::List readAttributesDataset(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  nlohmann::json j;
  z5::readAttributes(*d, j);
  return json_to_rlist(j);
}

// [[Rcpp::export]]
void writeAttributesFile(const Rcpp::XPtr<z5::filesystem::handle::File> f,
                         const Rcpp::List &l) {
  nlohmann::json j = rlist_to_json(l);
  z5::writeAttributes(*f, j);
}

// [[Rcpp::export]]
void writeAttributesGroup(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
                          const Rcpp::List &l) {
  nlohmann::json j = rlist_to_json(l);
  z5::writeAttributes(*g, j);
}

// [[Rcpp::export]]
void writeAttributesDataset(const Rcpp::XPtr<z5::filesystem::handle::Dataset> ds,
                            const Rcpp::List & l) {
  nlohmann::json j = rlist_to_json(l);
  z5::writeAttributes(*ds, j);
}

