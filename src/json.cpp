#include "json.h"

// [[Rcpp::export]]
Rcpp::List json_to_list(const std::string s){
  nlohmann::json j = nlohmann::json::parse(s.data());
  return json_to_rlist(j);
}

// [[Rcpp::export]]
std::string list_to_json(const Rcpp::List l) {
  nlohmann::json j = rlist_to_json(l);
  std::stringstream s;
  s << j;
  return s.str();
}
