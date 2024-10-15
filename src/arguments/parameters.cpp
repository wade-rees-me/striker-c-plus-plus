#include <string>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <sstream> 
#include <format>
#include <iomanip> 
#include <cstdlib>
#include <uuid/uuid.h>
#include <cjson/cJSON.h>
#include "parameters.hpp"

//
Parameters::Parameters(std::string decks, std::string strategy, int number_of_decks, int64_t number_of_hands)
		: decks(decks), strategy(strategy), number_of_decks(number_of_decks), number_of_hands(number_of_hands) {
    generateName(name);
    snprintf(playbook, MAX_STRING_SIZE, "%s-%s", decks.c_str(), strategy.c_str());
    snprintf(processor, MAX_STRING_SIZE, "%s", STRIKER_WHO_AM_I.c_str());
    getCurrentTime(timestamp);
}

//
void Parameters::print() {
    printf("    %-24s: %s\n", "Name", name);
    printf("    %-24s: %s\n", "Playbook", playbook);
    printf("    %-24s: %s\n", "Processor", processor);
    printf("    %-24s: %s\n", "Version", STRIKER_VERSION.c_str());
    printf("    %-24s: %lld\n", "Number of hands", number_of_hands);
    printf("    %-24s: %s\n", "Timestamp", timestamp);
}

//
std::string Parameters::serialize() {
    cJSON* json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "playbook", playbook);
    cJSON_AddStringToObject(json, "name", name);
    cJSON_AddStringToObject(json, "processor", processor);
    cJSON_AddStringToObject(json, "timestamp", timestamp);
    cJSON_AddStringToObject(json, "decks", decks.c_str());
    cJSON_AddStringToObject(json, "strategy", strategy.c_str());
    cJSON_AddNumberToObject(json, "number_of_hands", number_of_hands);
    cJSON_AddNumberToObject(json, "number_of_decks", number_of_decks);

    char* jsonString = cJSON_Print(json);
    std::string result(jsonString);

    cJSON_Delete(json);
    free(jsonString);

    return result;
}

// Function to get current time and format it
void Parameters::getCurrentTime(char* buffer) {
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	strftime(buffer, MAX_STRING_SIZE, TIME_LAYOUT, tm_info);
}

//
void Parameters::generateName(char* buffer) {
	std::time_t t = std::time(nullptr);
	struct tm* tm_info = std::localtime(&t);

	int year = tm_info->tm_year + 1900;
	int month = tm_info->tm_mon + 1;
	int day = tm_info->tm_mday;

	std::snprintf(buffer, MAX_STRING_SIZE, "%s_%4d_%02d_%02d_%012ld", STRIKER_WHO_AM_I.c_str(), year, month, day, t);
}

