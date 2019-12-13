#include "Rcpp/XPtr.h"
#include "z5/filesystem/handle.hxx"
#include "z5/metadata.hxx"
#include <Rcpp.h>
#include <z5/filesystem/factory.hxx>

// [[Rcpp::export]]
void createFile(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  z5::filesystem::createFile(*f, true);
}

// [[Rcpp::export]]
void createGroup(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  z5::filesystem::createGroup(*g, true);
}

// [[Rcpp::export]]
void createDataset(Rcpp::XPtr<z5::filesystem::handle::Dataset> d,
                   Rcpp::XPtr<z5::DatasetMetadata> m) {
  z5::filesystem::createDataset(*d, *m);
}


// // [[Rcpp::export]]
// std::string
// relativePath(Rcpp::XPtr<z5::filesystem::handle::Group> g1,
//                  Rcpp::XPtr<z5::filesystem::handle::Group> g2) {
//   return z5::filesystem::relativePath(*g1, *g2);
// }
