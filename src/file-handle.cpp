
#include "helpers.h"
#include <Rcpp.h>
#include <z5/filesystem/handle.hxx>
#include <z5/util/file_mode.hxx>

////////////////////////////////////////////////////////
///////////// File /////////////////////////////////////
////////////////////////////////////////////////////////

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::File>
getFileHandle(const std::string &path, const std::string &mode) {
  fs::path p(path);
  z5::FileMode mode_ = rfilemode_to_filemode(mode);
  auto f = new z5::filesystem::handle::File(p, mode_);
  Rcpp::XPtr<z5::filesystem::handle::File> fptr(f, true);
  return fptr;
}

// [[Rcpp::export]]
bool FileHandleIsS3(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->isS3();
}

// [[Rcpp::export]]
bool FileHandleIsGcs(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->isGcs();
}

// [[Rcpp::export]]
bool FileHandleExists(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->exists();
}

// [[Rcpp::export]]
bool FileHandleIsZarr(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->isZarr();
}

// [[Rcpp::export]]
std::string FileHandlePath(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return std::string(f->path());
}

// [[Rcpp::export]]
void FileHandleDelete(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  f->remove();
}

// [[Rcpp::export]]
Rcpp::CharacterVector
FileHandleKeys(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  std::vector<std::string> out;
  f->keys(out);
  return Rcpp::wrap(out);
}

// [[Rcpp::export]]
bool FileHandleIn(Rcpp::XPtr<z5::filesystem::handle::File> f,
            const std::string &key) {
  return f->in(key);
}

// [[Rcpp::export]]
std::string FileHandleNameInBucket(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->nameInBucket();
}

// [[Rcpp::export]]
std::string FileHandleBucket(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->bucketName();
}

// [[Rcpp::export]]
void FileHandleCreate(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  f->create();
}