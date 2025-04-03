#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>
#include "parameters.hpp"
#include "rules.hpp"
#include "table.hpp"
#include "report.hpp"
#include "strategy.hpp"

//
class Simulator {
	public:
		Simulator(Parameters *params, Rules *rules, Strategy *strategy);

	private:
		Parameters *parameters;
		Rules *rules;
		Table *table;
		Report report;

	public:
		Simulator* simulatorRunOnce();
		Report *getReport() {
			return &report;
		}

	private:
		void simulatorRunSimulation();
};

#endif // SIMULATOR_HPP
