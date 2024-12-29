#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <format>
#include "arguments.hpp"
#include "parameters.hpp"
#include "rules.hpp"
#include "strategy.hpp"
#include "simulator.hpp"
#include "constants.hpp"

//
int main(int argc, char *argv[]) {
	std::cout << "Start: " << STRIKER_WHO_AM_I << std::endl;
	Arguments arguments(argc, argv);
	Parameters parameters(arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks(), arguments.getNumberOfHands());
	Rules rules(arguments.getDecks());
	Strategy strategy(arguments.getDecks(), arguments.getStrategy(), arguments.getNumberOfDecks() * 52);
	Simulator simulator(&parameters, &rules, &strategy);

	std::cout << "  -- arguments -------------------------------------------------------------------" << std::endl;
	parameters.print();
	rules.print();
	std::cout << "  --------------------------------------------------------------------------------" << std::endl << std::endl;

	simulator.simulatorRunOnce();
	std::cout << "End: " << STRIKER_WHO_AM_I << std::endl;

	return 0;
}

