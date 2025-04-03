#include <string>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <sstream> 
#include <iomanip> 
#include <cstdlib>
#include <nlohmann/json.hpp>
#include "parameters.hpp"

//
Parameters::Parameters(Arguments *arguments)
		: decks(arguments->getDecks()), strategy(arguments->getStrategy()), number_of_decks(arguments->getNumberOfDecks()),
		  number_of_hands(arguments->getNumberOfHands()), number_of_threads(arguments->getNumberOfThreads()) {
	generateName();
	snprintf(playbook, sizeof(playbook), "%s-%s", decks.c_str(), strategy.c_str());
	snprintf(processor, sizeof(processor), "%s", STRIKER_WHO_AM_I.c_str());
    share_of_hands = number_of_hands / number_of_threads;
	verbose = number_of_threads == 1;
	getCurrentTime();
}

//
void Parameters::print() {
	printf("    %-26s: %s\n", "Processor", processor);
	printf("    %-26s: %ld\n", "Threads", number_of_threads);
	printf("    %-26s: %s\n", "Name", name);
	printf("    %-26s: %s\n", "Version", STRIKER_VERSION.c_str());
	printf("    %-26s: %s\n", "Playbook", playbook);
	printf("    %-26s: %s\n", "Decks", decks.c_str());
	printf("    %-26s: %s\n", "Strategy", strategy.c_str());
	printf("    %-26s: %17s\n", "Number of hands", formatWithCommas(number_of_hands).c_str());
	printf("    %-26s: %17s\n", "Thread's share of hands", formatWithCommas(share_of_hands).c_str());
	printf("    %-26s: %s\n", "Timestamp", timestamp);
}

//
void Parameters::getCurrentTime() {
	time_t rawtime;
	struct tm *timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(timestamp, sizeof(timestamp), TIME_LAYOUT.c_str(), timeinfo);
}

//
void Parameters::generateName() {
	std::time_t t = std::time(nullptr);
	struct tm* tm_info = std::localtime(&t);

	int year = tm_info->tm_year + 1900;
	int month = tm_info->tm_mon + 1;
	int day = tm_info->tm_mday;

	std::snprintf(name, sizeof(name), "%s_%4d_%02d_%02d_%012ld", STRIKER_WHO_AM_I.c_str(), year, month, day, t);
}

/*
//
void Parameters::serialize(char *buffer, int buffer_size) {
	nlohmann::json json;

	json["name"] = name;
	json["playbook"] = playbook;
	json["processor"] = processor;
	json["timestamp"] = timestamp;
	json["decks"] = decks.c_str();
	json["strategy"] = strategy.c_str();
	json["number_of_hands"] = number_of_hands;
	json["number_of_decks"] = number_of_decks;

	std::string jsonString = json.dump();
	std::snprintf(buffer, buffer_size, "%s", jsonString.c_str());
}
*/

