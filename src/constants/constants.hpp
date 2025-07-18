#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cjson/cJSON.h>
#include <string>

// General constants
const static std::string STRIKER_WHO_AM_I = "striker-c-plus-plus";
const static std::string STRIKER_VERSION = "v3.00.00"; // Epoch.Major.Minor
const static std::string TIME_LAYOUT = "%Y-%m-%d %H:%M:%S %z";
const static int64_t STATUS_ROUNDS = 1000000;
const static std::string MY_HOSTNAME = "Striker";

// Define the maximum sizes-string fields
const static int64_t MAX_STRING_SIZE = 128;
const static int64_t MAX_BUFFER_SIZE = 4096;
const static int64_t MAX_MEMORY_SIZE = 1048576;

//
const static int64_t NUMBER_OF_CARDS_IN_DECK = 52;
const static int64_t NUMBER_OF_CORES_PHYSICAL = 24;
const static int64_t NUMBER_OF_CORES_LOGICAL = 32;
const static int64_t NUMBER_OF_CORES_DEFAULT = 20;

// Simulation constants
const static int64_t MILLION = 1000000LL;
const static int64_t BILLION = MILLION * 1000LL;
const static int64_t NUMBER_OF_HANDS_MAXIMUM = 10 * BILLION;
const static int64_t NUMBER_OF_HANDS_MINIMUM = 100LL;
const static int64_t NUMBER_OF_HANDS_DEFAULT = 100LL * MILLION;
const static int64_t NUMBER_OF_HANDS_DATABASE = 100LL * MILLION;

// Betting constants
const static int64_t MINIMUM_BET = 2LL;
const static int64_t MAXIMUM_BET = 20LL;
const static int64_t TRUE_COUNT_BET = 2;
const static int64_t TRUE_COUNT_MULTIPLIER = 26;

// Function declarations for getting environment variables
bool is_my_computer();
std::string getRulesUrl();
std::string getChartsUrl();
std::string getSimulationsUrl();
std::string getResourcesUrl();
const char *boolToString(bool b);
std::string toUpperCase(std::string &str);
std::string removeAllSubstrings(const std::string &src, const std::string &substr);
std::string formatWithCommas(long long num);
std::string readJsonFile(const std::string &filename);
void unescape_json(std::string &str);
void strip_quotes(std::string &str);

#endif // CONSTANTS_HPP
