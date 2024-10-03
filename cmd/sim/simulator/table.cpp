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
    char buffer[256];
	std::snprintf(buffer, sizeof(buffer), "      Start: table, playing %s hands\n", Utilities::addCommas(parameters->number_of_hands).c_str());
	parameters->logger->simulation(buffer);

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
	parameters->logger->simulation("\n");

	report.end = std::time(nullptr);
	report.duration = report.end - report.start;
	std::snprintf(buffer, sizeof(buffer), "      End: table\n");
	parameters->logger->simulation(buffer);
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
		parameters->logger->simulation(std::string("        "));
	}
	if((round + 1) % STATUS_DOT == 0) {
		parameters->logger->simulation(std::string("."));
	}
	if((round + 1) % STATUS_LINE == 0) {
		parameters->logger->simulation(std::string(" : " + Utilities::addCommas(round + 1) + " (rounds), " + Utilities::addCommas(hand) + " (hands)\n"));
		parameters->logger->simulation(std::string("        "));
	}
}

