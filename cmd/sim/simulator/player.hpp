#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "strategy.hpp"
#include "shoe.hpp"
#include "hand.hpp"
#include "report.hpp"
#include "wager.hpp"
#include "parameters.hpp"
#include "constants.hpp"

//
class Player {
public:
	Player(Parameters* params, Rules* rules, int num_cards);

	void shuffle();
	void placeBet(bool mimic);
	void insurance();
	void play(Card* up, Shoe* shoe, bool mimic);
	void playSplit(Wager* w, Shoe* shoe, Card* up);
	void drawCard(Hand* hand, Card* card);
	void showCard(Card* card);
	bool bustedOrBlackjack() const;
	void payoff(bool dealer_blackjack, bool dealer_busted, int dealer_total);
	void payoffHand(Wager* w, bool dealer_blackjack, bool dealer_busted, int dealer_total);

//private:
	Parameters* parameters;
	Rules* rules;
	int number_of_cards;

	Wager wager;
	std::vector<Wager*> splits;
	Strategy strategy;
	Report report = Report();

	int seen_cards[13] = {0};  // Keeps track of the cards the player has seen

	void splitHand(Card* up, Shoe* shoe, Wager* wager);
	void payoffSplit(Wager* wager, bool dealer_busted, int dealer_total);
	void executeStand(int have_cards[], Card* up, Shoe* shoe);
	void getHave(Hand* hand);
	bool mimicStand();
};

#endif // PLAYER_HPP
