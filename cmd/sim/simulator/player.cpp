#include <iostream>
#include <cstdlib>
#include <cstring>
#include "shoe.hpp"
#include "player.hpp"
#include "strategy.hpp"

// Constructor for Player
Player::Player(Parameters* parameters, int number_of_cards)
	: parameters(parameters), number_of_cards(number_of_cards), strategy(parameters->getPlaybook(), number_of_cards) {
}

// Shuffle function (reinitializes seen cards)
void Player::shuffle() {
	std::memset(seen_cards, 0, sizeof(seen_cards));
}

// Place a bet for the player
void Player::placeBet(bool mimic) {
	splits.clear();
	wager.reset();
	wager.amount_bet = mimic ? MINIMUM_BET : strategy.getBet(seen_cards);;
}

// Simulate an insurance bet
void Player::insurance() {
	if (strategy.getInsurance(seen_cards)) {
		wager.insurance_bet = wager.amount_bet / 2;
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

	if (parameters->rules->surrender && strategy.getSurrender(seen_cards, wager.getHaveCards(), up)) {
		wager.surrender();
		return;
	}

	if ((parameters->rules->double_any_two_cards || wager.getHandTotal() == 10 || wager.getHandTotal() == 11) && strategy.getDouble(seen_cards, wager.getHaveCards(), up)) {
		wager.doubleBet();
		drawCard(&wager, shoe->drawCard());
		return;
	}

	if (wager.isPair() && strategy.getSplit(seen_cards, wager.getCardPair(), up)) {
		Wager* split = new Wager();
		splits.push_back(split);
		wager.splitHand(split);

		if (wager.isPairOfAces()) {
			if (!parameters->rules->resplit_aces && !parameters->rules->hit_split_aces) {
				drawCard(&wager, shoe->drawCard());
				drawCard(split, shoe->drawCard());
				return;
			}
		}

	  	drawCard(&wager, shoe->drawCard());
		playSplit(&wager, shoe, up);
		drawCard(split, shoe->drawCard());
		playSplit(split, shoe, up);
		return;
	}

	bool doStand = strategy.getStand(seen_cards, wager.getHaveCards(), up);
	while (!wager.isBusted() && !doStand) {
		drawCard(&wager, shoe->drawCard());
		doStand = strategy.getStand(seen_cards, wager.getHaveCards(), up);
	}
}

//
void Player::playSplit(Wager* wager, Shoe* shoe, Card* up) {
	if (parameters->rules->double_after_split && strategy.getDouble(seen_cards, wager->getHaveCards(), up)) {
		wager->doubleBet();
		drawCard(wager, shoe->drawCard());
		return;
	}

	if (wager->isPair()) {
		if (wager->isPairOfAces()) {
			if (parameters->rules->resplit_aces && strategy.getSplit(seen_cards, wager->getCardPair(), up)) {
				Wager* split = new Wager();
				splits.push_back(split);
				wager->splitHand(split);

	  			drawCard(wager, shoe->drawCard());
				playSplit(wager, shoe, up);
				drawCard(split, shoe->drawCard());
				playSplit(split, shoe, up);
				return;
			}
		} else {
			if (strategy.getSplit(seen_cards, wager->getCardPair(), up)) {
				Wager* split = new Wager();
				splits.push_back(split);
				wager->splitHand(split);

	  			drawCard(wager, shoe->drawCard());
				playSplit(wager, shoe, up);
				drawCard(split, shoe->drawCard());
				playSplit(split, shoe, up);
				return;
			}
		}
	}

	bool doStand = strategy.getStand(seen_cards, wager->getHaveCards(), up);
	while (!wager->isBusted() && !doStand) {
		drawCard(wager, shoe->drawCard());
		doStand = strategy.getStand(seen_cards, wager->getHaveCards(), up);
	}
}

// Draw a card
void Player::drawCard(Hand* hand, Card* card) {
	showCard(card);
	hand->drawCard(card);
}

// Show the card
void Player::showCard(Card* c) {
	seen_cards[c->getOffset()]++;
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
	if (wager->didSurrender()) {
		report.total_bet += wager->amount_bet;
		report.total_won -= wager->amount_bet / 2;
		return;
	}

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
			wager->wonBlackjack(parameters->rules->blackjack_pays, parameters->rules->blackjack_bets);
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

	report.total_bet += wager->amount_bet + wager->insurance_bet;
	report.total_won += wager->amount_won + wager->insurance_won;
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

	report.total_won += wager->amount_won;
	report.total_bet += wager->amount_bet;
}

//
bool Player::mimicStand() {
	if (wager.isSoft17()) {
		return false;
	}
	return wager.getHandTotal() >= 17;
}

