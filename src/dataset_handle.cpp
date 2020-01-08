/*

////////////////// This code should be handled for z5::Dataset and NOT
////////////////// z5::filesystem::handle::Dataset therefore this code should
////////////////// not be used. Everything should be handled in factory.cpp

#include "helpers.h"
#include <Rcpp.h>
#include <z5/filesystem/handle.hxx>
#include <z5/util/file_mode.hxx>

////////////////////////////////////////////////////////
///////////// Dataset //////////////////////////////////
////////////////////////////////////////////////////////

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::Dataset>
getDatasetHandleFileHandle(Rcpp::XPtr<z5::filesystem::handle::File> f,
                           std::string &key) {
  auto d = new z5::filesystem::handle::Dataset(*f, key);
  Rcpp::XPtr<z5::filesystem::handle::Dataset> dptr(d, true);
  return dptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::Dataset>
getDatasetHandleGroupHandle(Rcpp::XPtr<z5::filesystem::handle::Group> g,
                            std::string &key) {
  auto d =  new z5::filesystem::handle::Dataset(*g, key);
  Rcpp::XPtr<z5::filesystem::handle::Dataset> dptr(d, true);
  return dptr;
}

// [[Rcpp::export]]
bool DatasetHandleIsS3(Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  return d->isS3();
}

// [[Rcpp::export]]
bool DatasetHandleIsGcs(Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  return d->isGcs();
}

// [[Rcpp::export]]
bool DatasetHandleExists(Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  return d->exists();
}

// [[Rcpp::export]]
bool DatasetHandleIsZarr(Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  return d->isZarr();
}

// [[Rcpp::export]]
std::string DatasetHandlePath(Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  return std::string(d->path());
}

// [[Rcpp::export]]
void DatasetHandleDelete(Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  d->remove();
}

// [[Rcpp::export]]
void DatasetHandleCreate(Rcpp::XPtr<z5::filesystem::handle::Dataset> d) {
  d->create();
}
*/