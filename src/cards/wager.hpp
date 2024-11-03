#ifndef WAGER_HPP
#define WAGER_HPP

#include <cstdint>
#include "hand.hpp"

//
class Wager : public Hand {
	public:
		Wager(int64_t minimum_bet, int64_t maximum_bet);

	private:
		int64_t minimum_bet;
		int64_t maximum_bet;
		int64_t amount_bet;
		int64_t amount_won;
		int64_t insurance_bet;
		int64_t insurance_won;

	public:
		void reset();
		void placeAmountBet(int64_t bet) { // Round up to nearest even number
			amount_bet = (std::min(maximum_bet, std::max(minimum_bet, bet)) + 1) / 2 * 2;
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
