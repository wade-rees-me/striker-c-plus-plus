#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>

// General constants
const static std::string STRIKER_WHO_AM_I = "striker-c-plus-plus";
const static std::string STRIKER_VERSION = "v01.02.02";
const static char* TIME_LAYOUT;

// Simulation constants
const static int64_t MAXIMUM_NUMBER_OF_HANDS = 1000000000LL;
const static int64_t MINIMUM_NUMBER_OF_HANDS = 100LL;
const static int64_t DEFAULT_NUMBER_OF_HANDS = 1000000LL;
const static int64_t DATABASE_NUMBER_OF_HANDS = 1000000LL;

// Bettting constants
const static int64_t MAXIMUM_BET = 80LL;
const static int64_t MINIMUM_BET = 2LL;

// Function declarations for getting environment variables
std::string getRulesUrl();
std::string getStrategyUrl();
std::string getStrategyMlbUrl();
std::string getSimulationUrl();
std::string getSimulationDirectory();

#endif // CONSTANTS_HPP
