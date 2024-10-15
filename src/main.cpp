#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <format>
#include "arguments.hpp"
#include "rules.hpp"
#include "parameters.hpp"
#include "simulator.hpp"
#include "constants.hpp"

//
int main(int argc, char* argv[]) {
	Arguments arguments(argc, argv);
	Parameters parameters(arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks(), arguments.getHands());
	Rules rules(arguments.getDecks());
	Simulator simulator(&parameters, &rules);

	//std::locale loc("");	// Set the global locale to the default system locale, which typically uses commas
	//std::cout.imbue(loc);	// Imbue the locale into std::cout

	std::cout << "Start: {}\n" << STRIKER_WHO_AM_I << std::endl;
	std::cout << "  -- arguments -------------------------------------------------------------------\n";
	parameters.print();
	rules.print();
	std::cout << "  --------------------------------------------------------------------------------\n\n";

	simulator.simulatorRunOnce();

	std::cout << "End: {}\n" << STRIKER_WHO_AM_I << std::endl;
	return 0;
}

