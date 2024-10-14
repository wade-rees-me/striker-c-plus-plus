#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>
#include <ctime>
#include <cstdint>
#include "constants.hpp"

// Parameters class to store simulation parameters
class Parameters {
  public:
	Parameters(std::string decks, std::string strategy, int number_of_decks, int64_t number_of_hands);

  public:
    char name[MAX_STRING_SIZE];
    char playbook[MAX_STRING_SIZE];
    char processor[MAX_STRING_SIZE];
    char timestamp[MAX_STRING_SIZE];
    std::string decks;
    std::string strategy;
	int number_of_decks;
	int64_t number_of_hands;

  public:
	void print();
	std::string serialize();

  private:
	void getCurrentTime(char* buffer);
	void generateName(char* buffer);
};

#endif // PARAMETERS_HPP
