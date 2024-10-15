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
Simulator::Simulator(Parameters* parameters, Rules* rules) : parameters(parameters) {
	table = new Table(parameters, rules);
	report = Report();
}

// The simulator process function
void Simulator::simulatorRunOnce() {
	Simulation dbTable;
    char buffer[MAX_BUFFER_SIZE];

    std::snprintf(buffer, sizeof(buffer), "  Start: simulation %s\n", parameters->name);
	std::cout << buffer;
	simulatorRunSimulation();
    std::snprintf(buffer, sizeof(buffer), "  End: simulation\n");
	std::cout << buffer;

	dbTable.playbook = parameters->playbook;
	dbTable.name = parameters->name;
	dbTable.simulator = STRIKER_WHO_AM_I;
	dbTable.summary = "no";
	dbTable.simulations = "1";
	dbTable.parameters = parameters->serialize();

	// Format rounds, hands, and other values
	std::snprintf(dbTable.rounds, MAX_STRING_SIZE, "%lld", report.total_rounds);
	std::snprintf(dbTable.hands, MAX_STRING_SIZE, "%lld", report.total_hands);
	std::snprintf(dbTable.total_bet, MAX_STRING_SIZE, "%lld", report.total_bet);
	std::snprintf(dbTable.total_won, MAX_STRING_SIZE, "%lld", report.total_won);
	std::snprintf(dbTable.total_time, MAX_STRING_SIZE, "%lld", report.duration);
	std::snprintf(dbTable.average_time, MAX_STRING_SIZE, "%06.2f seconds", (float)report.duration * 1000000 / (float)report.total_hands);
	std::snprintf(dbTable.advantage, MAX_STRING_SIZE, "%+04.3f %%", ((double)report.total_won / report.total_bet) * 100);

	// Print out the results
    printf("\n  -- results ---------------------------------------------------------------------\n");
    printf("    %-24s: %lld\n", "Number of hands", report.total_hands);
    printf("    %-24s: %lld\n", "Number of rounds", report.total_rounds);
    printf("    %-24s: %lld %+04.3f average bet per hand\n", "Total bet", report.total_bet, (double)report.total_bet / report.total_hands);
    printf("    %-24s: %lld %+04.3f average win per hand\n", "Total won", report.total_won, (double)report.total_won / report.total_hands);
    printf("    %-24s: %s seconds\n", "Total time", dbTable.total_time);
    printf("    %-24s: %s per 1,000,000 hands\n", "Average time", dbTable.average_time);
    printf("    %-24s: %s\n", "Player advantage", dbTable.advantage);
    printf("  --------------------------------------------------------------------------------\n");

	if(report.total_hands >= DATABASE_NUMBER_OF_HANDS) {
		simulatorInsert(&dbTable, parameters->playbook);
	}
}

// Function to run the simulation
void Simulator::simulatorRunSimulation() {
	std::cout << "    Start: " + parameters->strategy + " table session\n";
	table->session(parameters->strategy == "mimic");
	std::cout << "    End: table session\n";

	report.total_bet += table->player->report.total_bet;
	report.total_won += table->player->report.total_won;
	report.total_rounds += table->report.total_rounds;
	report.total_hands += table->report.total_hands;
	report.duration += table->report.duration;
}

// Function to insert simulation into the database (HTTP POST)
void Simulator::simulatorInsert(Simulation* sdt, std::string playbook) {
	struct curl_slist* headers = nullptr;
	CURL* curl;

	MemoryStruct chunk;
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		char url[MAX_BUFFER_SIZE];
		snprintf(url, sizeof(url), "http://%s/%s/%s/%s", getSimulationUrl().c_str(), sdt->simulator.c_str(), playbook.c_str(), sdt->name.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, url);

    	std::cout << " -- insert ----------------------------------------------------------------------\n";
		// Set headers
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Convert Simulation to JSON
		cJSON* json = cJSON_CreateObject();
		cJSON_AddStringToObject(json, "playbook", sdt->playbook.c_str());
		cJSON_AddStringToObject(json, "name", sdt->name.c_str());
		cJSON_AddStringToObject(json, "simulator", sdt->simulator.c_str());
		cJSON_AddStringToObject(json, "summary", "no");
		cJSON_AddStringToObject(json, "simulations", "1");
		cJSON_AddStringToObject(json, "rounds", sdt->rounds);
		cJSON_AddStringToObject(json, "hands", sdt->hands);
		cJSON_AddStringToObject(json, "total_bet", sdt->total_bet);
		cJSON_AddStringToObject(json, "total_won", sdt->total_won);
		cJSON_AddStringToObject(json, "advantage", sdt->advantage);
		cJSON_AddStringToObject(json, "total_time", sdt->total_time);
		cJSON_AddStringToObject(json, "average_time", sdt->average_time);
		cJSON_AddStringToObject(json, "parameters", sdt->parameters.c_str());
		cJSON_AddStringToObject(json, "payload", "n/a");

		char* jsonStr = cJSON_Print(json);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr);

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cout << std::string("curl -X POST ") + std::string(url) + std::string(" -H \"Content-Type: application/json\" -d") + std::string(jsonStr) + std::string("\n\n");
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
		}

		curl_easy_cleanup(curl);
		cJSON_Delete(json);
		free(jsonStr);
    	std::cout << "  --------------------------------------------------------------------------------\n";
	}

	curl_global_cleanup();
}

