#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

//
class Card {
	public:
		Card(const std::string& suit, const std::string& rank, int value, int offset)
				: suit(suit), rank(rank), value(value), offset(offset) {}

	private:
		std::string suit;	// Suit of the card (e.g., "hearts")
		std::string rank;	// Rank of the card (e.g., "ace")
		int value;			// Value of the card for game calculations - 2 thru 11
		int offset;			// Index of the card in a suit - 0 thru 12

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
};

#endif // CARD_HPP
