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

			if(SoftDouble.getRowCount() != 10 ||
			   HardDouble.getRowCount() != 18 ||
			   PairSplit.getRowCount() != 13 ||
			   SoftStand.getRowCount() != 10 ||
			   HardStand.getRowCount() != 18) {
				std::cerr << "Error fetching strategy table: " << strategy << std::endl;
				std::exit(EXIT_FAILURE);
			}
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
		return processValue(SoftDouble.getValue(key, up->getOffset()).c_str(), trueCount, false);
	}
	return processValue(HardDouble.getValue(key, up->getOffset()).c_str(), trueCount, false);
}

//
bool Strategy::getSplit(const int *seenCards, Card *pair, Card *up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%s", (pair->getKey().c_str()));
	std::string key = buffer;
	return processValue(PairSplit.getValue(key, up->getOffset()).c_str(), trueCount, false);
}

//
bool Strategy::getStand(const int *seenCards, const int total, bool soft, Card *up) {
	int trueCount = getTrueCount(seenCards, getRunningCount(seenCards));
	char buffer[MAX_STRING_SIZE];
	std::snprintf(buffer, sizeof(buffer), "%d", total);
	std::string key = buffer;
	if (soft) {
		return processValue(SoftStand.getValue(key, up->getOffset()).c_str(), trueCount, true);
	}
	return processValue(HardStand.getValue(key, up->getOffset()).c_str(), trueCount, true);
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

	   		strategyLoadTable(jsonPayload["soft-double"].get<const std::map<std::string, std::vector<std::string>>>(), &SoftDouble);
	   		strategyLoadTable(jsonPayload["hard-double"].get<const std::map<std::string, std::vector<std::string>>>(), &HardDouble);
	   		strategyLoadTable(jsonPayload["pair-split"].get<const std::map<std::string, std::vector<std::string>>>(), &PairSplit);
	   		strategyLoadTable(jsonPayload["soft-stand"].get<const std::map<std::string, std::vector<std::string>>>(), &SoftStand);
	   		strategyLoadTable(jsonPayload["hard-stand"].get<const std::map<std::string, std::vector<std::string>>>(), &HardStand);

	   		//SoftDouble = jsonPayload["soft-double"].get<const std::map<std::string, std::vector<std::string>>>();
	   		//HardDouble = jsonPayload["hard-double"].get<const std::map<std::string, std::vector<std::string>>>();
	   		//PairSplit = jsonPayload["pair-split"].get<const std::map<std::string, std::vector<std::string>>>();
	   		//SoftStand = jsonPayload["soft-stand"].get<const std::map<std::string, std::vector<std::string>>>();
	   		//HardStand = jsonPayload["hard-stand"].get<const std::map<std::string, std::vector<std::string>>>();

			return;
		}
   	}
}

void strategyLoadTable(const std::map<std::string, std::vector<std::string>>& strategy, Chart *chart) {
    for (auto& pair : strategy) {  // Range-based for loop
        const std::string& key = pair.first;           // Access the key
        const std::vector<std::string>& values = pair.second;  // Access the values

        //std::cout << "Key: " << key << "\nValues: ";
		int index = 0;
        for (const std::string& value : values) {  // Loop through the vector
			chart->insert(key, index++, value);
            //std::cout << value << " ";
        }
        //std::cout << "\n";
    }
}

/*
    if (strategy != NULL) {
        cJSON *key;
        cJSON *valueArray;
        cJSON_ArrayForEach(key, strategy) {
            valueArray = cJSON_GetObjectItem(strategy, key->string);
            if (valueArray != NULL) {
                cJSON *valueItem;
                int index = 0;
                cJSON_ArrayForEach(valueItem, valueArray) {
                    chart->insert(key->string, index++, valueItem->valuestring);
                }
            }
        }
    }
}
*/


//
int Strategy::getRunningCount(const int *seenCards) {
	int running = 0;
	for (int i = 0; i <= 12; i++) {
		running += Counts[i] * seenCards[i];
	}
	return running;
}

//
int Strategy::getTrueCount(const int *seenCards, int runningCount) {
	int unseen = number_of_cards;
	for (int i = 2; i <= 11; i++) {
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

