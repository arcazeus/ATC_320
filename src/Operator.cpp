#include "Operator.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>

/////Constructors/////

// OperatorConsole constructor
Operator::Operator() {

	std::ofstream logFile("logFile");
	// Open a log file to store commands (append mode)

	if (logFile.is_open()) {
		std::cout << "Operator Console initialized. Log file created." << std::endl;
		logFile << "Operator Console started.\n";  // Log initialization event
	} else {
		std::cerr << "Error: Unable to open log file." << std::endl;
	}


}

// Destructor
Operator::~Operator() {
	std::ofstream logFile("logFile");

	if (logFile.is_open()) {
		logFile << "Operator Console shutting down.\n";  // Log shutdown event
		logFile.close();  // Close the log file
	}
}

/////Aircraft related commands/////
void Operator::sendCommandToAircraft(int aircraftID, const std::string& command) {
    std::cout << "Sending command to aircraft " << aircraftID << ": " << command << std::endl;
    logCommand(command);
    // communicate with the communication system to send the command
}

// Method to request information about an aircraft
void Operator::requestAircraftInfo(int aircraftID) {
    std::cout << "Requesting information for aircraft " << aircraftID << std::endl;
    // Interface with radar or computer system to retrieve the requested info
}

// Method to display information (could be on the radar display or console)
void Operator::displayInfo(const std::string& info) {
    std::cout << "Displaying information: " << info << std::endl;
    // Logic to display the info on the console
}

// Private method to log the commands sent
void Operator::logCommand(const std::string& command) {
    std::cout << "Logging command: " << command << std::endl;
    // Add logic to store the command in a log file
}


