#include <iostream>
#include <ctime>
#include <cstdio>
#include <string>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "table.hpp"
#include "player.hpp"
#include "simulator.hpp"

//
Simulator::Simulator(Parameters* parameters, Rules* rules) : parameters(parameters), rules(rules), name(128, '\0') {
	std::time_t t = std::time(nullptr);
	struct tm* tm_info = std::localtime(&t);

	year = tm_info->tm_year + 1900;
	month = tm_info->tm_mon + 1;
	day = tm_info->tm_mday;
	std::snprintf(&name[0], 128, "striker-c-plus-plus_%4d_%02d_%02d_%012ld", year, month, day, t);

	table = new Table(parameters, rules);
	report = Report();
}

// The simulator process function
void Simulator::simulatorRunOnce() {
	SimulationDatabaseTable dbTable;

	std::cout << "Starting striker-c++ simulation(" << name << ")...\n";
	simulatorRunSimulation();
	std::cout << "Ending striker-c++ simulation(" << name << ")...\n\n";

	// Populate SimulationDatabaseTable
	dbTable.playbook = parameters->playbook.c_str();
	dbTable.guid = parameters->guid.c_str();
	dbTable.simulator = "StrikerWhoAmI";
	dbTable.summary = "no";
	dbTable.simulations = "1";
	dbTable.parameters = serializeParameters(parameters);

	// Format rounds, hands, and other values
	std::snprintf(dbTable.rounds, 128, "%lld", report.total_rounds);
	std::snprintf(dbTable.hands, 128, "%lld", report.total_hands);
	std::snprintf(dbTable.total_bet, 128, "%lld", report.total_bet);
	std::snprintf(dbTable.total_won, 128, "%lld", report.total_won);
	std::snprintf(dbTable.total_time, 128, "%lld", report.duration);
	std::snprintf(dbTable.average_time, 128, "%06.2f seconds", (float)report.duration * 1000000 / (float)report.total_hands);
	std::snprintf(dbTable.advantage, 128, "%+04.3f%%", ((double)report.total_won / report.total_bet) * 100);

	// Print out the results
	std::cout << "Number of rounds:  " << report.total_rounds << "\n";
	std::cout << "Number of hands:   " << report.total_hands << "\n";
	std::cout << "Total bet:         " << report.total_bet << ", average bet per hand: " << (double)report.total_bet / report.total_hands << "\n";
	std::cout << "Total won:         " << report.total_won << ", average win per hand: " << (double)report.total_won / report.total_hands << "\n";
	std::cout << "Total time:        " << dbTable.total_time << " seconds\n";
	std::cout << "Average time:      " << dbTable.average_time << " per 1,000,000 hands\n";
	std::cout << "Player advantage:  " << dbTable.advantage << "\n\n";

	simulatorInsert(&dbTable, parameters->playbook);
}

// Function to run the simulation
void Simulator::simulatorRunSimulation() {
	std::cout << "  Starting (" << parameters->strategy << ") table session...\n";
	table->session(parameters->strategy == "mimic");
	std::cout << "  Ending (" << parameters->strategy << ") table session...\n";

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
		char* jsonStr = cJSON_Print(json);

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr);

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

// Function to serialize parameters struct to JSON
std::string Simulator::serializeParameters(Parameters* parameters) {
	cJSON* json = cJSON_CreateObject();

	cJSON_AddStringToObject(json, "guid", parameters->guid.c_str());
	cJSON_AddStringToObject(json, "processor", parameters->processor.c_str());
	cJSON_AddStringToObject(json, "timestamp", parameters->timestamp.c_str());
	cJSON_AddStringToObject(json, "decks", parameters->decks.c_str());
	cJSON_AddStringToObject(json, "strategy", parameters->strategy.c_str());
	cJSON_AddStringToObject(json, "playbook", parameters->playbook.c_str());
	cJSON_AddNumberToObject(json, "rounds", parameters->rounds);
	cJSON_AddNumberToObject(json, "number_of_decks", parameters->number_of_decks);

	char* jsonString = cJSON_Print(json);
	std::string result(jsonString);

	cJSON_Delete(json);
	free(jsonString);

	return result;
}

