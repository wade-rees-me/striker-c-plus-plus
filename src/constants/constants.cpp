#include <cstdlib>
#include <string>
#include <string>
#include <iostream>
#include "constants.hpp"

std::string getRulesUrl() {
	return std::getenv("STRIKER_URL_RULES");
}

std::string getStrategyUrl() {
	return std::getenv("STRIKER_URL_ACE");
}

std::string getSimulationUrl() {
	return std::getenv("STRIKER_URL_SIMULATION");
}

// Function to convert bool to string
const char* boolToString(bool b) {
    return b ? "true" : "false";
}

// Callback function for libcurl to write data
size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct*)userp;

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

//
std::string parseAuxString(cJSON *json, std::string tag) {
	cJSON *item = cJSON_GetObjectItem(json, tag.c_str());
	if (item != nullptr) {
		return item->valuestring;
	}
	std::cerr << "Error fetching rules table item: " << tag << std::endl;
	std::exit(EXIT_FAILURE);
}

//
bool parseAuxBool(cJSON *json, std::string tag, bool value) {
	cJSON *item = cJSON_GetObjectItem(json, tag.c_str());
	if (item != nullptr) {
		return cJSON_IsTrue(item);
	}
	return value;
}

//
int parseAuxInt(cJSON *json, std::string tag, int value) {
	cJSON *item = cJSON_GetObjectItem(json, tag.c_str());
	if (item != nullptr) {
		return item->valueint;
	}
	return value;
}

//
double parseAuxDouble(cJSON *json, std::string tag, double value) {
	cJSON *item = cJSON_GetObjectItem(json, tag.c_str());
	if (item != nullptr) {
		return item->valuedouble;
	}
	return value;
}

