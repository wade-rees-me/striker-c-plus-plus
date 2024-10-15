#include <iostream>
#include <cstdio>
#include <iomanip>
#include <ctime>
#include <string>
#include "table.hpp"
#include "strategy.hpp"
#include "shoe.hpp"

//
Table::Table(Parameters* params, Rules* rules) : parameters(params) {
	shoe = new Shoe(parameters->number_of_decks, rules->penetration);
	dealer = new Dealer(rules->hit_soft_17);
	player = new Player(parameters, rules, shoe->getNumberOfCards());
	report = Report();
}

//
Table::~Table() {
	delete shoe;
	delete dealer;
	delete player;
}

// Function to simulate a session
void Table::session(bool mimic) {
    char buffer[256];
	std::snprintf(buffer, sizeof(buffer), "      Start: table, playing %lld hands\n", parameters->number_of_hands);
	std::cout << buffer;

	report.start = std::time(nullptr);
	while (report.total_hands < parameters->number_of_hands) {
		status(report.total_rounds, report.total_hands);
		shoe->shuffle();
		player->shuffle();
		report.total_rounds++;

		while (!shoe->shouldShuffle()) {
			report.total_hands++;
			dealer->reset();
			player->placeBet(mimic);

			Card* up = dealCards(&player->wager);
			if (!mimic && up->isAce()) {
				player->insurance();
			}

			if (!dealer->hand.isBlackjack()) {
				player->play(up, shoe, mimic);
				if (!player->bustedOrBlackjack()) {
					dealer->play(shoe);
				}
			}

			player->payoff(dealer->hand.isBlackjack(), dealer->hand.isBusted(), dealer->hand.getHandTotal());
			player->showCard(up);
		}
	}
	std::cout << "\n";

	report.end = std::time(nullptr);
	report.duration = report.end - report.start;
	std::snprintf(buffer, sizeof(buffer), "      End: table\n");
	std::cout << buffer;
}

// Function to deal cards
Card* Table::dealCards(Hand* hand) {
	player->drawCard(hand, shoe->drawCard());
	Card* up = shoe->drawCard();
	dealer->drawCard(up);
	player->drawCard(hand, shoe->drawCard());
	dealer->drawCard(shoe->drawCard());
	player->showCard(up);
	return up;
}

// Function to show cards
void Table::show(Card* card) {
	player->showCard(card);
}

//
void Table::status(int64_t round, int64_t hand) {
	if(round == 0) {
		std::cout << std::string("        ") << std::flush;
	}
	if((round + 1) % STATUS_DOT == 0) {
		std::cout << std::string(".") << std::flush;
	}
	if((round + 1) % STATUS_LINE == 0) {
    	char buffer[256];
		std::snprintf(buffer, sizeof(buffer), " : %lld (rounds), %lld (hands)\n", (round + 1), hand);
		std::cout << buffer;
		//std::cout << std::string(" : ") + std::string(round + 1) + std::string" (rounds), ") + std::string(hand) + std::string" (hands)\n"));
		std::cout << std::string("        ") << std::flush;
	}
}

