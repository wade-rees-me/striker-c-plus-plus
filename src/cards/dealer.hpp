#ifndef DEALER_HPP
#define DEALER_HPP

#include "hand.hpp"
#include "shoe.hpp"

//
class Dealer {
	public:
		Dealer(bool hitSoft17);

	private:
		Hand hand;
		bool hitSoft17 = true;

	public:
		bool shouldStand() const;
		void reset() {
			hand.reset();
		}
		void drawCard(Card *card) {
			hand.drawCard(card);
		}
		bool isBlackjack() {
			return hand.isBlackjack();
		}
		bool isBusted() {
			return hand.isBusted();
		}
		int getHandTotal() {
			return hand.getHandTotal();
		}
};

#endif // DEALER_HPP
