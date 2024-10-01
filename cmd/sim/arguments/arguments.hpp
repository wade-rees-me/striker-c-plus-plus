#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include <string>
#include "constants.hpp"

//
class Arguments {
  public:
	void parseArguments(int argc, char *argv[]);
	std::string getStrategy() const;
	std::string getDecks() const;
	int getNumberOfDecks() const;

	int64_t getHands() const {
		return number_of_hands;
	}

  private:
	void printVersion() const;
	void printHelpMessage() const;

  private:
	bool mimic_flag = false;
	bool basic_flag = false;
	bool linear_flag = false;
	bool polynomial_flag = false;
	bool high_low_flag = false;
	bool wong_flag = false;
	bool striker_flag = false;
	bool single_deck_flag = false;
	bool double_deck_flag = false;
	bool six_shoe_flag = false;
	int64_t number_of_hands = DEFAULT_NUMBER_OF_HANDS;
};

#endif // ARGUMENTS_HPP
