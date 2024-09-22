#include <string>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <iomanip> 
#include <cstdlib>
#include <uuid/uuid.h>
#include "parameters.hpp"
#include "constants.hpp"

Parameters::Parameters(std::string d, std::string s, int n, int64_t r) : decks(d), strategy(s), number_of_decks(n), rounds(r) {
	playbook = decks + std::string("-") + strategy;
	generateUUID(guid);
	processor = STRIKER_WHO_AM_I;
	getCurrentTime(timestamp);
}

void Parameters::print(int indent) {
	std::cout << std::left << std::setw(indent) << "Playbook" << ": " << playbook << std::endl;
	std::cout << std::left << std::setw(indent) << "Processor" << ": " << processor << std::endl;
	std::cout << std::left << std::setw(indent) << "Number of rounds" << ": " << rounds << std::endl;
	std::cout << std::left << std::setw(indent) << "Timestamp" << ": " << timestamp << std::endl;
	std::cout << std::left << std::setw(indent) << "Guid" << ": " << guid << std::endl;
}

// Function to generate a UUID
void Parameters::generateUUID(std::string& buffer) {
	uuid_t uuid;
	char uuid_str[37];
	uuid_generate(uuid);
	uuid_unparse(uuid, uuid_str);
	buffer = uuid_str;
}

// Function to get current time and format it
void Parameters::getCurrentTime(std::string &buffer) {
	char time_str[128];
	time_t t = time(NULL);
	struct tm *tm_info = localtime(&t);
	strftime(time_str, sizeof(time_str), TIME_LAYOUT, tm_info);
	buffer = time_str;
}

