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
bool FileIsS3(Rcpp::XPtr<z5::filesystem::handle::File> f) { return f->isS3(); }

// [[Rcpp::export]]
bool FileIsGcs(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->isGcs();
}

// [[Rcpp::export]]
bool FileExists(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->exists();
}

// [[Rcpp::export]]
bool FileIsZarr(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return f->isZarr();
}

// [[Rcpp::export]]
std::string FilePath(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  return std::string(f->path());
}

// [[Rcpp::export]]
void FileDelete(Rcpp::XPtr<z5::filesystem::handle::File> f) { f->remove(); }

// [[Rcpp::export]]
Rcpp::CharacterVector FileKeys(Rcpp::XPtr<z5::filesystem::handle::File> f) {
  std::vector<std::string> out;
  f->keys(out);
  return Rcpp::wrap(out);
}

// [[Rcpp::export]]
bool FileIn(Rcpp::XPtr<z5::filesystem::handle::File> f,
            const std::string &key) {
  return f->in(key);
}

////////////////////////////////////////////////////////
///////////// Group ////////////////////////////////////
////////////////////////////////////////////////////////

    // [[Rcpp::export]]
    Rcpp::XPtr<z5::filesystem::handle::Group> getGroupHandleFile(
        Rcpp::XPtr<z5::filesystem::handle::File> f, std::string &key) {
  z5::filesystem::handle::Group g(*f, key);
  Rcpp::XPtr<z5::filesystem::handle::Group> gptr(&g);
  return gptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::Group>
getGroupHandleGroup(Rcpp::XPtr<z5::filesystem::handle::Group> g1,
                    std::string &key) {
  z5::filesystem::handle::Group g2(*g1, key);
  Rcpp::XPtr<z5::filesystem::handle::Group> gptr(&g2);
  return gptr;
}

////////////////////////////////////////////////////////
///////////// Dataset //////////////////////////////////
////////////////////////////////////////////////////////

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::Dataset>
getDatasetHandleFile(Rcpp::XPtr<z5::filesystem::handle::File> f,
                     std::string &key) {
  z5::filesystem::handle::Dataset d(*f, key);
  Rcpp::XPtr<z5::filesystem::handle::Dataset> dptr(&d);
  return dptr;
}

// [[Rcpp::export]]
Rcpp::XPtr<z5::filesystem::handle::Dataset>
getDatasetHandleGroup(Rcpp::XPtr<z5::filesystem::handle::Group> g,
                      std::string &key) {
  z5::filesystem::handle::Dataset d(*g, key);
  Rcpp::XPtr<z5::filesystem::handle::Dataset> dptr(&d);
  return dptr;
}
