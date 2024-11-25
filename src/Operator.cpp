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
#include "globals.h"
#include "Logger.h"
/////Constructors/////

// OperatorConsole constructor
Operator::Operator(): connectionId(-1) {

	logFile.open("logFile", std::ios_base::app);
	while ((connectionId = name_open("computerSystemServer", 0)) == -1) {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr
					<< "Failed to connect to the Computer System. Retrying in 1 second..."
					<< std::endl;
		}
		sleep(1); // Wait before retrying
	}
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout
				<< "Successfully connected to Computer System via name lookup."
				<< std::endl;
	}

	// Open a log file to store commands (append mode)

	if (logFile.is_open()) {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cout << "Operator Console initialized. Log file created."
					<< std::endl;
			logFile << "Operator Console started.\n";  // Log initialization event
		}
	} else {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr << "Error: Unable to open log file." << std::endl;
		}
	}

}

// Destructor
Operator::~Operator() {

//	std::ofstream logFile("logFile");
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
void Operator::sendCommandToAircraft(int aircraftID,
		const std::string &command) {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "sending command" << std::endl;
	}
	// Format the command to include aircraft ID and the actual command
	std::string formattedCommand = "Aircraft " + std::to_string(aircraftID)
			+ " - Command: " + command;

	std::cout << "Sending command to aircraft " << aircraftID << ": " << command
			<< std::endl;

	// Log the command
	logCommand(formattedCommand);

	// Example communication with the Computer System (acting as a communication hub)
	if (connectionId == -1) {
		std::cerr << "Error: No valid connection to the Computer System!"
				<< std::endl;
		return;
	}

	// Send the command to the Computer System, which will forward it to the Aircraft
	char response[256];
	int status = MsgSend(connectionId, formattedCommand.c_str(),
			formattedCommand.size() + 1, response, sizeof(response));

	if (status == -1) {
		std::cerr << "Failed to send command to the Computer System!"
				<< std::endl;
	} else {
		std::cout << "Received response from Computer System: " << response
				<< std::endl;
	}

}

void Operator::changeParameterN(int newN) {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Changing Parameter N to:" << newN << std::endl;
	}
	if (connectionId == -1) {
		std::cerr << "Failed to connect to Computer System: " << strerror(errno)
				<< std::endl;
		return;
	}
		std::string command = "ChangeN" + std::to_string(newN);
		char response[256];
		if (MsgSend(connectionId, command.c_str(), command.size() + 1, response, sizeof(response)) == -1) {
			std::cerr << "Failed to send command to Computer System: "
					<< strerror(errno) << std::endl;
		} else{
			std::cout << "Computer System response:" << response << std::endl;
		}
}

// Method to request information about an aircraft
void Operator::requestAircraftInfo(int aircraftID) {
	std::cout << "Requesting information for aircraft " << aircraftID
			<< std::endl;

	// Check if there is a valid connection to the Computer System
	if (connectionId == -1) {
		std::cerr << "Error: No valid connection to the Computer System!"
				<< std::endl;
		return;
	}

	// Format the request to get information about the specific aircraft
	std::string request = "RequestInfo " + std::to_string(aircraftID);

	// Buffer for the response from the Computer System
	char response[256];

	// Send the request to the Computer System
	int status = MsgSend(connectionId, request.c_str(), request.size() + 1,
			response, sizeof(response));

	if (status == -1) {
		std::cerr << "Failed to send request for aircraft info!" << std::endl;
	} else {
		// Call displayInfo to show the received information
		displayInfo(response);
	}
}

// Method to display information (could be on the radar display or console)
void Operator::displayInfo(const std::string &info) {
	std::cout << "Displaying information: " << info << std::endl;
	// Logic to display the info on the console
}

// Private method to log the commands sent
void Operator::logCommand(const std::string &command) {
	if (logFile.is_open()) {
		logFile << "Command: " << command << std::endl;
		std::cout << "Logging command: " << command << std::endl;
	} else {
		std::cerr << "Error: Log file is not open!" << std::endl;
	}

//	OperatorLog.log_OperatorCommand(commands, commands);
}


void* Operator::startOperator(void *arg) {

	((Operator*) arg)->runOperator();
	return NULL;

}

void Operator::runOperator() {

	 while (true) {
		std::cout << "Enter a new value for Parameter N (-1 to skip): ";
		int newN;
		std::cin >> newN;

		if (newN >= 0) {
			changeParameterN(newN); // Send the new value to ComSys
		} else {
			std::cout << "Skipping parameter change." << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait before prompting again
	}

}
