#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

const static char* SUIT_SPADES = "spades";
const static char* SUIT_DIAMONDS = "diamonds";
const static char* SUIT_CLUBS = "clubs";
const static char* SUIT_HEARTS = "hearts";

const static char* RANK_TWO = "two";
const static char* RANK_THREE = "three";
const static char* RANK_FOUR = "four";
const static char* RANK_FIVE = "five";
const static char* RANK_SIX = "six";
const static char* RANK_SEVEN = "seven";
const static char* RANK_EIGHT = "eight";
const static char* RANK_NINE = "nine";
const static char* RANK_TEN = "ten";
const static char* RANK_JACK = "jack";
const static char* RANK_QUEEN = "queen";
const static char* RANK_KING = "king";
const static char* RANK_ACE = "ace";

//
class Card {
  public:
	Card(const std::string& suit, const std::string& rank, int value, int offset)
		: suit(suit), rank(rank), value(value), offset(offset) {}

  public:
	bool isAce() const {
		return value == 11;
	}
	std::string getRank() const {
		return rank;
	}
	std::string getSuit() const {
		return suit;
	}
	int getValue() const {
		return value;
	}
	int getOffset() const {
		return offset;
	}
	void display() const {
		std::cout << rank << " of " << suit << " {" << value << ", " << offset << "}" << std::endl;
	}

  private:
	std::string suit;	// Suit of the card (e.g., "hearts")
	std::string rank;	// Rank of the card (e.g., "ace")
	int value;			// Value of the card for game calculations - 2 thru 11
	int offset;			// Index of the card in a suit - 0 thru 12
};

//
const std::vector<Card> poker_cards = {
	{ SUIT_SPADES,   RANK_TWO,    2,  0},
	{ SUIT_SPADES,   RANK_THREE,  3,  1},
	{ SUIT_SPADES,   RANK_FOUR,   4,  2},
	{ SUIT_SPADES,   RANK_FIVE,   5,  3},
	{ SUIT_SPADES,   RANK_SIX,    6,  4},
	{ SUIT_SPADES,   RANK_SEVEN,  7,  5},
	{ SUIT_SPADES,   RANK_EIGHT,  8,  6},
	{ SUIT_SPADES,   RANK_NINE,   9,  7},
	{ SUIT_SPADES,   RANK_TEN,   10,  8},
	{ SUIT_SPADES,   RANK_JACK,  10,  9},
	{ SUIT_SPADES,   RANK_QUEEN, 10, 10},
	{ SUIT_SPADES,   RANK_KING,  10, 11},
	{ SUIT_SPADES,   RANK_ACE,   11, 12},
	{ SUIT_DIAMONDS, RANK_TWO,    2,  0},
	{ SUIT_DIAMONDS, RANK_THREE,  3,  1},
	{ SUIT_DIAMONDS, RANK_FOUR,   4,  2},
	{ SUIT_DIAMONDS, RANK_FIVE,   5,  3},
	{ SUIT_DIAMONDS, RANK_SIX,    6,  4},
	{ SUIT_DIAMONDS, RANK_SEVEN,  7,  5},
	{ SUIT_DIAMONDS, RANK_EIGHT,  8,  6},
	{ SUIT_DIAMONDS, RANK_NINE,   9,  7},
	{ SUIT_DIAMONDS, RANK_TEN,   10,  8},
	{ SUIT_DIAMONDS, RANK_JACK,  10,  9},
	{ SUIT_DIAMONDS, RANK_QUEEN, 10, 10},
	{ SUIT_DIAMONDS, RANK_KING,  10, 11},
	{ SUIT_DIAMONDS, RANK_ACE,   11, 12},
	{ SUIT_CLUBS,    RANK_TWO,    2,  0},
	{ SUIT_CLUBS,    RANK_THREE,  3,  1},
	{ SUIT_CLUBS,    RANK_FOUR,   4,  2},
	{ SUIT_CLUBS,    RANK_FIVE,   5,  3},
	{ SUIT_CLUBS,    RANK_SIX,    6,  4},
	{ SUIT_CLUBS,    RANK_SEVEN,  7,  5},
	{ SUIT_CLUBS,    RANK_EIGHT,  8,  6},
	{ SUIT_CLUBS,    RANK_NINE,   9,  7},
	{ SUIT_CLUBS,    RANK_TEN,   10,  8},
	{ SUIT_CLUBS,    RANK_JACK,  10,  9},
	{ SUIT_CLUBS,    RANK_QUEEN, 10, 10},
	{ SUIT_CLUBS,    RANK_KING,  10, 11},
	{ SUIT_CLUBS,    RANK_ACE,   11, 12},
	{ SUIT_HEARTS,   RANK_TWO,    2,  0},
	{ SUIT_HEARTS,   RANK_THREE,  3,  1},
	{ SUIT_HEARTS,   RANK_FOUR,   4,  2},
	{ SUIT_HEARTS,   RANK_FIVE,   5,  3},
	{ SUIT_HEARTS,   RANK_SIX,    6,  4},
	{ SUIT_HEARTS,   RANK_SEVEN,  7,  5},
	{ SUIT_HEARTS,   RANK_EIGHT,  8,  6},
	{ SUIT_HEARTS,   RANK_NINE,   9,  7},
	{ SUIT_HEARTS,   RANK_TEN,   10,  8},
	{ SUIT_HEARTS,   RANK_JACK,  10,  9},
	{ SUIT_HEARTS,   RANK_QUEEN, 10, 10},
	{ SUIT_HEARTS,   RANK_KING,  10, 11},
	{ SUIT_HEARTS,   RANK_ACE,   11, 12}
};

#endif // CARD_HPP
