#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <curl/curl.h>
#include "strategy.hpp"
#include "memory.hpp"
#include "json.hpp"
#include "constants.hpp"

//
Strategy::Strategy(std::string p, int n) : playbook(p), numberOfCards(n) {
	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	if (!curl_handle) {
		curl_global_cleanup();
		std::exit(EXIT_FAILURE);
	}
}

//
Strategy::~Strategy() {
	curl_easy_cleanup(curl_handle);
	curl_global_cleanup();
}

//
void Strategy::doPlay(const int *seenCards, const int *haveCards, Card* pair, Card* up) {
	json_object = httpGet(urlPlay, buildParams(seenCards, haveCards, pair, up));
}

// Function to build the URL parameters
std::string Strategy::buildParams(const int *seenData, const int *haveData, Card* pair, Card* up) {
	std::string params = "playbook=" + playbook;
	params += "&cards=" + std::to_string(numberOfCards);
	if (up != nullptr) {
		params += "&up=" + std::to_string(up->getOffset());
	}
	if (pair != nullptr) {
		params += "&pair=" + std::to_string(pair->getValue());
	}

	if (seenData != nullptr) {
		cJSON *seenJson = cJSON_CreateIntArray(seenData, 13);
		std::string seenString = cJSON_Print(seenJson);
		seenString.erase(std::remove(begin(seenString), end(seenString), ' '), end(seenString));
		params += "&seen=" + seenString;
		cJSON_Delete(seenJson);
	}

	if (haveData != nullptr) {
		cJSON *haveJson = cJSON_CreateIntArray(haveData, 13);
		std::string haveString = cJSON_Print(haveJson);
		haveString.erase(std::remove(begin(haveString), end(haveString), ' '), end(haveString));
		params += "&have=" + haveString;
		cJSON_Delete(haveJson);
	}

	return params;
}

// Function to perform HTTP GET request
cJSON* Strategy::httpGet(std::string& url, std::string params) {
	MemoryStruct chunk;
	char fullUrl[2048];

	chunk.memory = static_cast<char *>(malloc(1));  // Will be grown as needed by realloc
	chunk.size = 0;  // No data at this point

	snprintf(fullUrl, sizeof(fullUrl), "%s?%s", url.c_str(), params.c_str());
//std::cout << "http: " << fullUrl << std::endl;
	curl_easy_setopt(curl_handle, CURLOPT_URL, fullUrl);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, static_cast<void *>(&chunk));

	CURLcode res = curl_easy_perform(curl_handle);
	if (res != CURLE_OK) {
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
		std::exit(EXIT_FAILURE);
	}

	cJSON *json = cJSON_Parse(chunk.memory);
	if (json == nullptr) {
		std::cerr << "Failed to parse JSON\n";
		std::exit(EXIT_FAILURE);
	}
	free(chunk.memory);
	return json;
}

