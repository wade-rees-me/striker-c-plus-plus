#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include "arguments.hpp"
#include "constants.hpp"
#include <cstdint>
#include <ctime>
#include <string>

// Parameters class to store simulation parameters
class Parameters {
  public:
    Parameters(Arguments *arguments);

  public:
    char name[MAX_STRING_SIZE];
    char processor[MAX_STRING_SIZE];
    char playbook[MAX_STRING_SIZE];
    char strategy[MAX_STRING_SIZE];
    char decks[MAX_STRING_SIZE];
    char epoch[MAX_STRING_SIZE];
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
