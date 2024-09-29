#include <iostream>
#include <ctime>
#include <cstdio>
#include <string>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "table.hpp"
#include "player.hpp"
#include "simulator.hpp"
#include "utilities.hpp"
#include "constants.hpp"

//
Simulator::Simulator(Parameters* parameters) : parameters(parameters), name(128, '\0') {
	std::time_t t = std::time(nullptr);
	struct tm* tm_info = std::localtime(&t);

	year = tm_info->tm_year + 1900;
	month = tm_info->tm_mon + 1;
	day = tm_info->tm_mday;
	std::snprintf(&name[0], 128, "striker-c-plus-plus_%4d_%02d_%02d_%012ld", year, month, day, t);

	table = new Table(parameters);
	report = Report();
}

// The simulator process function
void Simulator::simulatorRunOnce() {
	SimulationDatabaseTable dbTable;

	parameters->logger->simulation("  Starting striker-c++ simulation(" + name + ")...\n");
	simulatorRunSimulation();
	parameters->logger->simulation("  Ending striker-c++ simulation(" + name + ")...\n");

	// Populate SimulationDatabaseTable
	dbTable.playbook = parameters->playbook.c_str();
	dbTable.guid = parameters->guid.c_str();
	dbTable.simulator = STRIKER_WHO_AM_I;
	dbTable.summary = "no";
	dbTable.simulations = "1";
	dbTable.parameters = parameters->serialize();

	// Format rounds, hands, and other values
	std::snprintf(dbTable.rounds, 128, "%lld", report.total_rounds);
	std::snprintf(dbTable.hands, 128, "%lld", report.total_hands);
	std::snprintf(dbTable.total_bet, 128, "%lld", report.total_bet);
	std::snprintf(dbTable.total_won, 128, "%lld", report.total_won);
	std::snprintf(dbTable.total_time, 128, "%lld", report.duration);
	std::snprintf(dbTable.average_time, 128, "%06.2f seconds", (float)report.duration * 1000000 / (float)report.total_hands);
	std::snprintf(dbTable.advantage, 128, "%+04.3f%%", ((double)report.total_won / report.total_bet) * 100);

	// Print out the results
    char buffer[256];

    parameters->logger->simulation("\n  -- results ---------------------------------------------------------------------\n");
    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Number of rounds", Utilities::addCommas(report.total_rounds).c_str());
	parameters->logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Number of hands", Utilities::addCommas(report.total_hands).c_str());
	parameters->logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Total bet", Utilities::addCommas(report.total_bet).c_str());
	parameters->logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Total won", Utilities::addCommas(report.total_won).c_str());
	parameters->logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Total time", Utilities::addCommas(report.duration).c_str());
	parameters->logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Average time", std::string(dbTable.average_time).c_str());
	parameters->logger->simulation(buffer);

    std::snprintf(buffer, sizeof(buffer), "    %-24s: %s\n", "Player advantage", std::string(dbTable.advantage).c_str());
	parameters->logger->simulation(buffer);
    parameters->logger->simulation("  --------------------------------------------------------------------------------\n\n");


	if(report.total_rounds >= getDatabaseMinimumRounds()) {
		simulatorInsert(&dbTable, parameters->playbook);
	}
}

// Function to run the simulation
void Simulator::simulatorRunSimulation() {
	parameters->logger->simulation("    Starting (" + parameters->strategy + ") table session...\n");
	table->session(parameters->strategy == "mimic");
	parameters->logger->simulation("\n    Ending (" + parameters->strategy + ") table session...\n");

	report.total_bet += table->player->report.total_bet;
	report.total_won += table->player->report.total_won;
	report.total_rounds += table->report.total_rounds;
	report.total_hands += table->report.total_hands;
	report.duration += table->report.duration;
}

// Function to insert simulation into the database (HTTP POST)
void Simulator::simulatorInsert(SimulationDatabaseTable* sdt, std::string playbook) {
	CURL* curl;
	struct curl_slist* headers = nullptr;
	MemoryStruct chunk;

	chunk.memory = static_cast<char*>(malloc(1));
	chunk.size = 0;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		char url[256];
		snprintf(url, sizeof(url), "http://%s/%s/%s/%s", getSimulationUrl().c_str(), sdt->simulator.c_str(), playbook.c_str(), sdt->guid.c_str());
		curl_easy_setopt(curl, CURLOPT_URL, url);

		// Set headers
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Convert SimulationDatabaseTable to JSON
		cJSON* json = cJSON_CreateObject();
		cJSON_AddStringToObject(json, "playbook", sdt->playbook.c_str());
		cJSON_AddStringToObject(json, "guid", sdt->guid.c_str());
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
		parameters->logger->insert(std::string("curl -X POST ") + std::string(url) + std::string(" -H \"Content-Type: application/json\" -d") + std::string(jsonStr) + std::string("\n\n"));

		std::cout << "Insert results into the database..." << std::endl;
		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
		}

		curl_easy_cleanup(curl);
		free(chunk.memory);
		cJSON_Delete(json);
		free(jsonStr);
	}

	curl_global_cleanup();
}

