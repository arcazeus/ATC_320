#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include <errno.h>
#include <semaphore.h>
#include "ComSys.h"
#include "Radar.h"
#include "Aircraft.h"
#include "Comms.h"
#include "Operator.h"
#include "Display.h"
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include "globals.h"
#include "cTimer.h"

Comms::Comms() {

}

void* Comms::startComms(void *arg) {

	((Comms*) arg)->runComms();
	return NULL;
}

void Comms::runComms() {
	{
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout << "Communication System started..." << std::endl;
	}
	// Register with the name service
	name_attach_t *attach = name_attach(NULL, "commsServer", 0);
	if (attach == NULL) {
		std::cerr << "Error: Failed to register Comms with name service!"
				<< std::endl;
		return;
	}

	std::cout << "Comms " << 1 << " registered as commsServer" << std::endl;

	cTimer time = cTimer(1, 0);
	time.startTimer();

	while (true) {
		time.tick();
		checkForMessage(attach);
		time.waitTimer();
		time.tock();
	}

}

void Comms::checkForMessage(name_attach_t *attach) {

	char msg[256];
	int rcvid;
	{
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout<<"Communication System waiting for message"<<std::endl;
	}
	// Non-blocking receive
	rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
	if (rcvid != -1) {

		handleMessage(rcvid, msg);
	}

}

//void Comms::handleMessage(int rcvid, const char *msg) {
//	std::string receivedMessage(msg);
//
//
//	if (receivedMessage == "OperatorCommand") {
//		std::stringstream response;
//		response << this->aircraftID << this->command;
//		sendCommandToAircraft(aircraftID, command);
//
//		MsgReply(rcvid, 0, &aircraftID, sizeof(aircraftID) + sizeof(command));
//	}
//}


void Comms::handleMessage(int rcvid, const char *msg) {
    std::string receivedMessage(msg);

    size_t colonPos = receivedMessage.find(":");
    if (colonPos == std::string::npos) {
        MsgReply(rcvid, 0, "Invalid Message Format", strlen("Invalid Message Format") + 1);
        return;
    }

    int aircraftID = std::stoi(receivedMessage.substr(0, colonPos));
    std::string command = receivedMessage.substr(colonPos + 1);

    sendCommandToAircraft(aircraftID, command);

    MsgReply(rcvid, 0, "Command Relayed", strlen("Command Relayed") + 1);
}

void Comms::sendCommandToAircraft(int aircraftID, const std::string &message) {
    std::string aircraftName = "Aircraft_" + std::to_string(aircraftID);

    int coid = name_open(aircraftName.c_str(), 0);
    if (coid == -1) {
        std::cerr << "Error: Failed to connect to Aircraft " << aircraftID << std::endl;
        return;
    }

    if (MsgSend(coid, message.c_str(), message.size() + 1, NULL, 0) == -1) {
        std::cerr << "Error: Failed to send message to Aircraft " << aircraftID << std::endl;
    } else {
        std::cout << "Message sent to Aircraft " << aircraftID << ": " << message << std::endl;
    }

    name_close(coid);
}

//void Comms::sendCommand() {
/*	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "sending command to aircraft" << std::endl;
	}
	// Build the name for the aircraft
	std::string aircraftName = "Aircraft_" + std::to_string(aircraftID);


	// Open a connection to the aircraft
	int coid = name_open(aircraftName.c_str(), 0);
	if (coid == -1) {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr << "Failed to connect to Aircraft " << aircraftID << ": "
					<< strerror(errno) << std::endl;
		}

		return;
	}

	// Prefix the command with "Command:"
	std::string fullCommand = "Command:" + command;

	// Send the command
	if (MsgSend(coid, fullCommand.c_str(), fullCommand.size() + 1, NULL, 0)
			== -1) {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr << "Failed to send command to Aircraft " << aircraftID
					<< ": " << strerror(errno) << std::endl;
		}
	} else {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cout << "Command sent to Aircraft " << aircraftID << ": "
					<< command << std::endl;
		}
	}

	// Close the connection
	name_close(coid);

*/
//}

