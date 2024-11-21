#include "Operator.h"
#include "Aircraft.h"
#include "ComSys.h"
#include "Display.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ostream>
#include <iostream>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <unistd.h>

/////Constructors/////

// OperatorConsole constructor
Operator::Operator() {
	std::ofstream logFile("logFile");
	 // Establish a connection to the Computer System's communication channel
	connectionId = name_open("computerSystemServer", 0);
	if (connectionId == -1) {
		std::cerr << "Failed to connect to the Computer System!" << std::endl;
	} else {
        std::cout << "Successfully connected to Computer System via name lookup." << std::endl;
    }

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
	if (connectionId != -1) {
	        name_close(connectionId);
	    }
	if (logFile.is_open()) {
		logFile << "Operator Console shutting down.\n";  // Log shutdown event
		logFile.close();  // Close the log file
	}
}

/////Setters & Getters////
// Setter for connectionId
void Operator::setConnectionId(int connId) {
    this->connectionId = connId;
}

// Getter for connectionId
int Operator::getConnectionId() const {
    return connectionId;
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
	std::ofstream logFile("logFile");
	if (logFile.is_open()) {
		logFile << "Command: " << command << std::endl;
		std::cout << "Logging command: " << command << std::endl;
	} else {
		std::cerr << "Error: Log file is not open!" << std::endl;
	}
}

void Operator::checkViolationFromCS(){
	// Buffer to store the message received from the Computer System
	char message[256];
	int csId = getConnectionId();  // Get the connection ID for the Computer System

	// Receive the message from the Computer System
	int status = MsgReceive(csId, message, sizeof(message), NULL);

	if (status == -1) {
		std::cerr << "Error: Failed to receive message from the Computer System!" << std::endl;
		return;
	}

	// Process the received message
	std::string receivedMessage(message);
	if (receivedMessage.find("Violation") != std::string::npos) {
		std::cout << "Violation detected: " << receivedMessage << std::endl;
		// Trigger alarm or handle the violation appropriately
		// For example, display an alert to the operator:
		displayInfo("ALARM: Aircraft Separation Violation!");
	} else {
		// No violation found, display regular info (optional)
		std::cout << "Received from CS: " << receivedMessage << std::endl;
	}
}

void* Operator::startOperator(void* arg){
//	Operator* instance = (Operator*)arg;
//	instance->runOperator();
//	std::cout << "Operator Console starting..." << std::endl;
//
//	// Ensure the connection to the Computer System is established
//	if (instance->connectionId == -1) {
//		std::cerr << "Error: Unable to start Operator Console, no connection to Computer System!" << std::endl;
//	} else {
//		std::cout << "Computer System Connected." << std::endl;
//	}
//
//	std::cout << "Operator Console started" << std::endl;
	return NULL;
}

void Operator::runOperator(){

	// Main operator loop: check for aircraft violations and handle other tasks
		while (true) {
			// Check for violations from the Computer System
			this->checkViolationFromCS();

			// Add more functionality here if needed (e.g., process incoming commands)

			// Sleep for a short time before the next loop iteration (optional)
			usleep(1000000);  // 1 second
		}

}
