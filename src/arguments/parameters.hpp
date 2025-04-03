#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <string>
#include <ctime>
#include <cstdint>
#include "arguments.hpp"
#include "constants.hpp"

// Parameters class to store simulation parameters
class Parameters {
	public:
		Parameters(Arguments *arguments);
		//Parameters(std::string decks, std::string strategy, int number_of_decks, int64_t number_of_hands, int64_t number_of_threads);

	public:
		char name[MAX_STRING_SIZE];
		char playbook[MAX_STRING_SIZE];
		char processor[MAX_STRING_SIZE];
		char timestamp[MAX_STRING_SIZE];
		std::string decks;
		std::string strategy;
		int number_of_decks;
		int64_t number_of_hands;
		int64_t share_of_hands;
		int64_t number_of_threads;
		bool verbose = true;

	public:
		void print();
		void serialize(char *buffer, int buffer_size);

	private:
		void getCurrentTime();
		void generateName();
};

#endif // PARAMETERS_HPP
