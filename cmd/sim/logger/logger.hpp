#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include <ctime>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <mutex>
#include "constants.hpp"

//
class Logger {
public:
	Logger(std::string simulator, bool debugFlag);
    ~Logger();

private:
    std::mutex logMutex;

	std::string simulator;
	std::string directory;
	std::string subdirectory;
	std::string guid;

	std::string simulationFileName;
	std::ofstream simulationFile;
	
	std::string debugFileName;
	std::ofstream debugFile;

  public:
	void simulation(const std::string& message);
	void debug(const std::string& message);
	void insert(const std::string& message);

  private:
	void getSubdirectory();
};

#endif // LOGGER_HPP
