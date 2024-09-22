#include <cstdlib>
#include <string>
#include "constants.hpp"

std::string getRulesUrl() {
	return std::getenv("STRIKER_URL_RULES");
}

std::string getStrategyUrl() {
	return std::getenv("STRIKER_URL_ACE");
}

std::string getStrategyMlbUrl() {
	return std::getenv("STRIKER_URL_MLB");
}

std::string getSimulationUrl() {
	return std::getenv("STRIKER_URL_SIMULATION");
}

