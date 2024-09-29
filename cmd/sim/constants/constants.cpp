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

std::string getSimulationDirectory() {
	return std::getenv("STRIKER_SIMULATIONS");
}

int64_t getDatabaseMinimumRounds() {
	char* mr = std::getenv("STRIKER_MINIMUM_ROUNDS");
	char* end;
	return std::strtoll(mr, &end, 10);
}

