#include <iostream>
#include <cstdio>
#include <iomanip>
#include <ctime>
#include <string>
#include "table.hpp"
#include "strategy.hpp"
#include "shoe.hpp"

const int STATUS_DOT = 25000;
const int STATUS_LINE = 1000000;

//
Table::Table(Parameters* params, Rules* rules) : parameters(params), rules(rules) {
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
	std::cout << "    Starting table (" << (mimic ? "mimic" : "strategy") << ") rounds: " << parameters->rounds << std::endl;

	report.start = std::time(nullptr);
	report.total_rounds = parameters->rounds;

	for (int64_t i = 0; i < parameters->rounds; ++i) {
		status(i);
		shoe->shuffle();
		player->shuffle();

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
	std::cout << std::endl;

	report.end = std::time(nullptr);
	report.duration = report.end - report.start;
	std::cout << "    Ending table (" << (mimic ? "mimic" : "strategy") << "): total elapsed time: " << report.duration << " seconds" << std::endl;
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
void Table::status(int64_t round) {
	if(round == 0) {
		std::cout << "      " << std::flush;
	}
	if((round + 1) % STATUS_DOT == 0) {
		std::cout << "." << std::flush;
	}
	if((round + 1) % STATUS_LINE == 0) {
		std::cout << " : " << std::setfill('_') << std::setw(12) << std::right << (round + 1) << std::endl;
		std::cout << "      " << std::flush;
	}
}

