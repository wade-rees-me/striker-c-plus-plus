#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "shoe.hpp"
#include "hand.hpp"
#include "report.hpp"
#include "wager.hpp"
#include "rules.hpp"
#include "strategy.hpp"
#include "constants.hpp"

//
class Player {
	public:
		Player(Rules *rules, Strategy *strategy, int number_of_cards);

	private:
		Rules *rules;
		Strategy *strategy;
		Wager wager;
		std::vector<Wager*> splits;
		Report report = Report();
		int number_of_cards;
		int seen_cards[MAXIMUM_CARD_VALUE + 1] = {0};  // Keeps track of the cards the player has seen
		int initial_bet;

	public:
		void shuffle();
		void placeBet(bool mimic);
		void insurance(bool dealer_blackjack);
		void play(Card *up, Shoe *shoe, bool mimic);
		void playSplit(Wager *w, Shoe *shoe, Card *up);
		void drawCard(Hand *hand, Card *card);
		void showCard(Card *card);
		bool bustedOrBlackjack() const;
		void payoff(bool dealer_blackjack, bool dealer_busted, int dealer_total);
		void payoffHand(Wager *wager, bool dealer_blackjack, bool dealer_busted, int dealer_total);
		Wager *getWager() {
			return &wager;
		}
		Report *getReport() {
			return &report;
		}

	private:
		void splitHand(Card *up, Shoe *shoe, Wager *wager);
		void payoffSplit(Wager *wager, bool dealer_busted, int dealer_total);
		bool mimicStand();
};

#endif // PLAYER_HPP
