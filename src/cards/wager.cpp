#include <algorithm>
#include <iostream>
#include "wager.hpp"

// Constructor: Reset the wager to its initial state
Wager::Wager() {
	reset();
}

void Wager::reset() {
	Hand::reset();
	amount_bet = 0;
	amount_won = 0;
	insurance_bet = 0;
	insurance_won = 0;
}

// Split the wager
void Wager::splitHand(Wager* split) {
	split->amount_bet = amount_bet;
	split->drawCard(splitPair());
}

