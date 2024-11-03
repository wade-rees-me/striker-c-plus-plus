#ifndef REPORT_HPP
#define REPORT_HPP

#include <string>
#include <ctime>
#include <cstdint>

class Report {
	public:
		int64_t total_rounds = 0;
		int64_t total_hands = 0;
		int64_t total_bet = 0;
		int64_t total_won = 0;
		time_t start = 0;
		time_t end = 0;
		int64_t duration = 0;
};

#endif // REPORT_HPP
