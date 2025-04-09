#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <nlohmann/json.hpp>
#include <string>

//
class Request {
  protected:
    std::string responseString;
    nlohmann::json jsonResponse;

  protected:
    void fetchJson(const std::string &url);
};

#endif // REQUEST_HPP
