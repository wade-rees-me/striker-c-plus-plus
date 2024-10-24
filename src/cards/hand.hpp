#ifndef HAND_HPP
#define HAND_HPP

#include <vector>
#include "shoe.hpp"

// Hand class definition
class Hand {
  public:
	Hand();

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
	int *getHaveCards() {
		return have_cards;
	}
	void surrender() {
		hand_surrendered = true;
	}
	bool didSurrender() const {
		return hand_surrendered;
	}

  private:
	std::vector<Card*> cards;
	int have_cards[13] = {0};
	int hand_total = 0;
	int soft_ace = false;
	bool hand_surrendered = false;

  private:
	void calculateTotal();
};

#endif // HAND_HPP
