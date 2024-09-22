#include <string>
#include <iostream>
#include <cjson/cJSON.h>

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

