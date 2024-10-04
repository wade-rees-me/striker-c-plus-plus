#include <iostream>
#include <iomanip> 
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <exception>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "rules.hpp"
#include "memory.hpp"
#include "json.hpp"
#include "constants.hpp"

// Function to load table rules by calling fetchRulesTable
void Rules::rulesLoadTable(const std::string &decks) {
	try {
		rulesFetchTable(getRulesUrl() + "/" + decks);
	}
	catch (std::exception fault) {
		std::cerr << "Error fetching rules table: " << fault.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

// Function to fetch rules table using libcurl
void Rules::rulesFetchTable(const std::string &url) {
	struct MemoryStruct chunk;
	CURLcode res;
	CURL *curl_handle;

	chunk.memory = (char*) malloc(1);  // Will be grown as needed by realloc
	chunk.size = 0;					// No data at this point

	curl_global_init(CURL_GLOBAL_ALL);
	curl_handle = curl_easy_init();

	if(!curl_handle) {
		throw std::runtime_error("cannot get curl handle");
	}

	curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeMemoryCallback);
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	res = curl_easy_perform(curl_handle);
	if(res != CURLE_OK) {
		throw std::runtime_error("curl_easy_perform() failed");
	}

	// Process the JSON response
	cJSON *json = cJSON_Parse(chunk.memory);
	if(json == NULL) {
		throw std::runtime_error("Error parsing JSON response");
	}

	cJSON *itemPayload = cJSON_GetObjectItemCaseSensitive(json, "payload");
	json = cJSON_Parse(itemPayload->valuestring);
	if(json == NULL) {
		throw std::runtime_error("Error parsing JSON response");
	}

	std::snprintf(playbook, 128, "%s", parseAuxString(json, "playbook").c_str());
	hit_soft_17 = parseAuxBool(json, "hitSoft17", false);
	surrender = parseAuxBool(json, "surrender", false);
	double_any_two_cards = parseAuxBool(json, "doubleAnyTwoCards", false);
	double_after_split = parseAuxBool(json, "doubleAfterSplit", false);
	resplit_aces = parseAuxBool(json, "resplitAces", false);
	hit_split_aces = parseAuxBool(json, "hitSplitAces", false);
	blackjack_bets = parseAuxInt(json, "blackjackBets", 1);
	blackjack_pays = parseAuxInt(json, "blackjackPays", 1);
	penetration = parseAuxDouble(json, "penetration", 0.65);

	// Cleanup
	cJSON_Delete(json);
	curl_easy_cleanup(curl_handle);
	free(chunk.memory);
	curl_global_cleanup();
}

//
void Rules::print(Logger *logger) {
    char buffer[256];

    std::snprintf(buffer, sizeof(buffer), "    %-24s\n", "Table Rules");
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %s\n", "Table", playbook);
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %s\n", "Hit soft 17", hit_soft_17 ? "true" : "false");
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %s\n", "Surrender", surrender ? "true" : "false");
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %s\n", "Double any two cards", double_any_two_cards ? "true" : "false");
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %s\n", "Double after split", double_after_split ? "true" : "false");
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %s\n", "Resplit aces", resplit_aces ? "true" : "false");
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %s\n", "Hit split aces", hit_split_aces ? "true" : "false");
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %d\n", "Blackjack bets", blackjack_bets);
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %d\n", "Blackjack pays", blackjack_pays);
	logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "      %-24s: %0.3f %%\n", "Penetration", penetration);
	logger->simulation(buffer);
}
