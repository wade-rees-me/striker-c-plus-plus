#include <string>
#include <ctime>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <sstream>
#include <iomanip> 
#include <cstdlib>
#include <uuid/uuid.h>
#include "logger.hpp"

//
// Striker/simulations/2024_09_27/striker-c-plus-plus_00000000_simulation.txt
// Striker/simulations/2024_09_27/striker-c-plus-plus_00000000_debug.txt
//
Logger::Logger(std::string simulator, bool debugFlag) : simulator(simulator) {
	directory = getSimulationDirectory();
	getSubdirectory();

	if (!std::filesystem::exists(directory + "/" + subdirectory)) {
		if (std::filesystem::create_directory(directory + "/" + subdirectory)) {
		}
	}

	simulationFileName = directory + "/" + subdirectory + "/" + simulator + "_" + guid + "_simulation.txt";
	simulationFile.open(simulationFileName, std::ios::out);

	if(debugFlag) {
		std::string debugFileName = directory + "/" + subdirectory + "/" + simulator + "_" + guid + "_debug.txt";
		debugFile.open(debugFileName, std::ios::out);
	}
}

//
Logger::~Logger() {
    if (simulationFile.is_open()) {
        simulationFile.close();
    }
    if (debugFile.is_open()) {
        debugFile.close();
    }
}

//
void Logger::simulation(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);  // Ensure thread-safe access to logFile
    if (simulationFile.is_open()) {
        simulationFile << message;
        std::cout << message << std::flush;
    }
    if (debugFile.is_open()) {
        debugFile << message;
    }
}

//
void Logger::debug(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);  // Ensure thread-safe access to logFile
    if (debugFile.is_open()) {
        debugFile << message;
    }
}

//
void Logger::insert(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);  // Ensure thread-safe access to logFile
	std::string insertFileName = directory + "/" + subdirectory + "/" + simulator + "_" + guid + "_insert.txt";
	std::ofstream insertFile;
	insertFile.open(insertFileName, std::ios::out);
    if (insertFile.is_open()) {
        insertFile << message;
    }
}

void Logger::getSubdirectory() {
    auto now = std::chrono::system_clock::now(); // Get the current time as a time_point
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now); // Convert to a time_t object for easy extraction of date and time components
    std::tm* localTime = std::localtime(&currentTime); // Convert to a tm structure to extract individual components
	std::ostringstream oss;

    oss << (localTime->tm_year + 1900) << "_" << std::setw(2) << std::setfill('0') << (localTime->tm_mon + 1) << "_" << std::setw(2) << std::setfill('0') << localTime->tm_mday;
    subdirectory = oss.str();

    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()) % 1000000;
	guid = std::to_string(micros.count());
}

