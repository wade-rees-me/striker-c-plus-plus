#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>
#include "shoe.hpp"

//
const std::string SPADES = "spades";
const std::string DIAMONDS = "diamonds";
const std::string CLUBS = "clubs";
const std::string HEARTS = "hearts";

//
const std::string TWO = "two";
const std::string THREE = "three";
const std::string FOUR = "four";
const std::string FIVE = "five";
const std::string SIX = "six";
const std::string SEVEN = "seven";
const std::string EIGHT = "eight";
const std::string NINE = "nine";
const std::string TEN = "ten";
const std::string JACK = "jack";
const std::string QUEEN = "queen";
const std::string KING = "king";
const std::string ACE = "ace";

// Define the suits array
const std::vector<std::string> suits = {SPADES, DIAMONDS, CLUBS, HEARTS};

// Constructor: Create a new shoe of cards
Shoe::Shoe(int number_of_decks, float penetration) {
	for (int i = 0; i < number_of_decks; i++) {
		for (int j = 0; j < 4; j++) {
			cards.push_back(new Card(suits[j], TWO, "2", 2));
			cards.push_back(new Card(suits[j], THREE, "3", 3));
			cards.push_back(new Card(suits[j], FOUR, "4", 4));
			cards.push_back(new Card(suits[j], FIVE, "5", 5));
			cards.push_back(new Card(suits[j], SIX, "6", 6));
			cards.push_back(new Card(suits[j], SEVEN, "7", 7));
			cards.push_back(new Card(suits[j], EIGHT, "8", 8));
			cards.push_back(new Card(suits[j], NINE, "9", 9));
			cards.push_back(new Card(suits[j], TEN, "X", 10));
			cards.push_back(new Card(suits[j], JACK, "X", 10));
			cards.push_back(new Card(suits[j], QUEEN, "X", 10));
			cards.push_back(new Card(suits[j], KING, "X", 10));
			cards.push_back(new Card(suits[j], ACE, "A", 11));
		}
	}
	number_of_cards = cards.size();
	next_card = number_of_cards;
	last_discard = number_of_cards;
	cut_card = static_cast<int>(number_of_cards * penetration);

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
bool Shoe::isAce(const Card *card) {
	return card->isAce();
}

