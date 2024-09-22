#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include "shoe.hpp"

// Define the suits array
const std::vector<std::string> suits = {"spades", "diamonds", "clubs", "hearts"};

// Define the card ranks and values
const std::vector<Rank> ranks = {
	{"two", 2, 0},
	{"three", 3, 1},
	{"four", 4, 2},
	{"five", 5, 3},
	{"six", 6, 4},
	{"seven", 7, 5},
	{"eight", 8, 6},
	{"nine", 9, 7},
	{"ten", 10, 8},
	{"jack", 10, 9},
	{"queen", 10, 10},
	{"king", 10, 11},
	{"ace", 11, 12}
};

// Constructor: Create a new shoe of cards
Shoe::Shoe(int number_of_decks, float penetration) {
	for (int i = 0; i < number_of_decks; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 13; k++) {
				Card* card = new Card(suits[j], ranks[k].getRank(), ranks[k].getValue(), ranks[k].getOffset());
				cards.push_back(card);
			}
		}
	}
	number_of_cards = cards.size();
	next_card = number_of_cards;
	last_discard = number_of_cards;
	cut_card = static_cast<int>(number_of_cards * penetration);

	//display();
	shuffle();
}

// Shuffle the shoe
Shoe::~Shoe() {
	for (Card* card : cards) {
		delete card;
	}
}

// Shuffle the shoe
void Shoe::shuffle() {
	last_discard = number_of_cards;
	force_shuffle = false;
	shuffle_random();
}

// Fisher-Yates shuffle algorithm for shuffling cards
void Shoe::shuffle_random() {
	static std::default_random_engine rng(std::random_device{}());
	std::shuffle(cards.begin(), cards.begin() + last_discard, rng);
	next_card = burn_card;
	//display();
}

// Draw a card from the shoe
Card* Shoe::drawCard() {
	if(next_card >= number_of_cards) {
		force_shuffle = true;
		shuffle_random();
	}
	Card* card = cards[next_card];
	next_card++;
	return card;
}

// Check if the shoe should be shuffled
bool Shoe::shouldShuffle() {
	last_discard = next_card;
	return (next_card >= cut_card) || force_shuffle;
}

// Check if a card is an ace
bool Shoe::isAce(const Card* card) {
	return card->isAce();
}

