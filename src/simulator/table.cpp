#include "table.hpp"
#include "shoe.hpp"
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

//
Table::Table(Parameters *parameters, Rules *rules, Strategy *strategy) : parameters(parameters) {
    shoe = new Shoe(parameters->number_of_decks, rules->penetration);
    dealer = new Dealer(rules->hit_soft_17);
    player = new Player(rules, strategy, shoe->getNumberOfCards());
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
    report.start = std::time(nullptr);
    while (report.total_hands < parameters->share_of_hands) {
        if (parameters->verbose) {
            status(report.total_rounds, report.total_hands);
        }
        shoe->shuffle();
        player->shuffle();
        report.total_rounds++;

        while (!shoe->shouldShuffle()) {
            report.total_hands++;
            dealer->reset();
            player->placeBet(mimic);
            dealCards(player->getWager());

            if (!mimic && up->isAce()) {
                player->insurance(dealer->isBlackjack());
            }

            if (!dealer->isBlackjack()) {
                player->play(up, shoe, mimic);
                if (!player->bustedOrBlackjack()) {
                    while (!dealer->shouldStand()) {
                        Card *card = shoe->drawCard();
                        dealer->drawCard(card);
                        player->showCard(card);
                    }
                }
            }

            player->showCard(down);
            player->payoff(dealer->isBlackjack(), dealer->isBusted(), dealer->getHandTotal());
        }
    }
    if (parameters->verbose) {
        printf("\r");
    }

    report.end = std::time(nullptr);
    report.duration = report.end - report.start;
}

// Function to deal cards
void Table::dealCards(Hand *hand) {
    player->drawCard(hand, shoe->drawCard());
    up = shoe->drawCard();
    dealer->drawCard(up);
    player->showCard(up);

    player->drawCard(hand, shoe->drawCard());
    down = shoe->drawCard();
    dealer->drawCard(down);
}

//
void Table::status(int64_t round, int64_t hand) {
    const char *spinner = "|/-\\";
    printf("\r%c Simulating...", spinner[hand % 4]);
    fflush(stdout);
}

