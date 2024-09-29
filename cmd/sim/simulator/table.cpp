#include <iostream>
#include <cstdio>
#include <iomanip>
#include <ctime>
#include <string>
#include "table.hpp"
#include "strategy.hpp"
#include "shoe.hpp"
#include "utilities.hpp"

const int STATUS_DOT = 25000;
const int STATUS_LINE = 1000000;

//
Table::Table(Parameters* params) : parameters(params) {
	shoe = new Shoe(parameters->number_of_decks, parameters->rules->penetration);
	dealer = new Dealer(parameters->rules->hit_soft_17);
	player = new Player(parameters, shoe->getNumberOfCards());
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
	parameters->logger->simulation(std::string("      Starting table (") + (mimic ? "mimic" : "strategy") + ") rounds: " + Utilities::addCommas(parameters->rounds) + "\n");

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
	parameters->logger->simulation("\n");

	report.end = std::time(nullptr);
	report.duration = report.end - report.start;
	parameters->logger->simulation(std::string("      Ending table (") + (mimic ? "mimic" : "strategy") + ")");
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
		parameters->logger->simulation(std::string("      "));
	}
	if((round + 1) % STATUS_DOT == 0) {
		parameters->logger->simulation(std::string("."));
	}
	if((round + 1) % STATUS_LINE == 0) {
		parameters->logger->simulation(std::string(" : " + Utilities::addCommas(round + 1) + "\n"));
		parameters->logger->simulation(std::string("      "));
	}
}

