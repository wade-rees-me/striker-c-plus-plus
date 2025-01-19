#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include "request.hpp"
#include "chart.hpp"
#include "shoe.hpp"
#include "card.hpp"

//
class Strategy : public Request {
	public:
		Strategy(const std::string& decks, const std::string& strategy, int number_of_cards);

	private:
		int number_of_cards;

	public:
		std::string Playbook;
		std::vector<int> Counts;
		std::string Insurance;
		Chart SoftDouble;
		Chart HardDouble;
		Chart PairSplit;
		Chart SoftStand;
		Chart HardStand;

	public:
		int getBet(const int* seenCards);
		bool getInsurance(const int* seenCards);
		bool getDouble(const int* seenCards, const int total, bool soft, Card *up);
		bool getSplit(const int* seenCards, Card *pair, Card *up);
		bool getStand(const int* seenCards, const int total, bool soft, Card *up);

	private:
		void fetchTable(const std::string& decks, const std::string& strategy);
		bool processValue(const char* value, int trueCount, bool missing);
		int getRunningCount(const int* seenCards);
		int getTrueCount(const int* seenCards, int runningCount);
		void printCount();
};

#endif // STRATEGY_HPP
