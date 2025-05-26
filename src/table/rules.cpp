#include "rules.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iomanip>
#include <iostream>

//
Rules::Rules(const std::string &decks) : Request() {
    try {
        fetchJson(getRulesUrl() + "/" + decks);
        fetchTable();
    } catch (const std::exception &fault) {
        std::cerr << "Error fetching rules table: " << fault.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

//
void Rules::fetchTable() {
    // Extract values from JSON and set member variables
    std::snprintf(playbook, sizeof(playbook), "%s", jsonResponse["playbook"].get<std::string>().c_str());
    hit_soft_17 = jsonResponse.value("hitSoft17", false);
    surrender = jsonResponse.value("surrender", false);
    double_any_two_cards = jsonResponse.value("doubleAnyTwoCards", false);
    double_after_split = jsonResponse.value("doubleAfterSplit", false);
    resplit_aces = jsonResponse.value("resplitAces", false);
    hit_split_aces = jsonResponse.value("hitSplitAces", false);
    blackjack_bets = jsonResponse.value("blackjackBets", 1);
    blackjack_pays = jsonResponse.value("blackjackPays", 1);
    penetration = jsonResponse.value("penetration", 0.65f);
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

