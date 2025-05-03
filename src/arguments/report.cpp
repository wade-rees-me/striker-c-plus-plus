#include "report.hpp"
#include "constants.hpp"
#include <cstdint>
#include <cstdlib>
#include <curl/curl.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

//
void Report::init(Parameters *parameters, int64_t startTime) {
    strcpy(this->name, parameters->name);
    strcpy(this->version, STRIKER_VERSION.c_str());
    strcpy(this->simulator, parameters->processor);
    strcpy(this->playbook, parameters->playbook);
    strcpy(this->strategy, parameters->strategy);
    strcpy(this->decks, parameters->decks);
    strcpy(this->epoch, parameters->epoch);
    this->total_threads = parameters->number_of_threads;
    this->start = startTime;
}

//
void Report::merge(Report *a) {
    this->total_rounds += a->total_rounds;
    this->total_hands += a->total_hands;
    this->total_bet += a->total_bet;
    this->total_won += a->total_won;
    this->total_blackjacks += a->total_blackjacks;
    this->total_doubles += a->total_doubles;
    this->total_splits += a->total_splits;
    this->total_splits_ace += a->total_splits_ace;
    this->total_wins += a->total_wins;
    this->total_loses += a->total_loses;
    this->total_pushes += a->total_pushes;
}

//
void Report::finish(int64_t endTime) {
    this->end = endTime;
    this->duration = endTime - this->start;
    this->advantage = ((double)this->total_won / this->total_bet) * 100;
    this->per_billion = ((float)this->duration * (float)BILLION / (float)this->total_hands);
}

// Print out the results
void Report::print() {
    printf("    %-26s: %17s\n", "Number of hands", formatWithCommas(total_hands).c_str());
    printf("    %-26s: %17s\n", "Number of rounds", formatWithCommas(total_rounds).c_str());
    printf("    %-26s: %17s %+08.3f average bet per hand\n", "Total bet", formatWithCommas(total_bet).c_str(),
           (double)total_bet / total_hands);
    printf("    %-26s: %17s %+08.3f average win per hand\n", "Total won", formatWithCommas(total_won).c_str(),
           (double)total_won / total_hands);
    printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of blackjacks",
           formatWithCommas(total_blackjacks).c_str(), (double)total_blackjacks / total_hands * 100.0);
    printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of doubles", formatWithCommas(total_doubles).c_str(),
           (double)total_doubles / total_hands * 100.0);
    printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of splits", formatWithCommas(total_splits).c_str(),
           (double)total_splits / total_hands * 100.0);
    printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of splits - Aces",
           formatWithCommas(total_splits_ace).c_str(), (double)total_splits_ace / total_hands * 100.0);
    printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of wins", formatWithCommas(total_wins).c_str(),
           (double)total_wins / total_hands * 100.0);
    printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of pushes", formatWithCommas(total_pushes).c_str(),
           (double)total_pushes / total_hands * 100.0);
    printf("    %-26s: %17s %+08.3f %% of total hands\n", "Number of loses", formatWithCommas(total_loses).c_str(),
           (double)total_loses / total_hands * 100.0);
    printf("    %-26s: %17s seconds\n", "Total time", formatWithCommas(duration).c_str());
    printf("    %-26s: %17s threads\n", "Number of threads", formatWithCommas(this->total_threads).c_str());
    printf("    %-26s: %17s seconds per %s hands\n", "Average time",
           formatWithCommas((int)((float)duration * BILLION / (float)total_hands)).c_str(),
           formatWithCommas(BILLION).c_str());
    printf("    %-26s: %17s %+08.3f %%\n", "Player advantage", "", advantage);
}

// Function to insert simulation into the database (HTTP POST)
void Report::insert() {
    if (this->total_hands < NUMBER_OF_HANDS_DATABASE) {
        printf("    Error: Not enough hands played (%s). Minimum required is %s\n",
               formatWithCommas(total_hands).c_str(), formatWithCommas(NUMBER_OF_HANDS_DATABASE).c_str());
        return;
    }

    struct curl_slist *headers = nullptr;
    CURL *curl;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if (!curl) {
        std::cout << "    Curl failed to generate" << std::endl;
        return;
    }
    char url[MAX_BUFFER_SIZE];
    snprintf(url, sizeof(url), "http://%s/%s/%s/%s", getSimulationsUrl().c_str(), this->simulator, this->playbook,
             this->name);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fopen("/dev/null", "w"));
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, fopen("/dev/null", "w"));

    // Set headers
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Convert Simulation to JSON
    nlohmann::json json;
    toJsonObject(json);

    std::string jsonString = json.dump();
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        std::cout << "    Insert successful" << std::endl;
    } else {
        std::cout << "    curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

// Convert Simulation to JSON
void Report::toJsonObject(nlohmann::json json) {
    json["guid"] = this->name;
    json["version"] = STRIKER_VERSION;
    json["simulator"] = this->simulator;
    json["threads"] = this->total_threads;
    json["playbook"] = this->playbook;
    json["decks"] = this->decks;
    json["strategy"] = this->strategy;
    json["rounds"] = this->total_rounds;
    json["hands"] = this->total_hands;
    json["total_bet"] = this->total_bet;
    json["total_won"] = this->total_won;
    json["total_blackjacks"] = this->total_blackjacks;
    json["total_doubles"] = this->total_doubles;
    json["total_splits"] = this->total_splits;
    json["total_splits_ace"] = this->total_splits_ace;
    json["total_wins"] = this->total_wins;
    json["total_loses"] = this->total_loses;
    json["total_pushes"] = this->total_pushes;
    json["advantage"] = ((double)this->total_won / this->total_bet) * 100;
    json["epoch"] = this->epoch;
    json["start"] = this->start;
    json["end"] = this->end;
    json["duration"] = this->duration;
    json["per_billion"] = ((float)this->duration * (float)BILLION / (float)this->total_hands);
}

