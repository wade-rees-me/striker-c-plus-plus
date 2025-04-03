#ifndef REPORT_HPP
#define REPORT_HPP

#include <string>
#include <ctime>
#include <cstdint>
#include "parameters.hpp"
#include "rules.hpp"
#include "constants.hpp"

class Report {
	public:
		void merge(Report *a);
		void print(int number_of_threads);
		void insert(Parameters *parameters, Rules *rules);

	public:
		char advantage[MAX_STRING_SIZE];
		char total_time[MAX_STRING_SIZE];
		char average_time[MAX_STRING_SIZE];
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
		int total_threads = 0;
		time_t start = 0;
		time_t end = 0;
		int64_t duration = 0;
};

#endif // REPORT_HPP
