/*

///// The code in this file has been moved to *_handle.cpp!!!!

///// TODO: Delete this file!


/// contains the code handling z5::filesystem::handle::File,
/// z5::filesystem::handle::Group

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

////////////////////////////////////////////////////////
///////////// Group ////////////////////////////////////
////////////////////////////////////////////////////////

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
void GroupHandleDelete(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
g->remove(); }

// [[Rcpp::export]]
Rcpp::CharacterVector GroupHandleKeys(Rcpp::XPtr<z5::filesystem::handle::Group>
g) { std::vector<std::string> out; g->keys(out); return Rcpp::wrap(out);
}

// [[Rcpp::export]]
bool GroupHandleIn(Rcpp::XPtr<z5::filesystem::handle::Group> g,
                   const std::string &key) {
  return g->in(key);
}

// [[Rcpp::export]]
std::string GroupHandleNameInBucket(Rcpp::XPtr<z5::filesystem::handle::Group> g)
{ return g->nameInBucket();
}

// [[Rcpp::export]]
std::string GroupHandleBucket(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  return g->bucketName();
}

// [[Rcpp::export]]
void GroupHandleCreate(Rcpp::XPtr<z5::filesystem::handle::Group> g) {
  g->create();
}


////////////////// The code below should be handled for z5::Dataset and NOT
////////////////// z5::filesystem::handle::Dataset therefore this code should
////////////////// not be used. Everything should be handled in factory.cpp

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
