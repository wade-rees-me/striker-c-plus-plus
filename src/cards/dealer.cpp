#include "dealer.hpp"

Dealer::Dealer(bool hitSoft17)
		: hitSoft17(hitSoft17) {
	hand.reset();
}

bool Dealer::shouldStand() const {
	if (hitSoft17 && hand.isSoft17()) {
		return false;
	}
	return hand.getHandTotal() >= 17;
}

