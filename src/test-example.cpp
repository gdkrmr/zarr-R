/*
 * This file uses the Catch unit testing library, alongside
 * testthat's simple bindings, to test a C++ function.
 *
 * For your own packages, ensure that your test files are
 * placed within the `src/` folder, and that you include
 * `LinkingTo: testthat` within your DESCRIPTION file.
 */

// All test files should include the <testthat.h>
// header file.
#define STRICT_R_HEADERS // otherwise a PI macro is defined in R

#include <testthat.h>

#ifdef WITH_BOOST_FS
  #ifndef BOOST_FILESYSTEM_NO_DEPERECATED
    #define BOOST_FILESYSTEM_NO_DEPERECATED
  #endif
  #include <boost/filesystem.hpp>
  namespace fs = boost::filesystem;
#else
  #if __GCC__ > 7
    #include <filesystem>
    namespace fs = std::filesystem;
  #else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
  #endif
#endif

#include <array>
#include <xtensor-r/rarray.hpp>
#include <xtensor/xarray.hpp>
#include <z5/factory.hxx>
#include <z5/filesystem/factory.hxx>
#include <z5/multiarray/xtensor_access.hxx>

  // Normally this would be a function from your package's
  // compiled library -- you might instead just include a header
  // file providing the definition, and let R CMD INSTALL
  // handle building and linking.
  bool xtensor_test() {
    fs::path p("data.zr");
    z5::filesystem::handle::File f(p);
    z5::filesystem::createFile(f, true);

    xt::xarray<double> xa = {{1, 2, 3, 4, 5, 6, 7, 8, 9},
                             {10, 11, 12, 13, 14, 15, 16, 17, 18},
                             {19, 20, 21, 22, 23, 24, 25, 26, 27},
                             {28, 29, 30, 31, 32, 33, 34, 35, 36},
                             {37, 38, 39, 40, 41, 42, 43, 44, 45},
                             {46, 47, 48, 49, 50, 51, 52, 53, 54},
                             {55, 56, 57, 58, 59, 60, 61, 62, 63},
                             {64, 65, 66, 67, 68, 69, 70, 71, 72},
                             {73, 74, 75, 76, 77, 78, 79, 80, 81}};
    auto xa2 = xt::xarray<double>::from_shape(xa.shape());
    auto xasingle = xt::xarray<double>::from_shape({1,1});
    auto xachunk = xt::xarray<double>::from_shape({3,3});
    xt::rarray<double> ra(xa.shape());
    z5::types::ShapeType s({0, 0});

    auto ds1 = z5::createDataset(f, "dataset", "float64", {9, 9}, {3, 3}, "raw",
                                 z5::types::CompressionOptions());

    z5::multiarray::writeSubarray<double>(*ds1, xa, s.begin());
    z5::multiarray::readSubarray<double>(*ds1, xa2, s.begin());

    std::transform(xa2.begin(), xa2.end(), ra.begin(),
                   [](const double x) { return x * x; });

    size_t i,j,k,l;

    test_that("full readout values") {
      for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
          expect_true(xa2(i, j) == xa(i, j));
        }
      }
    }
    test_that("single value readouts"){
      for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
          z5::types::ShapeType s2({i, j});
          z5::multiarray::readSubarray<double>(*ds1, xasingle, s2.begin());
          expect_true(xasingle(0, 0) == xa(i, j));
        }
      }
    }

    test_that("chunk readouts"){
      for (i = 0; i < 9; i += 3) {
        for (j = 0; j < 9; j += 3) {
          z5::types::ShapeType s2({i, j});
          z5::multiarray::readSubarray<double>(*ds1, xachunk, s2.begin());
          for (k = 0; k < 3; k++) {
            for (l = 0; l < 3; l++) {
              expect_true(xa(i + k, j + l) == xachunk(k, l));
            }
          }
        }
      }
    }

    test_that("rarrays"){
      for (i = 0; i < 9; i++) {
        for (j = 0; j < 9; j++) {
          expect_true(ra(i, j) == xa(i, j) * xa(i, j));
        }
      }
    }

    ds1->remove();
    f.remove();

    return true;
}

// Initialize a unit test context. This is similar to how you
// might begin an R test file with 'context()', expect the
// associated context should be wrapped in braced.
context("Sample unit tests") {

  // The format for specifying tests is similar to that of
  // testthat's R functions. Use 'test_that()' to define a
  // unit test, and use 'expect_true()' and 'expect_false()'
  // to test the desired conditions.
    xtensor_test();

}
