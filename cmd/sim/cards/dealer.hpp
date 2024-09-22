#ifndef DEALER_HPP
#define DEALER_HPP

#include "hand.hpp"
#include "shoe.hpp"

//
class Dealer {
  public:
	Dealer(bool h17);
	~Dealer();

  public:
	void reset();
	void drawCard(Card* card);
	void play(Shoe* shoe);

  //private:
	Hand hand;
	bool hitSoft17 = true;

  private:
	bool shouldStand() const;
};

#endif // DEALER_HPP
