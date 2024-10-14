#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <string>

//
class Simulation {
public:
	std::string playbook;
	std::string name;
	std::string simulator;
	std::string summary;
	std::string simulations;
	char rounds[MAX_STRING_SIZE];
	char hands[MAX_STRING_SIZE];
	char total_bet[MAX_STRING_SIZE];
	char total_won[MAX_STRING_SIZE];
	char advantage[MAX_STRING_SIZE];
	char total_time[MAX_STRING_SIZE];
	char average_time[MAX_STRING_SIZE];
	std::string parameters;
	std::string rules;
};

#endif // SIMULATION_HPP
