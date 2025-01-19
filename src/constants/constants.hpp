#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <cjson/cJSON.h>

// General constants
const static std::string STRIKER_WHO_AM_I = "striker-c-plus-plus";
const static std::string STRIKER_VERSION = "v2.02.00";	 // Epoch.Major.Minor
const static char* TIME_LAYOUT = "%Y-%m-%d %H:%M:%S %z";

// Define the maximum sizesstring fields
#define MAX_STRING_SIZE 128
#define MAX_BUFFER_SIZE 4096
#define MAX_MEMORY_SIZE 1048576

// Simulation constants
const static int64_t MAXIMUM_NUMBER_OF_HANDS = 250000000000LL;
const static int64_t MINIMUM_NUMBER_OF_HANDS = 100LL;
const static int64_t DEFAULT_NUMBER_OF_HANDS = 25000000LL;
const static int64_t DATABASE_NUMBER_OF_HANDS = 250000000LL;

// Bettting constants
const static int64_t MINIMUM_BET = 2LL;
const static int64_t MAXIMUM_BET = 80LL;
const static int64_t TRUE_COUNT_BET = 2;
const static int64_t TRUE_COUNT_MULTIPLIER = 26;

//
#define STATUS_DOT 25000
#define STATUS_LINE 1000000

// Function declarations for getting environment variables
std::string getRulesUrl();
std::string getStrategyUrl();
std::string getSimulationUrl();
const char* boolToString(bool b);
std::string toUpperCase(std::string& str);

#endif // CONSTANTS_HPP
