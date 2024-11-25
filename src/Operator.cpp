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
#include "cTimer.h"
/////Constructors/////

// OperatorConsole constructor
Operator::Operator() {

}

// Destructor
Operator::~Operator() {

}

void Operator::checkForMessages(name_attach_t *attach) {
	char msg[256];
	int rcvid;
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Operator waiting for messages" << std::endl;
	}
	// Non-blocking receive
	rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
	if (rcvid != -1) {
		handleMessage(rcvid, msg);
	}
}

void Operator::handleMessage(int rcvid, const char *msg) {
	std::string receivedMessage(msg);

	if (receivedMessage == "Parameter") {
		// Respond to ComSys
		int newN;
		std::cout<<"WRITE NEW PARAMETER: "<<std::endl;
		std::cin >> newN;

		MsgReply(rcvid, 0, &newN, sizeof(newN));
	} else {
		MsgReply(rcvid, 0, "Unknown Message", strlen("Unknown Message") + 1);
	}
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

}

void Operator::changeParameterN(int newN) {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Changing Parameter N to:" << newN << std::endl;
	}

}

// Private method to log the commands sent
void Operator::logCommand(const std::string &command) {
	if (logFile.is_open()) {
		logFile << "Command: " << command << std::endl;
		std::cout << "Logging command: " << command << std::endl;
	} else {
		std::cerr << "Error: Log file is not open!" << std::endl;
	}

	//OperatorLog.log_OperatorCommand(commands, commands);
}

void* Operator::startOperator(void *arg) {

	((Operator*) arg)->runOperator();
	return NULL;

}

void Operator::runOperator() {
	cTimer time = cTimer(1, 0);
	time.startTimer();

	std::string OperatorName = "Operator_1";

	name_attach_t *attach = name_attach(NULL, "Operator_1", 0);
	if (attach == NULL) {
		std::cerr << "Error: Failed to register Operator " <<" with name service!" << std::endl;
		return;
	}
	int parameterTimer=0;

	while (true) {
		time.tick();
if(parameterTimer%30==0){
		checkForMessages(attach);
}
		time.waitTimer();
		time.tock();
parameterTimer++;
	}
}
