#include "dealer.hpp"

Dealer::Dealer(bool h17) : hitSoft17(h17) {
	hand.reset();
}

Dealer::~Dealer() {
	// Destructor will automatically free resources if necessary (nothing to do here for now)
}

void Dealer::reset() {
	hand.reset();
}

void Dealer::drawCard(Card *card) {
	hand.drawCard(card);
}

void Dealer::play(Shoe *shoe) {
	while (!shouldStand()) {
		drawCard(shoe->drawCard());
	}
}

bool Dealer::shouldStand() const {
	if (hitSoft17 && hand.isSoft17()) {
		return false;
	}
	return hand.getHandTotal() >= 17;
}

