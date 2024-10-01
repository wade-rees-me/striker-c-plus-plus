#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "arguments.hpp"
#include "rules.hpp"
#include "parameters.hpp"
#include "logger.hpp"
#include "simulator.hpp"
#include "constants.hpp"

std::string generateName();

//
int main(int argc, char* argv[]) {
	std::string name = generateName();
	Arguments arguments;
	arguments.parseArguments(argc, argv);

	Logger logger(name, arguments.getHands() < 1000);
	std::locale loc("");	// Set the global locale to the default system locale, which typically uses commas
	std::cout.imbue(loc);	// Imbue the locale into std::cout

	Rules rules;
	rules.rulesLoadTable(arguments.getDecks());
	Parameters params(name, arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks(), arguments.getHands(), &rules, &logger);

	logger.simulation("Starting: " + name + " ...\n\n");
	logger.simulation("  -- arguments -------------------------------------------------------------------\n");
	params.print();
	rules.print(&logger);
	logger.simulation("  --------------------------------------------------------------------------------\n\n");

	Simulator sim(&params);
	sim.simulatorRunOnce();

	logger.simulation("\nComplete: " + name + " ...\n");
	return 0;
}

//
std::string generateName() {
	std::time_t t = std::time(nullptr);
	struct tm* tm_info = std::localtime(&t);
    char buffer[256];

	int year = tm_info->tm_year + 1900;
	int month = tm_info->tm_mon + 1;
	int day = tm_info->tm_mday;

	std::snprintf(&buffer[0], 128, "%s_%4d_%02d_%02d_%012ld", STRIKER_WHO_AM_I.c_str(), year, month, day, t);
    return std::string(buffer);
}

