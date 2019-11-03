#include <experimental/filesystem>

#include "helpers.h"
#include <Rcpp.h>
#include <string>
#include <z5/file.hxx>

// [[Rcpp::export]]
Rcpp::XPtr<z5::handle::File> createFile(const std::string & path,
                                        const std::string & file_mode) {
  const auto mode = rfilemode_to_filemode(file_mode);
  auto file = new z5::handle::File(path, mode);

  z5::createFile(*file, true);
  Rcpp::XPtr<z5::handle::File> ptr(file);

  return ptr;
}
