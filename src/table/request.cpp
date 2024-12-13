#include <iostream>
#include <curl/curl.h>
#include "request.hpp"

// Function to handle HTTP requests with libcurl
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t newLength = size * nmemb;
    buffer->append((char*)contents, newLength);
    return newLength;
}

//
void Request::fetchJson(const std::string& url) {
    CURL* curl;
    CURLcode res;

    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize curl" << std::endl;
        exit(0);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        exit(0);
    }

    curl_easy_cleanup(curl);

    jsonResponse = nlohmann::json::parse(responseString);
    if (jsonResponse.is_discarded()) {
        std::cerr << "Error parsing JSON response" << std::endl;
        exit(0);
    }
}

