#ifndef REPORT_HPP
#define REPORT_HPP

#include "constants.hpp"
#include "parameters.hpp"
#include "rules.hpp"
#include <cstdint>
#include <ctime>
#include <string>

class Report {
  public:
    void init(Parameters *parameters, int64_t startTime);
    void finish(int64_t endTime);
    void merge(Report *a);
    void print();
    void insert();

  public:
    char name[MAX_STRING_SIZE];
    char version[MAX_STRING_SIZE];
    char simulator[MAX_STRING_SIZE];
    char playbook[MAX_STRING_SIZE];
    char strategy[MAX_STRING_SIZE];
    char decks[MAX_STRING_SIZE];
    char epoch[MAX_STRING_SIZE];
    int64_t total_rounds = 0;
    int64_t total_hands = 0;
    int64_t total_bet = 0;
    int64_t total_won = 0;
    int64_t total_blackjacks = 0;
    int64_t total_doubles = 0;
    int64_t total_splits = 0;
    int64_t total_wins = 0;
    int64_t total_loses = 0;
    int64_t total_pushes = 0;
    int64_t total_threads = 0;
    int64_t start = 0;
    int64_t end = 0;
    int64_t duration = 0;
    float advantage = 0.0;
    float per_billion = 0.0;
};

#endif // REPORT_HPP
