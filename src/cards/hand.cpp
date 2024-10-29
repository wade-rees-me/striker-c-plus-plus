#include <iostream>
#include <vector>
#include <string>
#include "hand.hpp"

Hand::Hand() {
	reset();
}

void Hand::reset() {
	hand_total = 0;
	soft_ace = 0;
	cards.clear();
}

Card *Hand::drawCard(Card *card) {
	cards.push_back(card);
	calculateTotal();
	return card;
}

bool Hand::isBlackjack() const {
	return cards.size() == 2 && hand_total == 21;
}

bool Hand::isPair() const {
	return cards.size() == 2 && cards[0]->getRank() == cards[1]->getRank();
}

Card* Hand::getCardPair() const {
	return cards[0];
}

bool Hand::isPairOfAces() const {
	return isPair() && cards[0]->getRank() == "ace";
}

bool Hand::isBusted() const {
	return hand_total > 21;
}

bool Hand::isSoft() const {
	return soft_ace > 0;
}

bool Hand::isSoft17() const {
	return hand_total == 17 && isSoft();
}

// Split a pair from the hand
Card *Hand::splitPair() {
	if (isPair()) {
		Card *card = cards.back();
		cards.pop_back();
		calculateTotal();
		return card;
	}
	std::cerr << "Error: Trying to split a non-pair" << std::endl;
	exit(0);
}

// Recalculate the total value of the hand
void Hand::calculateTotal() {
	hand_total = 0;
	soft_ace = 0;
	for (const auto& card : cards) {
		hand_total += card->getValue();
		if (card->getValue() == 11) {
			soft_ace++;
		}
	}

	// Adjust hand total if it's over 21 and there are soft aces
	while (hand_total > 21 && soft_ace > 0) {
		hand_total -= 10;
		soft_ace--;
	}
}

