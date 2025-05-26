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
    table->session(strcmp(parameters->strategy, "mimic") == 0);
    report.merge(table->getPlayer()->getReport());
    report.total_rounds += table->getReport()->total_rounds;
    report.total_hands += table->getReport()->total_hands;
    return this;
}

