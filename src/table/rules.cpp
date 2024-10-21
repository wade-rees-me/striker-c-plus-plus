#include <iostream>
#include <iomanip> 
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <exception>
#include "rules.hpp"

//
Rules::Rules(const std::string &decks) : Request() {
	try {
        fetchJson(getRulesUrl() + "/" + decks);
		fetchTable();
	}
	catch (std::exception fault) {
		std::cerr << "Error fetching rules table: " << fault.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//
void Rules::fetchTable() {
    auto itemPayload = jsonResponse["payload"];
    if (itemPayload.is_null()) {
        throw std::runtime_error("Error fetching rules table payload");
    }

    auto jsonPayload = nlohmann::json::parse(itemPayload.get<std::string>(), nullptr, false);
    if (jsonPayload.is_discarded()) {
        throw std::runtime_error("Error parsing JSON payload");
    }

    // Extract values from JSON and set member variables
	std::snprintf(playbook, sizeof(playbook), "%s", jsonPayload["playbook"].get<std::string>().c_str());
    hit_soft_17 = jsonPayload.value("hitSoft17", false);
    surrender = jsonPayload.value("surrender", false);
    double_any_two_cards = jsonPayload.value("doubleAnyTwoCards", false);
    double_after_split = jsonPayload.value("doubleAfterSplit", false);
    resplit_aces = jsonPayload.value("resplitAces", false);
    hit_split_aces = jsonPayload.value("hitSplitAces", false);
    blackjack_bets = jsonPayload.value("blackjackBets", 1);
    blackjack_pays = jsonPayload.value("blackjackPays", 1);
    penetration = jsonPayload.value("penetration", 0.65f);
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
void Rules::serialize(char* buffer, int buffer_size) {
    nlohmann::json json;

    json["hit_soft_17"] = hit_soft_17 ? "true" : "false";
    json["surrender"] = surrender ? "true" : "false";
    json["double_any_two_cards"] = double_any_two_cards ? "true" : "false";
    json["double_after_split"] = double_after_split ? "true" : "false";
    json["resplit_aces"] = resplit_aces ? "true" : "false";
    json["hit_split_aces"] = hit_split_aces ? "true" : "false";
    json["blackjack_bets"] = blackjack_bets;
    json["blackjack_pays"] = blackjack_pays;
    json["penetration"] = penetration;

    std::string jsonString = json.dump();
    std::snprintf(buffer, buffer_size, "%s", jsonString.c_str());
}

