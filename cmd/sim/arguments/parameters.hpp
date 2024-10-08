#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>
#include <ctime>
#include <cstdint>
#include "rules.hpp"
#include "logger.hpp"

// Parameters class to store simulation parameters
class Parameters {
  public:
	Parameters(std::string name, std::string decks, std::string strategy, int number_of_decks, int64_t number_of_hands, Rules *rules, Logger *logger);

  public:
	Rules *rules;
	Logger *logger;

	std::string name;
	std::string playbook;
	std::string processor;
	std::string timestamp;
	std::string decks;
	std::string strategy;
	int number_of_decks;
	int64_t number_of_hands;

  public:
	void print();
	std::string serialize();

  private:
	void getCurrentTime(std::string &buffer);
};

#endif // PARAMETERS_HPP
