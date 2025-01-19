#ifndef SHOE_HPP
#define SHOE_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include "card.hpp"

#define MINIMUM_CARD_VALUE 2
#define MAXIMUM_CARD_VALUE 11

class Shoe {
	public:
		Shoe(int number_of_decks, float penetration);  // Constructor
		~Shoe();

	private:
		std::vector<Card*> cards;	// Cards currently in the shoe
		bool force_shuffle = false;	// Flag to force a shuffle
		int number_of_cards;		// Total number of cards
		int cut_card;				// The cut card position in the shoe
		int burn_card = 1;
		int next_card;
		int last_discard;

	public:
		Card *drawCard();
		void shuffle();
		void shuffle_random();
		bool shouldShuffle();
		bool isAce(const Card *card);
		int getNumberOfCards() {
			return number_of_cards;
		}
		void display() {
			int index = 0;
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			for (const auto& card : cards) {
				std::cout << std::setfill('0') << std::setw(3) << std::right << index++ << ": ";
				card->display();
			}
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
		}
};

#endif // SHOE_HPP
