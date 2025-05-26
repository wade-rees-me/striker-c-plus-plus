#ifndef ARGUMENTS_HPP
#define ARGUMENTS_HPP

#include "constants.hpp"
#include <string>

//
class Arguments {
  public:
    Arguments(int argc, char *argv[]);

  private:
    bool mimic_flag = false;
    bool basic_flag = false;
    bool neural_flag = false;
    bool linear_flag = false;
    bool polynomial_flag = false;
    bool high_low_flag = false;
    bool wong_flag = false;
    bool single_deck_flag = false;
    bool double_deck_flag = false;
    bool six_shoe_flag = false;
    int64_t number_of_hands = NUMBER_OF_HANDS_DEFAULT;
    int64_t number_of_threads = NUMBER_OF_CORES_DEFAULT;

  public:
    std::string getStrategy() const;
    std::string getDecks() const;
    int getNumberOfDecks() const;
    int64_t getNumberOfHands() const { return number_of_hands; }
    int64_t getNumberOfThreads() const { return number_of_threads; }

  private:
    void printVersion() const;
    void printHelpMessage() const;
};

#endif // ARGUMENTS_HPP
