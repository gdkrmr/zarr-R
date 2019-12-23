#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

#include "Rcpp/XPtr.h"
#include "helpers.h"
#include <Rcpp.h>
#include <z5/dataset.hxx>


void test(Rcpp::XPtr<z5::Dataset> d) {
  d->isZarr();
  d->getDtype();
  d->dimension();
  shapetype_to_intvec(d->shape());
  d->size();
  d->getDtype(); //
  d->isZarr();
  d->mode();
  d->path();
  d->remove();
  shapetype_to_intvec(d->defaultChunkShape());
  d->defaultChunkSize();
  // d->getFillValue(void *);
  d->getCompressor();

}

template <typename T> inline T get_fill_value(z5::Dataset &ds) {
  T *fp;
  ds.getFillValue(fp);
  T fw = *fp;
  return fw;
}
