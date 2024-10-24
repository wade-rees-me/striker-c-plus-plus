#include <cstdlib>
#include <string>
#include <string>
#include <iostream>
#include "constants.hpp"

std::string getRulesUrl() {
	return std::getenv("STRIKER_URL_RULES");
}

std::string getStrategyUrl() {
	return std::getenv("STRIKER_URL_ACE");
}

std::string getSimulationUrl() {
	return std::getenv("STRIKER_URL_SIMULATION");
}

// Function to convert bool to string
const char* boolToString(bool b) {
    return b ? "true" : "false";
}

