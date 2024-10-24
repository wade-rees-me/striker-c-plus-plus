#include <algorithm>
#include <cstdint>
#include "strategy.hpp"
#include "constants.hpp"

//
Strategy::Strategy(const std::string& decks, const std::string& strategy, const int number_of_cards) : Request(), number_of_cards(number_of_cards) {
	try {
		fetchJson("http://localhost:57910/striker/v1/strategy");
		fetchTable(decks, strategy);
	}
	catch (std::exception fault) {
		std::cerr << "Error fetching strategy table: " << fault.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//
void Strategy::fetchTable(const std::string& decks, const std::string& strategy) {
   	for (const auto& item : jsonResponse) {
		if (decks == item["playbook"].get<std::string>() && strategy == item["hand"].get<std::string>()) {
   			nlohmann::json jsonPayload = nlohmann::json::parse(item["payload"].get<std::string>());
			if (jsonPayload.is_null()) {
				throw std::runtime_error("Error fetching strategy table payload");
			}

	   		Playbook = jsonPayload["playbook"].get<std::string>();
	   		Counts = jsonPayload["counts"].get<std::vector<int>>();
	   		Bets = jsonPayload["bets"].get<std::vector<int>>();
	   		Insurance = jsonPayload["insurance"].get<std::string>();
	   		SoftDouble = jsonPayload["soft-double"].get<const std::map<std::string, std::vector<std::string>>>();
	   		HardDouble = jsonPayload["hard-double"].get<const std::map<std::string, std::vector<std::string>>>();
	   		PairSplit = jsonPayload["pair-split"].get<const std::map<std::string, std::vector<std::string>>>();
	   		SoftStand = jsonPayload["soft-stand"].get<const std::map<std::string, std::vector<std::string>>>();
	   		HardStand = jsonPayload["hard-stand"].get<const std::map<std::string, std::vector<std::string>>>();
	   		SoftSurrender = jsonPayload["soft-surrender"].get<const std::map<std::string, std::vector<std::string>>>();
	   		HardSurrender = jsonPayload["hard-surrender"].get<const std::map<std::string, std::vector<std::string>>>();
			return;
		}
   	}
}

//
int Strategy::getBet(const int* seenCards) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	return (int)(std::min(MAXIMUM_BET, std::max(MINIMUM_BET, (long long)trueCount * 2)) + 1) / 2 * 2;
}

//
bool Strategy::getInsurance(const int* seenCards) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	return processValue(Insurance.c_str(), trueCount, false);
}

//
bool Strategy::getSurrender(const int* seenCards, const int total, bool soft, Card* up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", total);
	if (soft) {
		return processValue(SoftSurrender[buffer][up->getOffset()].c_str(), trueCount, false);
	}
	return processValue(HardSurrender[buffer][up->getOffset()].c_str(), trueCount, false);
}

//
bool Strategy::getDouble(const int* seenCards, const int total, bool soft, Card* up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", total);
	if (soft) {
		return processValue(SoftDouble[buffer][up->getOffset()].c_str(), trueCount, false);
	}
	return processValue(HardDouble[buffer][up->getOffset()].c_str(), trueCount, false);
}

//
bool Strategy::getSplit(const int* seenCards, Card* pair, Card* up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", pair->getValue());
	return processValue(PairSplit[buffer][up->getOffset()].c_str(), trueCount, false);
}

//
bool Strategy::getStand(const int* seenCards, const int total, bool soft, Card* up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", total);
	if (soft) {
		return processValue(SoftStand[buffer][up->getOffset()].c_str(), trueCount, true);
	}
	return processValue(HardStand[buffer][up->getOffset()].c_str(), trueCount, true);
}

//
int Strategy::getRunningCount(const int* seenCards) {
	int running = 0;
	for (int i = 0; i <= 12; i++) {
		running += Counts[i] * seenCards[i];
	}
	return running;
}

//
int Strategy::getTrueCount(const int* seenCards, int runningCount) {
	int unseen = number_of_cards;
	for (int i = 2; i <= 11; i++) {
		unseen -= seenCards[i];
	}
	if (unseen > 0) {
		return int(float(runningCount) / (float(unseen) / float(26)));
	}
	return 0;
}

//
bool Strategy::processValue(const char* value, int trueCount, bool missing_value) {
	try {
		if (std::string(value).empty()) {
			return missing_value;
		}
		if (strcasecmp("yes", value) == 0 || strcasecmp("y", value) == 0) {
			return true;
		}
		if (strcasecmp("no", value) || strcasecmp("n", value)) {
			return false;
		}
		if (value[0] == 'R' || value[0] == 'r') {
			return trueCount <= std::stoi(std::string(value).substr(1));
		}
		return trueCount >= std::stoi(std::string(value));
	} catch (...) {
		return missing_value;
	}
}

