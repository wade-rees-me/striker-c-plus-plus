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
#include "utilities.hpp"
#include "constants.hpp"

//
Parameters::Parameters(std::string name, std::string decks, std::string strategy, int number_of_decks, int64_t number_of_hands, Rules *rules, Logger *logger)
		: rules(rules), logger(logger), name(name), decks(decks), strategy(strategy), number_of_decks(number_of_decks), number_of_hands(number_of_hands) {
	playbook = decks + std::string("-") + strategy;
	processor = STRIKER_WHO_AM_I;
	getCurrentTime(timestamp);
}

//
void Parameters::print() {
    char buffer[256];

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Name", name.c_str());
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Playbook", playbook.c_str());
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Processor", processor.c_str());
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Version", STRIKER_VERSION.c_str());
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Number of hands", Utilities::addCommas(number_of_hands).c_str());
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Timestamp", timestamp.c_str());
	logger->simulation(buffer);
}

// Function to get current time and format it
void Parameters::getCurrentTime(std::string &buffer) {
	char time_str[128];
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	strftime(time_str, sizeof(time_str), TIME_LAYOUT, tm_info);
	buffer = time_str;
}

std::string Parameters::serialize() {
    cJSON* json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "playbook", playbook.c_str());
    cJSON_AddStringToObject(json, "name", name.c_str());
    cJSON_AddStringToObject(json, "processor", processor.c_str());
    cJSON_AddStringToObject(json, "timestamp", timestamp.c_str());
    cJSON_AddStringToObject(json, "decks", decks.c_str());
    cJSON_AddStringToObject(json, "strategy", strategy.c_str());
    cJSON_AddNumberToObject(json, "number_of_hands", number_of_hands);
    cJSON_AddNumberToObject(json, "number_of_decks", number_of_decks);

    cJSON_AddStringToObject(json, "hit_soft_17", rules->hit_soft_17 ? "true" : "false");
    cJSON_AddStringToObject(json, "surrender", rules->surrender ? "true" : "false");
    cJSON_AddStringToObject(json, "double_any_two_cards", rules->double_any_two_cards ? "true" : "false");
    cJSON_AddStringToObject(json, "double_after_split", rules->double_after_split ? "true" : "false");
    cJSON_AddStringToObject(json, "resplit_aces", rules->resplit_aces ? "true" : "false");
    cJSON_AddStringToObject(json, "hit_split_aces", rules->hit_split_aces ? "true" : "false");
    cJSON_AddNumberToObject(json, "blackjack_bets", rules->blackjack_bets);
    cJSON_AddNumberToObject(json, "blackjack_pays", rules->blackjack_pays);
    cJSON_AddNumberToObject(json, "penetration", rules->penetration);

    char* jsonString = cJSON_Print(json);
    std::string result(jsonString);

    cJSON_Delete(json);
    free(jsonString);

    return result;
}

