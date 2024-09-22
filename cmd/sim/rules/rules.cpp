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
	//blackjack_bets = parseAuxInt(json, "blackjackBets", 2);
	//blackjack_pays = parseAuxInt(json, "blackjackPays", 3);
	//penetration = parseAuxDouble(json, "penetration", 0.75);

	// Cleanup
	cJSON_Delete(json);
	curl_easy_cleanup(curl_handle);
	free(chunk.memory);
	curl_global_cleanup();
}

//
void Rules::print(int indent) {
	std::cout << std::left << std::setw(indent) << "Rules" << std::endl;
	std::cout << std::left << std::setw(indent) << "  Table" << ": " << playbook << std::endl;
	std::cout << std::boolalpha << std::left << std::setw(indent) << "  Hit soft 17" << ": " << hit_soft_17 << std::endl;
	std::cout << std::boolalpha << std::left << std::setw(indent) << "  Surrender" << ": " << surrender << std::endl;
	std::cout << std::boolalpha << std::left << std::setw(indent) << "  Double any two cards" << ": " << double_any_two_cards << std::endl;
	std::cout << std::boolalpha << std::left << std::setw(indent) << "  Double after split" << ": " << double_after_split << std::endl;
	std::cout << std::boolalpha << std::left << std::setw(indent) << "  Resplit aces" << ": " << resplit_aces << std::endl;
	std::cout << std::boolalpha << std::left << std::setw(indent) << "  Hit split aces" << ": " << hit_split_aces << std::endl;
	std::cout << std::left << std::setw(indent) << "  Blackjack bets" << ": " << blackjack_bets << std::endl;
	std::cout << std::left << std::setw(indent) << "  Blackjack pays" << ": " << blackjack_pays << std::endl;
	std::cout << std::left << std::setw(indent) << "  Penetration" << ": " << penetration << std::endl;
}
