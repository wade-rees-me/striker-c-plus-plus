#include <iostream>
#include <ctime>
#include <cstdio>
#include <string>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "table.hpp"
#include "player.hpp"
#include "simulator.hpp"
#include "constants.hpp"

//
Simulator::Simulator(Parameters* parameters, Rules* rules, Strategy* strategy) : parameters(parameters), rules(rules) {
	table = new Table(parameters, rules, strategy);
	report = Report();
}

// The simulator process function
void Simulator::simulatorRunOnce() {
	Simulation simulation;
    char buffer[MAX_BUFFER_SIZE];

    std::snprintf(buffer, sizeof(buffer), "  Start: simulation %s", parameters->name);
	std::cout << buffer << std::endl;
	simulatorRunSimulation();
    std::snprintf(buffer, sizeof(buffer), "  End: simulation");
	std::cout << buffer << std::endl;

	simulation.playbook = parameters->playbook;
	simulation.name = parameters->name;
	simulation.simulator = STRIKER_WHO_AM_I;
	simulation.summary = "no";
	simulation.simulations = "1";
	parameters->serialize(simulation.parameters, sizeof(simulation.parameters));
	rules->serialize(simulation.rules, sizeof(simulation.rules));

	// Format rounds, hands, and other values
	std::snprintf(simulation.rounds, sizeof(simulation.rounds), "%lld", report.total_rounds);
	std::snprintf(simulation.hands, sizeof(simulation.hands), "%lld", report.total_hands);
	std::snprintf(simulation.total_bet, sizeof(simulation.total_bet), "%lld", report.total_bet);
	std::snprintf(simulation.total_won, sizeof(simulation.total_won), "%lld", report.total_won);
	std::snprintf(simulation.total_time, sizeof(simulation.total_time), "%lld", report.duration);
	std::snprintf(simulation.average_time, sizeof(simulation.average_time), "%06.2f seconds", (float)report.duration * 1000000 / (float)report.total_hands);
	std::snprintf(simulation.advantage, sizeof(simulation.advantage), "%+04.3f %%", ((double)report.total_won / report.total_bet) * 100);

	// Print out the results
    printf("\n  -- results ---------------------------------------------------------------------\n");
    printf("    %-24s: %lld\n", "Number of hands", report.total_hands);
    printf("    %-24s: %lld\n", "Number of rounds", report.total_rounds);
    printf("    %-24s: %lld %+04.3f average bet per hand\n", "Total bet", report.total_bet, (double)report.total_bet / report.total_hands);
    printf("    %-24s: %lld %+04.3f average win per hand\n", "Total won", report.total_won, (double)report.total_won / report.total_hands);
    printf("    %-24s: %s seconds\n", "Total time", simulation.total_time);
    printf("    %-24s: %s per 1,000,000 hands\n", "Average time", simulation.average_time);
    printf("    %-24s: %s\n", "Player advantage", simulation.advantage);
    printf("  --------------------------------------------------------------------------------\n");

	if(report.total_hands >= DATABASE_NUMBER_OF_HANDS) {
		simulatorInsert(&simulation, parameters->playbook);
	}
}

// Function to run the simulation
void Simulator::simulatorRunSimulation() {
	std::cout << "    Start: " + parameters->strategy + " table session" << std::endl;
	table->session(parameters->strategy == "mimic");
	std::cout << "    End: table session" << std::endl;

	report.total_bet += table->player->report.total_bet;
	report.total_won += table->player->report.total_won;
	report.total_rounds += table->report.total_rounds;
	report.total_hands += table->report.total_hands;
	report.duration += table->report.duration;
}

// Function to insert simulation into the database (HTTP POST)
void Simulator::simulatorInsert(Simulation* simulation, std::string playbook) {
	struct curl_slist* headers = nullptr;
	CURL* curl;

	MemoryStruct chunk;
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		char url[MAX_BUFFER_SIZE];
		snprintf(url, sizeof(url), "http://%s/%s/%s/%s", getSimulationUrl().c_str(), simulation->simulator.c_str(), playbook.c_str(), simulation->name.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, url);

    	std::cout << " -- insert ----------------------------------------------------------------------\n";
		// Set headers
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Convert Simulation to JSON
    	nlohmann::json json;

		json["playbook"] = simulation->playbook.c_str();
		json["name"] = simulation->name.c_str();
		json["simulator"] = simulation->simulator.c_str();
		json["summary"] = "no";
		json["simulations"] = "1";
		json["rounds"] = simulation->rounds;
		json["hands"] = simulation->hands;
		json["total_bet"] = simulation->total_bet;
		json["total_won"] = simulation->total_won;
		json["advantage"] = simulation->advantage;
		json["total_time"] = simulation->total_time;
		json["average_time"] = simulation->average_time;
		json["parameters"] = simulation->parameters;
		json["rules"] = simulation->rules;
		json["payload"] = "n/a";

    	std::string jsonString = json.dump();
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cout << std::string("curl -X POST ") + std::string(url) + std::string(" -H \"Content-Type: application/json\" -d") + jsonString + std::string("\n\n");
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
		}

		curl_easy_cleanup(curl);
    	std::cout << "  --------------------------------------------------------------------------------\n";
	}

	curl_global_cleanup();
}

