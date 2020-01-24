#include "helpers.h"
#include "json.h"

#include <Rcpp.h>
#include <z5/attributes.hxx>


///////////////////////////////////////////////////////////////////////////////////////
////// readAttributes
///////////////////////////////////////////////////////////////////////////////////////

//' Read Attributes from a file handle
//'
//' Takes a file handle and returns its attributes.
//'
//' @template xptr-warning
//'
//' @template file-handle-param
//'
//' @return List with the attributes of the file.
//'
// [[Rcpp::export]]
Rcpp::List readAttributesFileHandle(const Rcpp::XPtr<z5::filesystem::handle::File> f) {
  nlohmann::json j;
  z5::readAttributes(*f, j);
  return json_to_rlist(j);
}

//' Read Attributes from a group handle
//'
//' Takes a group handle and returns its attributes.
//'
//' @template xptr-warning
//'
//' @template group-handle-param
//'
//' @return List with the attributes of the group.
//'
// [[Rcpp::export]]
Rcpp::List readAttributesGroupHandle(const Rcpp::XPtr <z5::filesystem::handle::Group> g) {
  nlohmann::json j;
  z5::readAttributes(*g, j);
  return json_to_rlist(j);
}

//' Read Attributes from a dataset handle
//'
//' Takes a dataset handle and returns its attributes.
//'
//' @template xptr-warning
//'
//' @template dataset-handle-param
//'
//' @return List with the attributes of the dataset.
//'
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
void removeAttributeFileHandle(const Rcpp::XPtr<z5::filesystem::handle::File> f,
                                const std::string &key) {
  z5::removeAttribute(*f, key);
}

// [[Rcpp::export]]
void removeAttributeGroupHandle(const Rcpp::XPtr<z5::filesystem::handle::Group> g,
                                const std::string &key) {
  z5::removeAttribute(*g, key);
}

// [[Rcpp::export]]
void removeAttributeDatasetHandle(const Rcpp::XPtr<z5::filesystem::handle::Dataset> d,
                                const std::string &key) {
  z5::removeAttribute(*d, key);
}
