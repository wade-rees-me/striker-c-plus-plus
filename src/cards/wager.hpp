#ifndef WAGER_HPP
#define WAGER_HPP

#include <cstdint>
#include "hand.hpp"
#include "constants.hpp"

//
class Wager : public Hand {
  public:
	Wager();

  private:
	int64_t amount_bet;		// The amount bet
	int64_t amount_won;		// The amount won
	int64_t insurance_bet;  // The insurance bet
	int64_t insurance_won;  // The insurance winnings

  public:
	void reset();
	void setAmountBet(int64_t bet) {
		amount_bet = bet;
	}
	int64_t getAmountBet() {
		return amount_bet;
	}
	void setAmountWon(int64_t won) {
		amount_won = won;
	}
	int64_t getAmountWon() {
		return amount_won;
	}
	void placeInsuranceBet() {
		insurance_bet = amount_bet / 2;
	}
	int64_t getInsuranceBet() {
		return insurance_bet;
	}
	void setInsuranceWon(int64_t won) {
		insurance_won = won;
	}
	int64_t getInsuranceWon() {
		return insurance_won;
	}
	void splitHand(Wager *split);
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
