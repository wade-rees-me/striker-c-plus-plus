#include <string>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <sstream> 
#include <iomanip> 
#include <cstdlib>
#include <curl/curl.h>
#include "report.hpp"
#include "constants.hpp"

//
void Report::merge(Report *a) {
	this->total_rounds += a->total_rounds;
	this->total_hands += a->total_hands;
	this->total_bet += a->total_bet;
	this->total_won += a->total_won;
	this->total_blackjacks += a->total_blackjacks;
	this->total_doubles += a->total_doubles;
	this->total_splits += a->total_splits;
	this->total_wins += a->total_wins;
	this->total_loses += a->total_loses;
	this->total_pushes += a->total_pushes;
}

// Print out the results
void Report::print(int number_of_threads) {
    std::snprintf(total_time, sizeof(total_time), "%ld", duration);
    std::snprintf(average_time, sizeof(average_time), "%.0f", (float)duration * BILLION / (float)total_hands);
    std::snprintf(advantage, sizeof(advantage), "%+08.3f %%", ((double)total_won / total_bet) * 100);
	total_threads = number_of_threads;

	printf("\n  -- results ---------------------------------------------------------------------\n");
	printf("    %-26s: %17s\n", "Number of hands", formatWithCommas(total_hands).c_str());
	printf("    %-26s: %17s\n", "Number of rounds", formatWithCommas(total_rounds).c_str());
	printf("    %-26s: %17s %+08.3f average bet per hand\n", "Total bet", formatWithCommas(total_bet).c_str(), (double)total_bet / total_hands);
	printf("    %-26s: %17s %+08.3f average win per hand\n", "Total won", formatWithCommas(total_won).c_str(), (double)total_won / total_hands);
	printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of blackjacks", formatWithCommas(total_blackjacks).c_str(), (double)total_blackjacks / total_hands * 100.0);
	printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of doubles", formatWithCommas(total_doubles).c_str(), (double)total_doubles / total_hands * 100.0);
	printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of splits", formatWithCommas(total_splits).c_str(), (double)total_splits / total_hands * 100.0);
	printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of wins", formatWithCommas(total_wins).c_str(), (double)total_wins / total_hands * 100.0);
	printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of pushes", formatWithCommas(total_pushes).c_str(), (double)total_pushes / total_hands * 100.0);
	printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of loses", formatWithCommas(total_loses).c_str(), (double)total_loses / total_hands * 100.0);
	printf("    %-26s: %17s seconds\n", "Total time", formatWithCommas(duration).c_str());
	printf("    %-26s: %17s threads\n", "Number of threads", formatWithCommas(total_threads).c_str());
	printf("    %-26s: %17s seconds per %s hands\n", "Average time", formatWithCommas((int)((float)duration * BILLION / (float)total_hands)).c_str(), formatWithCommas(BILLION).c_str());
	printf("    %-26s: %17s %s\n", "Player advantage", "", advantage);
	printf("  --------------------------------------------------------------------------------\n");
}

// Function to insert simulation into the database (HTTP POST)
void Report::insert(Parameters *parameters, Rules *rules) {
	struct curl_slist* headers = nullptr;
	CURL* curl;

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if (curl) {
		char url[MAX_BUFFER_SIZE];
		char buffer[MAX_BUFFER_SIZE];
		snprintf(url, sizeof(url), "http://%s/%s/%s/%s", getSimulationsUrl().c_str(), parameters->processor, parameters->playbook, parameters->name);
		curl_easy_setopt(curl, CURLOPT_URL, url);

		std::cout << std::endl << " -- insert ----------------------------------------------------------------------\n";
		// Set headers
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		// Convert Simulation to JSON
		nlohmann::json json;

		json["guid"] = parameters->name;
		json["version"] = STRIKER_VERSION;
		json["simulator"] = parameters->processor;
		json["threads"] = total_threads;
		json["playbook"] = parameters->playbook;
		json["decks"] = parameters->decks;
		json["strategy"] = parameters->strategy;
		json["rounds"] = total_rounds;
		json["hands"] = total_hands;
		json["total_bet"] = total_bet;
		json["total_won"] = total_won;
		json["advantage"] = advantage;
		json["start"] = start;
		json["end"] = end;
		json["duration"] = duration;
		rules->serialize(buffer, sizeof(buffer));
		json["rules"] = buffer;

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

