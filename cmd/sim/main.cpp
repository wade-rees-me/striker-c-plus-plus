#include <iostream>
#include <cstdlib>
#include "arguments.hpp"
#include "rules.hpp"
#include "parameters.hpp"
#include "logger.hpp"
#include "simulator.hpp"
#include "constants.hpp"

//
int main(int argc, char* argv[]) {
	Arguments arguments;
	arguments.parseArguments(argc, argv);

	Logger logger(STRIKER_WHO_AM_I, arguments.getRounds() < 1000);
	std::locale loc("");	// Set the global locale to the default system locale, which typically uses commas
	std::cout.imbue(loc);	// Imbue the locale into std::cout

	Rules rules;
	rules.rulesLoadTable(arguments.getDecks());
	Parameters params(arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks(), arguments.getRounds(), &rules, &logger);

	logger.simulation("Starting: striker-c++ ...\n\n");
	logger.simulation("  -- arguments -------------------------------------------------------------------\n");
	params.print();
	rules.print(&logger);
	logger.simulation("  --------------------------------------------------------------------------------\n\n");

	Simulator sim(&params);
	sim.simulatorRunOnce();

	logger.simulation("\nComplete: striker-c++ ...\n");
	return 0;
}

