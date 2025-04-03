#include <cstdlib>
#include <string>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include "constants.hpp"

//
std::string getRulesUrl() {
	return std::getenv("STRIKER_URL_RULES");
}

//
std::string getChartsUrl() {
	return std::getenv("STRIKER_URL_CHARTS");
}

//
std::string getSimulationsUrl() {
	return std::getenv("STRIKER_URL_SIMULATIONS");
}

// Function to convert bool to string
const char* boolToString(bool b) {
	return b ? "true" : "false";
}

//
std::string toUpperCase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return std::toupper(c); });
	return str;
}

//
std::string removeAllSubstrings(const std::string& src, const std::string& substr) {
    std::string result;
    size_t pos = 0, found;
    
    while ((found = src.find(substr, pos)) != std::string::npos) {
        result.append(src, pos, found - pos);  // Copy up to substring
        pos = found + substr.length();        // Skip substring
    }
    result.append(src, pos);  // Copy remaining part
    return result;
}

//
std::string formatWithCommas(long long num) {
    std::string numStr = std::to_string(num);
    int insertPosition = numStr.length() - 3;
    while (insertPosition > 0) {
        numStr.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numStr;
}

//
std::string readJsonFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::in | std::ios::binary); // Open file in binary mode
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    // Read entire file into a string
    std::ostringstream buffer;
    buffer << file.rdbuf();  
    return buffer.str();
}

// Function to unescape JSON-like escape sequences
void unescape_json(std::string &str) {
    size_t i = 0;
    size_t len = str.length();

    while (i < len) {
        if (str[i] == '\\') {
            i++; // Skip the backslash

            // Handle known escape sequences
            if (i < len) {
                if (str[i] == 'n') {
                    str[i - 1] = '\n';  // Convert \n to newline (optional)
                    str.erase(i, 1);     /* Remove the 'n' */
                } else if (str[i] == '\"') {
                    str[i - 1] = '\"';  /* Convert \" to " */
                    str.erase(i, 1);    // Remove the quote
                } else if (str[i] == '\\') {
                    str[i - 1] = '\\';  /* Convert \\ to \ */
                    str.erase(i, 1);    // Remove the second backslash
                } else {
                    // Default: Just leave the character
                    i++;
                }
            }
        } else {
            i++;
        }
    }
}

// Function to strip quotes from the start and end of the string
void strip_quotes(std::string &str) {
    size_t len = str.length();
    if (len > 1 && str.front() == '"' && str.back() == '"') {
        // Remove the first and last quotes
        str = str.substr(1, len - 2);
    }
}

