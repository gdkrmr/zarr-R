#include "json.h"


//' Convert a json string to a list
//'
//' Takes a character vector and converts it to a list.
//'
//' @param s A character vector of length 1.
//'
//' @return A list.
//'
//' @examples
//' zarr:::json_to_list('{"hello": "json"}')
//'
// [[Rcpp::export]]
Rcpp::List json_to_list(const std::string s){
  nlohmann::json j = nlohmann::json::parse(s.data());
  return json_to_rlist(j);
}

//' Convert a list to a json character vector
//'
//' Takes a list and converts it to a json character vector.
//'
//' @param l A list object.
//'
//' @return A json object encoded in a character vector.
//'
//' @examples
//' zarr:::list_to_json(list(hello = "json"))
//'
// [[Rcpp::export]]
std::string list_to_json(const Rcpp::List l) {
  nlohmann::json j = rlist_to_json(l);
  std::stringstream s;
  s << j;
  return s.str();
}
