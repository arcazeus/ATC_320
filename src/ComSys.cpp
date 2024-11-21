#include <iostream>
#include "ComSys.h"
#include "Aircraft.h"
#include "Operator.h"
#include <vector>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

using namespace std;

ComSys::ComSys() : TotalNumAircraft(0), n(0){

	// Register with the name service
	name_attach_t* attach = name_attach(NULL, "computerSystemServer", 0);
	if (attach == NULL) {
		std::cerr << "Error: Failed to register Computer System with name service!" << std::endl;
		return;
	}

	std::cout << "Computer System registered with name service as 'computerSystemServer'." << std::endl;

//	// Main loop to process incoming messages
//		while (true) {
//			char msg[256];        // Buffer for received messages
//			char reply[256];      // Buffer for reply messages
//			int rcvid;            // Receive ID
//
//			// Wait for a message from the Operator
//			rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
//			if (rcvid == -1) {
//				std::cerr << "Error: Failed to receive message!" << std::endl;
//				continue;
//			}
//
//			// Process the received message
//			std::string receivedMessage(msg);
//			std::cout << "Received message: " << receivedMessage << std::endl;
//
//			if (receivedMessage.find("RequestInfo") == 0) {
//				// Handle a request for aircraft information
//				int aircraftId = std::stoi(receivedMessage.substr(11));
//				if (aircraftId >= 0 && aircraftId < TotalNumAircraft) {
//					std::stringstream info;
//					info << "Aircraft " << aircraft[aircraftId].getId()
//						 << " Position: X=" << aircraft[aircraftId].getPositionX()
//						 << ", Y=" << aircraft[aircraftId].getPositionY()
//						 << ", Z=" << aircraft[aircraftId].getPositionZ();
//					strcpy(reply, info.str().c_str());
//				} else {
//					strcpy(reply, "Error: Invalid Aircraft ID");
//				}
//			} else if (receivedMessage.find("ViolationCheck") == 0) {
//				// Check for violations and respond
//				checkViolations();
//				strcpy(reply, "Violation check completed.");
//			} else {
//				// Default response for unknown commands
//				strcpy(reply, "Error: Unknown command");
//			}
//
//			// Send a reply back to the sender
//			MsgReply(rcvid, 0, reply, strlen(reply) + 1);
//		}
//
//		// Detach the name service before exiting
//		name_detach(attach, 0);
}

void* ComSys::startComSys(void* arg) {
	((ComSys*) arg)->runComSys();
	return NULL;

}

void ComSys::runComSys(){
	std::cout << "Computer System Booting up..." << std::endl;

	this->sendDataDisplay(1);


}
void ComSys::checkOperatorAlert() {

}
//minimum Z=1000, X=3000, Y=3000
void ComSys::checkViolations() {
	int temp_X;
	int temp_Y;
	int temp_Z;

	//checks for any current violations
	for (int i = 0; i < TotalNumAircraft; i++) {
		for (int j = 1; j < TotalNumAircraft; j++) {
			temp_X = aircraft[i].getPositionX() - aircraft[j].getPositionX();
			temp_Y = aircraft[i].getPositionY() - aircraft[j].getPositionY();
			temp_Z = aircraft[i].getPositionZ() - aircraft[j].getPositionZ();

			if (temp_X <= 3000 || temp_Y <= 3000 || temp_Z <= 1000) {
				operatorAlert(i, j);
			}

			else {
				break;
			}
		}
	}

	// checks for future violations
	for (int i = 0; i < TotalNumAircraft; i++) {
		for (int j = 1; j < TotalNumAircraft; j++) {
			temp_X = aircraft[i].getPositionX() + n * aircraft[i].getSpeedX()
					- aircraft[j].getPositionX() + n * aircraft[j].getSpeedX();
			temp_Y = aircraft[i].getPositionY() + n * aircraft[i].getSpeedY()
					- aircraft[j].getPositionY() + n * aircraft[j].getSpeedY();
			temp_Z = aircraft[i].getPositionY() + n * aircraft[i].getSpeedY()
					- aircraft[j].getPositionY() + n * aircraft[j].getSpeedY();

			if (temp_X <= 3000 || temp_Y <= 3000 || temp_Z <= 1000) {
				operatorAlert(i, j);
			}

			else {
				break;
			}
		}
	}

}

void ComSys::operatorAlert(int id_1, int id_2) {

	std::cout << "There is a current\future violation with plane" << id_1
			<< "and plane" << id_2 << std::endl;

}

void ComSys::sendDataDisplay(int id) {

	std::cout << aircraft[id].getId() << "" << aircraft[id].getPositionX() << ""
			<< aircraft[id].getPositionY() << "" << aircraft[id].getPositionZ()
			<< std::endl;
}

