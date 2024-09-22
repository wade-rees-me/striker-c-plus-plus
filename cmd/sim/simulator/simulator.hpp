#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>
#include "table.hpp"
#include "report.hpp"
#include "memory.hpp"
#include "parameters.hpp"

//
class SimulationDatabaseTable {
public:
	std::string playbook;
	std::string guid;
	std::string simulator;
	std::string summary;
	std::string simulations;
	char rounds[128];
	char hands[128];
	char total_bet[128];
	char total_won[128];
	char advantage[128];
	char total_time[128];
	char average_time[128];
	std::string parameters;
};

//
class Simulator {
  public:
	Simulator(Parameters* params, Rules* rules);

	void simulatorRunOnce();

  public:
	Parameters* parameters;
	Rules* rules;
	Table* table;
	Report report;

  private:
	std::string name;
	int year;
	int month;
	int day;

  private:
	void simulatorRunSimulation();
	std::string serializeParameters(Parameters* parameters);
	void simulatorInsert(SimulationDatabaseTable* sdt, std::string playbook);
};

#endif // SIMULATOR_HPP
