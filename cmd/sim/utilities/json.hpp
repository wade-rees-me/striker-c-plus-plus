#ifndef JSON_HPP
#define JSON_HPP

// Function declarations
std::string parseAuxString(cJSON *json, std::string tag);
bool parseAuxBool(cJSON *json, std::string tag, bool value);
int parseAuxInt(cJSON *json, std::string tag, int value);
double parseAuxDouble(cJSON *json, std::string, double value);

#endif // JSON_HPP
