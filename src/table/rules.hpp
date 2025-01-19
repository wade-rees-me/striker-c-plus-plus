#ifndef RULES_HPP
#define RULES_HPP

#include <string>
#include "request.hpp"
#include "constants.hpp"

//
class Rules : public Request {
	public:
		Rules(const std::string& decks);

	public:
		char playbook[MAX_STRING_SIZE];
		bool hit_soft_17 = true;
		bool surrender = false;
		bool double_any_two_cards = true;
		bool double_after_split = false;
		bool resplit_aces = false;
		bool hit_split_aces = false;
		int blackjack_pays = 5;
		int blackjack_bets = 3;
		float penetration = 0.70;

	public:
		void print();
		void serialize(char *buffer, int buffer_size);

	private:
		void fetchTable();
};

#endif // RULES_HPP
