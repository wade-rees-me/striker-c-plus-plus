#include <iostream>
#include <cstdlib>
#include <cstring>
#include "shoe.hpp"
#include "player.hpp"

// Constructor for Player
Player::Player(Rules* rules, Strategy* strategy, int number_of_cards) : rules(rules), strategy(strategy), number_of_cards(number_of_cards) {
}

// Shuffle function (reinitializes seen cards)
void Player::shuffle() {
	std::memset(seen_cards, 0, sizeof(seen_cards));
}

// Place a bet for the player
void Player::placeBet(bool mimic) {
	splits.clear();
	wager.reset();
	wager.setAmountBet(mimic ? MINIMUM_BET : strategy->getBet(seen_cards));
}

// Simulate an insurance bet
void Player::insurance() {
	if (strategy->getInsurance(seen_cards)) {
		wager.placeInsuranceBet();
	}
}

// Play the hand
void Player::play(Card* up, Shoe* shoe, bool mimic) {
	if (wager.isBlackjack()) {
		return;
	}

	if(mimic) {
		while (!mimicStand()) {
			drawCard(&wager, shoe->drawCard());
		}
		return;
	}

	if (strategy->getDouble(seen_cards, wager.getHandTotal(), wager.isSoft(), up)) {
		wager.doubleBet();
		drawCard(&wager, shoe->drawCard());
		return;
	}

	if (wager.isPair() && strategy->getSplit(seen_cards, wager.getCardPair(), up)) {
		Wager* split = new Wager();
		wager.splitHand(split);
		splits.push_back(split);

		if (wager.isPairOfAces()) {
			drawCard(&wager, shoe->drawCard());
			drawCard(split, shoe->drawCard());
			return;
		}

	  	drawCard(&wager, shoe->drawCard());
		playSplit(&wager, shoe, up);
		drawCard(split, shoe->drawCard());
		playSplit(split, shoe, up);
		return;
	}

	bool doStand = strategy->getStand(seen_cards, wager.getHandTotal(), wager.isSoft(), up);
	while (!wager.isBusted() && !doStand) {
		drawCard(&wager, shoe->drawCard());
		if (!wager.isBusted()) {
			doStand = strategy->getStand(seen_cards, wager.getHandTotal(), wager.isSoft(), up);
		}
	}
}

//
void Player::playSplit(Wager* wager, Shoe* shoe, Card* up) {
	if (wager->isPair() && strategy->getSplit(seen_cards, wager->getCardPair(), up)) {
		Wager* split = new Wager();
		splits.push_back(split);
		wager->splitHand(split);

  		drawCard(wager, shoe->drawCard());
		playSplit(wager, shoe, up);
		drawCard(split, shoe->drawCard());
		playSplit(split, shoe, up);
		return;
	}

	bool doStand = strategy->getStand(seen_cards, wager->getHandTotal(), wager->isSoft(), up);
	while (!wager->isBusted() && !doStand) {
		drawCard(wager, shoe->drawCard());
		if (!wager->isBusted()) {
			doStand = strategy->getStand(seen_cards, wager->getHandTotal(), wager->isSoft(), up);
		}
	}
}

// Draw a card
void Player::drawCard(Hand* hand, Card* card) {
	showCard(card);
	hand->drawCard(card);
}

// Show the card
void Player::showCard(Card* card) {
	seen_cards[card->getOffset()]++;
}

// Check if player busted or has blackjack
bool Player::bustedOrBlackjack() const {
	if (splits.size() == 0) {
		return wager.isBusted() || wager.isBlackjack();
	}

	for (const auto& split : splits) {
		if (!split->isBusted()) {
			return false;
		}
	}
	return true;
}

//
void Player::payoff(bool dealer_blackjack, bool dealer_busted, int dealer_total) {
	if (splits.size() == 0) {
		payoffHand(&wager, dealer_blackjack, dealer_busted, dealer_total);
		return;
	}

	for (const auto& split : splits) {
		payoffSplit(split, dealer_busted, dealer_total);
	}
}

//
void Player::payoffHand(Wager* wager, bool dealer_blackjack, bool dealer_busted, int dealer_total) {
	if (dealer_blackjack) {
		wager->wonInsurance();
		if (wager->isBlackjack()) {
			wager->push();
		} else {
			wager->lost();
		}
	} else {
		wager->lostInsurance();
		if (wager->isBlackjack()) {
			wager->wonBlackjack(rules->blackjack_pays, rules->blackjack_bets);
		} else if (wager->isBusted()) {
			wager->lost();
		} else if (dealer_busted || (wager->getHandTotal() > dealer_total)) {
			wager->won();
		} else if (dealer_total > wager->getHandTotal()) {
			wager->lost();
		} else {
			wager->push();
		}
	}

	report.total_bet += wager->getAmountBet() + wager->getInsuranceBet();
	report.total_won += wager->getAmountWon() + wager->getInsuranceWon();
}

//
void Player::payoffSplit(Wager* wager, bool dealer_busted, int dealer_total) {
	if (wager->isBusted()) {
		wager->lost();
	} else if (dealer_busted || (wager->getHandTotal() > dealer_total)) {
		wager->won();
	} else if (dealer_total > wager->getHandTotal()) {
		wager->lost();
	} else {
		wager->push();
	}

	report.total_won += wager->getAmountWon();
	report.total_bet += wager->getAmountBet();
}

//
bool Player::mimicStand() {
	if (wager.isSoft17()) {
		return false;
	}
	return wager.getHandTotal() >= 17;
}

