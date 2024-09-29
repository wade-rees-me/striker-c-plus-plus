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
	Parameters(std::string d, std::string s, int n, int64_t r, Rules *rules, Logger *logger);

  public:
	Rules *rules;
	Logger *logger;

	std::string playbook;
	std::string guid;
	std::string processor;
	std::string timestamp;
	std::string decks;
	std::string strategy;
	int number_of_decks;
	int64_t rounds;

  public:
	void print();
	std::string serialize();

	std::string getPlaybook() const {
		return playbook;
	}

  private:
	void generateUUID(std::string& buffer);
	void getCurrentTime(std::string &buffer);
};

#endif // PARAMETERS_HPP
