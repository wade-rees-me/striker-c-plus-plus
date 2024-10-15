#ifndef WAGER_HPP
#define WAGER_HPP

#include <cstdint>
#include "hand.hpp"
#include "constants.hpp"

//
class Wager : public Hand {
  public:
	Wager();

  public:
	int64_t amount_bet;		// The amount bet
	int64_t amount_won;		// The amount won
	int64_t insurance_bet;  // The insurance bet
	int64_t insurance_won;  // The insurance winnings

  public:
	void reset();
	void splitHand(Wager* split);

	void placeBet(int64_t bet) {
		amount_bet = (std::min(MAXIMUM_BET, std::max(MINIMUM_BET, bet)) + 1) / 2 * 2;
	}
	void doubleBet() {
		amount_bet *= 2;
	}
	void wonBlackjack(int64_t pays, int64_t bet) {
		amount_won = (amount_bet * pays) / bet;
	}
	void won() {
		amount_won = amount_bet;
	}
	void lost() {
		amount_won = -amount_bet;
	}
	void push() { // No action needed for a push in this case
	}
	void wonInsurance() {
		insurance_won = insurance_bet * 2;
	}
	void lostInsurance() {
		insurance_won = -insurance_bet;
	}
};

#endif // WAGER_HPP
