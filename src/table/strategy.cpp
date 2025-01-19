#include <algorithm>
#include <cstdint>
#include "strategy.hpp"
#include "constants.hpp"

void strategyLoadTable(const std::map<std::string, std::vector<std::string>>& strategy, Chart *chart);

//
Strategy::Strategy(const std::string& decks, const std::string& strategy, const int number_of_cards)
		: Request(), number_of_cards(number_of_cards), SoftDouble("Soft Double"), HardDouble("Hard Double"), PairSplit("Pair Split"), SoftStand("Soft Stand"), HardStand("Hard Stand") {
	try {
		if (strcasecmp("mimic", strategy.c_str()) != 0) {
			fetchJson("http://localhost:57910/striker/v1/strategy");
			fetchTable(decks, strategy);

			SoftDouble.print();
			HardDouble.print();
			PairSplit.print();
			SoftStand.print();
			HardStand.print();
			printCount();
		}
	}
	catch (std::exception fault) {
		std::cerr << "Error fetching strategy table: " << fault.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

//
int Strategy::getBet(const int *seenCards) {
	return getTrueCount(seenCards, getRunningCount(seenCards)) * TRUE_COUNT_BET;
}

//
bool Strategy::getInsurance(const int *seenCards) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	return processValue(Insurance.c_str(), trueCount, false);
}

//
bool Strategy::getDouble(const int *seenCards, const int total, bool soft, Card *up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", total);
	std::string key = buffer;
	if (soft) {
		return processValue(SoftDouble.getValue(key, up->getValue()).c_str(), trueCount, false);
	}
	return processValue(HardDouble.getValue(key, up->getValue()).c_str(), trueCount, false);
}

//
bool Strategy::getSplit(const int *seenCards, Card *pair, Card *up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%s", (pair->getKey().c_str()));
	std::string key = buffer;
	return processValue(PairSplit.getValue(key, up->getValue()).c_str(), trueCount, false);
}

//
bool Strategy::getStand(const int *seenCards, const int total, bool soft, Card *up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", total);
	std::string key = buffer;
	if (soft) {
		return processValue(SoftStand.getValue(key, up->getValue()).c_str(), trueCount, true);
	}
	return processValue(HardStand.getValue(key, up->getValue()).c_str(), trueCount, true);
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
	   		Insurance = jsonPayload["insurance"].get<std::string>();
	   		Counts = jsonPayload["counts"].get<std::vector<int>>();
			Counts.insert(Counts.begin(), 0);
			Counts.insert(Counts.begin(), 0);

	   		strategyLoadTable(jsonPayload["soft-double"].get<const std::map<std::string, std::vector<std::string>>>(), &SoftDouble);
	   		strategyLoadTable(jsonPayload["hard-double"].get<const std::map<std::string, std::vector<std::string>>>(), &HardDouble);
	   		strategyLoadTable(jsonPayload["pair-split"].get<const std::map<std::string, std::vector<std::string>>>(), &PairSplit);
	   		strategyLoadTable(jsonPayload["soft-stand"].get<const std::map<std::string, std::vector<std::string>>>(), &SoftStand);
	   		strategyLoadTable(jsonPayload["hard-stand"].get<const std::map<std::string, std::vector<std::string>>>(), &HardStand);

			return;
		}
   	}
}

void strategyLoadTable(const std::map<std::string, std::vector<std::string>>& strategy, Chart *chart) {
	for (auto& pair : strategy) {  // Range-based for loop
		const std::string& key = pair.first; // Access the key
		const std::vector<std::string>& values = pair.second;  // Access the values

		int index = MINIMUM_CARD_VALUE;
		for (const std::string& value : values) {  // Loop through the vector
			chart->insert(key, index++, value);
		}
	}
}

//
int Strategy::getRunningCount(const int *seenCards) {
	int running = 0;
	for (int i = MINIMUM_CARD_VALUE; i <= MAXIMUM_CARD_VALUE; i++) {
		running += Counts[i] * seenCards[i];
	}
	return running;
}

//
int Strategy::getTrueCount(const int *seenCards, int runningCount) {
	int unseen = number_of_cards;
	for (int i = MINIMUM_CARD_VALUE; i <= MAXIMUM_CARD_VALUE; i++) {
		unseen -= seenCards[i];
	}
	if (unseen > 0) {
		return int(float(runningCount) / (float(unseen) / float(TRUE_COUNT_MULTIPLIER)));
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
		if (strcasecmp("no", value) == 0 || strcasecmp("n", value) == 0 ) {
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

void Strategy::printCount() {
	printf("Counts\n");
	printf("--------------------2-----3-----4-----5-----6-----7-----8-----9-----X-----A---\n");
	printf("     ");
	for (int i = 0; i <= MAXIMUM_CARD_VALUE; i++) {
		printf("%4d, ", Counts[i]);
	}
	printf("\n");
	printf("------------------------------------------------------------------------------\n\n");
}

