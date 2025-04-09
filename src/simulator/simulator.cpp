#include "simulator.hpp"
#include "constants.hpp"
#include "player.hpp"
#include "table.hpp"
#include <cjson/cJSON.h>
#include <cstdio>
#include <ctime>
#include <curl/curl.h>
#include <iostream>
#include <string>

//
Simulator::Simulator(Parameters *parameters, Rules *rules, Strategy *strategy) : parameters(parameters), rules(rules) {
    table = new Table(parameters, rules, strategy);
    report = Report();
}

// The simulator process function
Simulator *Simulator::simulatorRunOnce() {
    simulatorRunSimulation();
    return this;
}

// Function to run the simulation
void Simulator::simulatorRunSimulation() {
    if (parameters->verbose) {
        std::cout << "    Start: " << parameters->strategy << " table session" << std::endl;
    }
    table->session(strcmp(parameters->strategy, "mimic") == 0);
    if (parameters->verbose) {
        std::cout << "    End: table session" << std::endl;
    }

    report.total_bet += table->getPlayer()->getReport()->total_bet;
    report.total_won += table->getPlayer()->getReport()->total_won;
    report.total_rounds += table->getReport()->total_rounds;
    report.total_hands += table->getReport()->total_hands;
    report.total_blackjacks += table->getPlayer()->getReport()->total_blackjacks;
    report.total_doubles += table->getPlayer()->getReport()->total_doubles;
    report.total_splits += table->getPlayer()->getReport()->total_splits;
    report.total_wins += table->getPlayer()->getReport()->total_wins;
    report.total_pushes += table->getPlayer()->getReport()->total_pushes;
    report.total_loses += table->getPlayer()->getReport()->total_loses;
    report.duration += table->getReport()->duration;
}

