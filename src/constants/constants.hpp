#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <string>
#include <cjson/cJSON.h>

// General constants
const static std::string STRIKER_WHO_AM_I = "striker-c-plus-plus";
const static std::string STRIKER_VERSION = "v01.02.02";
const static char* TIME_LAYOUT = "%Y-%m-%d %H:%M:%S %z";

// Define the maximum sizesstring fields
#define MAX_STRING_SIZE 128
#define MAX_BUFFER_SIZE 4096
#define MAX_MEMORY_SIZE 1048576

// Simulation constants
const static int64_t MAXIMUM_NUMBER_OF_HANDS = 1000000000LL;
const static int64_t MINIMUM_NUMBER_OF_HANDS = 100LL;
const static int64_t DEFAULT_NUMBER_OF_HANDS = 1000000LL;
const static int64_t DATABASE_NUMBER_OF_HANDS = 1000000LL;

// Bettting constants
const static int64_t MAXIMUM_BET = 80LL;
const static int64_t MINIMUM_BET = 2LL;

//
#define STATUS_DOT 25000
#define STATUS_LINE 1000000

// Structure to store HTTP response
struct MemoryStruct {
    char memory[MAX_MEMORY_SIZE];
    size_t size;
};

// Function declarations for getting environment variables
std::string getRulesUrl();
std::string getStrategyUrl();
std::string getSimulationUrl();
const char* boolToString(bool b);
size_t writeMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
std::string parseAuxString(cJSON *json, std::string tag);
bool parseAuxBool(cJSON *json, std::string tag, bool value);
int parseAuxInt(cJSON *json, std::string tag, int value);
double parseAuxDouble(cJSON *json, std::string, double value);

#endif // CONSTANTS_HPP
