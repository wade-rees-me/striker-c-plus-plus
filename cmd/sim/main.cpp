#include <iostream>
#include <cstdlib>
#include "arguments.hpp"
#include "rules.hpp"
#include "parameters.hpp"
#include "simulator.hpp"

//
int main(int argc, char* argv[]) {
	Arguments arguments;
	arguments.parseArguments(argc, argv);

	std::locale loc("");	// Set the global locale to the default system locale, which typically uses commas
	std::cout.imbue(loc);	// Imbue the locale into std::cout

	Rules rules;
	rules.rulesLoadTable(arguments.getDecks());
	Parameters params(arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks(), arguments.getRounds());

	std::cout << "Starting: striker-c++ ..." << std::endl;
	std::cout << std::endl;
	params.print(24);
	rules.print(24);
	std::cout << std::endl;

	Simulator sim(&params, &rules);
	sim.simulatorRunOnce();

	std::cout << "\n\nComplete: striker-c++ ..." << std::endl;
	return 0;
}

