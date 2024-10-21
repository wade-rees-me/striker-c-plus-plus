#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>
#include "parameters.hpp"
#include "rules.hpp"
#include "table.hpp"
#include "report.hpp"
#include "simulation.hpp"
#include "strategy.hpp"

//
class Simulator {
  public:
	Simulator(Parameters* params, Rules* rules, Strategy* strategy);

	void simulatorRunOnce();

  public:
	Parameters* parameters;
	Rules* rules;
	Table* table;
	Report report;

  private:
	void simulatorRunSimulation();
	void simulatorInsert(Simulation* sdt, std::string playbook);
};

#endif // SIMULATOR_HPP
