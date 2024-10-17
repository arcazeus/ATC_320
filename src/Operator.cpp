#include "Operator.h"
<<<<<<< HEAD
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>
=======
>>>>>>> origin/peter
#include <iostream>
#include <sys/neutrino.h>
#include <string.h>
#include <unistd.h>

/////Constructors/////

// OperatorConsole constructor
Operator::Operator() {
<<<<<<< HEAD

	std::ofstream logFile("logFile");
=======
	 // Establish a connection to the Computer System's communication channel
	connectionId = name_open("computerSystemServer", 0);
	if (connectionId == -1) {
		std::cerr << "Failed to connect to the Computer System!" << std::endl;
	} else {
        std::cout << "Successfully connected to Computer System via name lookup." << std::endl;
    }

>>>>>>> origin/peter
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
<<<<<<< HEAD
	std::ofstream logFile("logFile");

=======
	if (connectionId != -1) {
	        name_close(connectionId);
	    }
>>>>>>> origin/peter
	if (logFile.is_open()) {
		logFile << "Operator Console shutting down.\n";  // Log shutdown event
		logFile.close();  // Close the log file
	}
}

/////Aircraft related commands/////
// Method to send command to aircraft
void Operator::sendCommandToAircraft(int aircraftID, const std::string& command) {
	// Format the command to include aircraft ID and the actual command
	std::string formattedCommand = "Aircraft " + std::to_string(aircraftID) + " - Command: " + command;

	std::cout << "Sending command to aircraft " << aircraftID << ": " << command << std::endl;

	// Log the command
	logCommand(formattedCommand);

	// Example communication with the Computer System (acting as a communication hub)
	if (connectionId == -1) {
		std::cerr << "Error: No valid connection to the Computer System!" << std::endl;
		return;
	}

	// Send the command to the Computer System, which will forward it to the Aircraft
	char response[256];
	int status = MsgSend(connectionId, formattedCommand.c_str(), formattedCommand.size() + 1, response, sizeof(response));

	if (status == -1) {
		std::cerr << "Failed to send command to the Computer System!" << std::endl;
	} else {
		std::cout << "Received response from Computer System: " << response << std::endl;
	}
}

// Method to request information about an aircraft
void Operator::requestAircraftInfo(int aircraftID) {
	std::cout << "Requesting information for aircraft " << aircraftID << std::endl;

	// Check if there is a valid connection to the Computer System
	if (connectionId == -1) {
		std::cerr << "Error: No valid connection to the Computer System!" << std::endl;
		return;
	}

	// Format the request to get information about the specific aircraft
	std::string request = "RequestInfo " + std::to_string(aircraftID);

	// Buffer for the response from the Computer System
	char response[256];

	// Send the request to the Computer System
	int status = MsgSend(connectionId, request.c_str(), request.size() + 1, response, sizeof(response));

	if (status == -1) {
		std::cerr << "Failed to send request for aircraft info!" << std::endl;
	} else {
		// Call displayInfo to show the received information
		displayInfo(response);
	}
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


