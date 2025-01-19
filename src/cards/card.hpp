#ifndef CARD_HPP
#define CARD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

//
class Card {
	public:
		Card(const std::string& suit, const std::string& rank, const std::string& key, int value)
				: suit(suit), rank(rank), key(key), value(value) {}

	private:
		std::string suit;	// Suit of the card (e.g., "hearts")
		std::string rank;	// Rank of the card (e.g., "ace")
		std::string key;
		int value;			// Value of the card for game calculations - 2 thru 11

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
		std::string getKey() const {
			return key;
		}
		int getValue() const {
			return value;
		}
		void display() const {
			std::cout << rank << " of " << suit << " {" << value << "}" << std::endl;
		}
};

#endif // CARD_HPP
