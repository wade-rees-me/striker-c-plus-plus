#ifndef HAND_HPP
#define HAND_HPP

#include <vector>
#include "shoe.hpp"

// Hand class definition
class Hand {
	public:
		Hand();

	private:
		std::vector<Card*> cards;
		int hand_total = 0;
		int soft_ace = false;

	public:
		void reset();
		Card *drawCard(Card *card);
		Card *splitPair();
		Card *getCardPair() const;
		bool isBlackjack() const;
		bool isPair() const;
		bool isPairOfAces() const;
		bool isBusted() const;
		bool isSoft() const;
		bool isSoft17() const;
		int getHandTotal() const {
			return hand_total;
		}

	private:
		void calculateTotal();
};

#endif // HAND_HPP
