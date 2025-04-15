#include "parameters.hpp"
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

//
Parameters::Parameters(Arguments *arguments)
    : number_of_decks(arguments->getNumberOfDecks()), number_of_hands(arguments->getNumberOfHands()),
      number_of_threads(arguments->getNumberOfThreads()) {
    generateName();
    snprintf(playbook, sizeof(playbook), "%s-%s", arguments->getDecks().c_str(), arguments->getStrategy().c_str());
    snprintf(processor, sizeof(processor), "%s", STRIKER_WHO_AM_I.c_str());
    snprintf(decks, sizeof(decks), "%s", arguments->getDecks().c_str());
    snprintf(strategy, sizeof(strategy), "%s", arguments->getStrategy().c_str());
    share_of_hands = (number_of_hands / number_of_threads) + 1;
    verbose = number_of_threads == 1;
    getCurrentTime();
}

//
void Parameters::print() {
    printf("    %-26s: %s\n", "Processor", processor);
    printf("    %-26s: %ld\n", "Threads", number_of_threads);
    printf("    %-26s: %s\n", "Name", name);
    printf("    %-26s: %s\n", "Version", STRIKER_VERSION.c_str());
    printf("    %-26s: %s\n", "Playbook", playbook);
    printf("    %-26s: %s\n", "Decks", decks);
    printf("    %-26s: %s\n", "Strategy", strategy);
    printf("    %-26s: %17s\n", "Number of hands", formatWithCommas(number_of_hands).c_str());
    printf("    %-26s: %17s\n", "Thread share of hands", formatWithCommas(share_of_hands).c_str());
    printf("    %-26s: %s\n", "Epoch", epoch);
}

//
void Parameters::getCurrentTime() {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(epoch, sizeof(epoch), TIME_LAYOUT.c_str(), timeinfo);
}

//
void Parameters::generateName() {
    std::time_t t = std::time(nullptr);
    struct tm *tm_info = std::localtime(&t);

    int year = tm_info->tm_year + 1900;
    int month = tm_info->tm_mon + 1;
    int day = tm_info->tm_mday;

    std::snprintf(name, sizeof(name), "%s_%4d_%02d_%02d_%012ld", STRIKER_WHO_AM_I.c_str(), year, month, day, t);
}

