#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>
#include <nlohmann/json.hpp>

//
class Request {
	protected:
		std::string responseString;
		nlohmann::json jsonResponse;

	protected:
		void fetchJson(const std::string& url);
};

#endif // REQUEST_HPP
