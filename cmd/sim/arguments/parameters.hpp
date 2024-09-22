#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>
#include <ctime>
#include <cstdint>

// Parameters class to store simulation parameters
class Parameters {
  public:
	Parameters(std::string d, std::string s, int n, int64_t r);

  public:
	std::string playbook;
	std::string guid;
	std::string processor;
	std::string timestamp;
	std::string decks;
	std::string strategy;
	int number_of_decks;
	int64_t rounds;

  public:
	void print(int indent);

	std::string getPlaybook() const {
		return playbook;
	}

  private:
	void generateUUID(std::string& buffer);
	void getCurrentTime(std::string &buffer);
};

#endif // PARAMETERS_HPP
