
#include "helpers.h"
#include <Rcpp.h>
#include <z5/filesystem/handle.hxx>
#include <z5/util/file_mode.hxx>

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::Group>
getGroupHandleFileHandle(Rcpp::XPtr<z5::filesystem::handle::File> f,
                         std::string &key) {
  auto g =  new z5::filesystem::handle::Group(*f, key);
  Rcpp::XPtr<z5::filesystem::handle::Group> gptr(g, true);
  return gptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::Group>
getGroupHandleGroupHandle(Rcpp::XPtr<z5::filesystem::handle::Group> g1,
                    std::string &key) {
  auto g2 = new z5::filesystem::handle::Group(*g1, key);
  Rcpp::XPtr<z5::filesystem::handle::Group> gptr(g2, true);
  return gptr;
}

// [[Rcpp::export]]
bool GroupHandleIsS3(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return g->isS3();
}

// [[Rcpp::export]]
bool GroupHandleIsGcs(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return g->isGcs();
}

// [[Rcpp::export]]
bool GroupHandleExists(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return g->exists();
}

// [[Rcpp::export]]
bool GroupHandleIsZarr(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return g->isZarr();
}

// [[Rcpp::export]]
std::string GroupHandlePath(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return std::string(g->path());
}

// [[Rcpp::export]]
void GroupHandleDelete(Rcpp::XPtr<z5::filesystem::handle::Group> g) { g->remove(); }

// [[Rcpp::export]]
Rcpp::CharacterVector GroupHandleKeys(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  std::vector<std::string> out;
  g->keys(out);
  return Rcpp::wrap(out);
}

// [[Rcpp::export]]
bool GroupHandleIn(Rcpp::XPtr<z5::filesystem::handle::Group> g,
                   const std::string &key) {
  return g->in(key);
}

// [[Rcpp::export]]
std::string GroupHandleNameInBucket(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return g->nameInBucket();
}

// [[Rcpp::export]]
std::string GroupHandleBucket(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return g->bucketName();
}

// [[Rcpp::export]]
void GroupHandleCreate(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  g->create();
}
