#include <iostream>
#include <iomanip> 
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <exception>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "rules.hpp"

// Function to load table rules by calling fetchRulesTable
Rules::Rules(const std::string &decks) {
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
	CURLcode res;
	CURL *curl_handle;

	struct MemoryStruct chunk;
	chunk.size = 0;

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
	curl_global_cleanup();
}

//
void Rules::print() {
    printf("    %-24s\n", "Table Rules");
    printf("      %-24s: %s\n", "Table", playbook);
    printf("      %-24s: %s\n", "Hit soft 17", boolToString(hit_soft_17));
    printf("      %-24s: %s\n", "Surrender", boolToString(surrender));
    printf("      %-24s: %s\n", "Double any two cards", boolToString(double_any_two_cards));
    printf("      %-24s: %s\n", "Double after split", boolToString(double_after_split));
    printf("      %-24s: %s\n", "Resplit aces", boolToString(resplit_aces));
    printf("      %-24s: %s\n", "Hit split aces", boolToString(hit_split_aces));
    printf("      %-24s: %d\n", "Blackjack bets", blackjack_bets);
    printf("      %-24s: %d\n", "Blackjack pays", blackjack_pays);
    printf("      %-24s: %0.3f %%\n", "Penetration", penetration);
}

//
void Rules::serializeRules(char* buffer, int buffer_size) {
    cJSON* json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "hit_soft_17", hit_soft_17 ? "true" : "false");
    cJSON_AddStringToObject(json, "surrender", surrender ? "true" : "false");
    cJSON_AddStringToObject(json, "double_any_two_cards", double_any_two_cards ? "true" : "false");
    cJSON_AddStringToObject(json, "double_after_split", double_after_split ? "true" : "false");
    cJSON_AddStringToObject(json, "resplit_aces", resplit_aces ? "true" : "false");
    cJSON_AddStringToObject(json, "hit_split_aces", hit_split_aces ? "true" : "false");
    cJSON_AddNumberToObject(json, "blackjack_bets", blackjack_bets);
    cJSON_AddNumberToObject(json, "blackjack_pays", blackjack_pays);
    cJSON_AddNumberToObject(json, "penetration", penetration);

    char* jsonString = cJSON_Print(json);
    snprintf(buffer, buffer_size, "%s", jsonString);
    free(jsonString);
    cJSON_Delete(json);
}

