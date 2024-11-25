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
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <cmath>
#include <errno.h>
#include "globals.h"
#include "ComSys.h"
#include "cTimer.h"

using namespace std;

ComSys::ComSys() :
		TotalNumAircraft(0), n(0) {
	// Constructor implementation


}

ComSys::~ComSys() {

}

void* ComSys::startComSys(void *arg) {
	((ComSys*) arg)->runComSys();
	return NULL;

}

void ComSys::runComSys() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "ComSys running..." << std::endl;
	}

	// Register with the name service here
	name_attach_t *attach = name_attach(NULL, "computerSystemServer", 0);
	if (attach == NULL) {
		std::cerr
				<< "Error: Failed to register Computer System with name service in RUN!"
				<< std::endl;
		// Handle error appropriately (e.g., throw exception or set a flag)
	} else {
		std::cout
				<< "Computer System registered with name service in constructor."
				<< std::endl;
	}
	std::thread listenerThread(&ComSys::listenForMessages, this, attach);
	listenerThread.detach(); // Allow it to run independently

	cTimer time = cTimer(1, 0);
	time.startTimer();
	while (true) {
		time.tick();

		setAircraftList();
		// Check for violations
		checkViolations();
		// Send data to Display
		sendDataDisplayAllAircraft();
		checkForMessages(attach);
		time.waitTimer();
		time.tock();
	}
}
void ComSys::checkForMessages(name_attach_t *attach) {

	char msg[256];
	{
	std::lock_guard<std::mutex> lock(coutMutex);
	std::cout<<"ComSys checking for messages"<<std::endl;
	}
	int rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);

	if (rcvid != -1) {
		handleMessage(rcvid, msg);

	}
}

void ComSys::listenForMessages(name_attach_t *attach) {
	while (true) {
		char msg[256]; // Buffer for received messages

		int rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);

		if (rcvid != -1) {
			std::cout<<"ComSys is listening"<<std::endl;
			handleMessage(rcvid, msg);
		} else if (errno != EAGAIN) { // Ignore EAGAIN; handle other errors
			std::cerr << "Message reception failed: " << strerror(errno)
					<< std::endl;
		}
	}
}

void ComSys::handleMessage(int rcvid, const char *msg) {
	char reply[256];
	std::string receivedMessage(msg);

	if (receivedMessage.find("RequestInfo") == 0) {
		// Extract aircraft ID
		int aircraftId = std::stoi(receivedMessage.substr(11));
		if (aircraftId >= 0 && aircraftId < TotalNumAircraft) {
			std::stringstream info;
			info << "Aircraft " << aircraft[aircraftId].getId()
					<< " Position: X=" << aircraft[aircraftId].getPositionX()
					<< ", Y=" << aircraft[aircraftId].getPositionY() << ", Z="
					<< aircraft[aircraftId].getPositionZ();
			strcpy(reply, info.str().c_str());
		} else {
			strcpy(reply, "Error: Invalid Aircraft ID");
		}
	} else if (receivedMessage.find("ChangeN") == 0) {
		// Change the parameter n

		int newN = std::stoi(receivedMessage.substr(7)); // Extract the new N value
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			n = newN; // Update the parameter
			std::cout << "Parameter N updated to: " << n << std::endl;
		}
		MsgReply(rcvid, 0, "Parameter updated successfully", 27);
	} else if (receivedMessage == "DisplayRequest") {
		// Handle request from Display for all aircraft data
		std::stringstream data;
		for (const auto &plane : aircraft) {
			data << plane.getId() << " " << plane.getPositionX() << " "
					<< plane.getPositionY() << " " << plane.getPositionZ()
					<< "\n";
		}

		// Send the full aircraft data back
		std::string response = data.str();
		MsgReply(rcvid, 0, response.c_str(), response.size() + 1);

	} else {
		MsgReply(rcvid, 0, "Unknown command", 15);

	}

// Send a reply back to the sender
	MsgReply(rcvid, 0, reply, strlen(reply) + 1);
}

void ComSys::setAircraftList() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Receiving Aircraft" << std::endl;
	}
	std::string RadarName = "Radar_1";
	int coid = name_open("Radar_1", 0);
	if (coid == -1) {
		std::cerr << "Failed to connect to Radar " << 1 << ": "
				<< strerror(errno) << std::endl;
		return;
	}
	const char *request = "ComSysRequest";

	std::vector<Aircraft> tempPlanes(MAX_AIRCRAFTS); // Placeholder for incoming Aircraft data

	if (MsgSend(coid, request, strlen(request) + 1, tempPlanes.data(),
			tempPlanes.size() * sizeof(Aircraft)) == -1) {
		std::cerr << "Failed to receive data from Radar " << 1 << ": "
				<< strerror(errno) << std::endl;
		name_close(coid);
		return;
	} else {
		std::lock_guard<std::mutex> lock(coutMutex);
		// Update the aircraft vector and TotalNumAircraft
		aircraft.clear();
		for (const auto &plane : tempPlanes) {
			if (plane.getId() == 0 && plane.getPositionX() == 0) {
				// Stop processing when an invalid entry is encountered
				break;
			}
			aircraft.push_back(plane);
		}

		// Update the total number of aircraft
		TotalNumAircraft = aircraft.size();

		std::cout << "ComSys received " << TotalNumAircraft
				<< " aircraft from Radar." << std::endl;
	}

	name_close(coid);

}

//minimum Z=1000, X=3000, Y=3000
void ComSys::checkViolations() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Checking for Violations" << std::endl;
	}
	for (int i = 0; i < TotalNumAircraft - 1; i++) {
		for (int j = i + 1; j < TotalNumAircraft; j++) {
			// Current positions
			double deltaX = aircraft[i].getPositionX()
					- aircraft[j].getPositionX();
			double deltaY = aircraft[i].getPositionY()
					- aircraft[j].getPositionY();
			double deltaZ = aircraft[i].getPositionZ()
					- aircraft[j].getPositionZ();

			// Future positions after n seconds
			double futureDeltaX =
					(aircraft[i].getPositionX() + aircraft[i].getSpeedX() * n)
							- (aircraft[j].getPositionX()
									+ aircraft[j].getSpeedX() * n);
			double futureDeltaY =
					(aircraft[i].getPositionY() + aircraft[i].getSpeedY() * n)
							- (aircraft[j].getPositionY()
									+ aircraft[j].getSpeedY() * n);
			double futureDeltaZ =
					(aircraft[i].getPositionZ() + aircraft[i].getSpeedZ() * n)
							- (aircraft[j].getPositionZ()
									+ aircraft[j].getSpeedZ() * n);

			// Check current violation
			if (std::abs(deltaX) <= 3000 || std::abs(deltaY) <= 3000
					|| std::abs(deltaZ) <= 1000) {
				operatorAlert(aircraft[i].getId(), aircraft[j].getId());
			}

			// Check future violation
			if (std::abs(futureDeltaX) <= 3000 || std::abs(futureDeltaY) <= 3000
					|| std::abs(futureDeltaZ) <= 1000) {
				operatorAlert(aircraft[i].getId(), aircraft[j].getId());
			}
		}
	}
}

void ComSys::operatorAlert(int id_1, int id_2) {

	std::string alertMessage = "Violation detected between aircraft "
			+ std::to_string(id_1) + " and aircraft " + std::to_string(id_2);

// Connect to the Operator
	int coid = name_open("DisplayServer", 0);
	if (coid == -1) {
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr << "Failed to connect to Display: " << strerror(errno)
					<< std::endl;
		}

		// Send the alert message
		if (MsgSend(coid, alertMessage.c_str(), alertMessage.size() + 1, NULL,
				0) == -1) {
			{
				std::lock_guard<std::mutex> lock(coutMutex);
				std::cerr << "Failed to send alert to Display: "
						<< strerror(errno) << std::endl;
			}

			// Close the connection
			name_close(coid);
		}
	}
}

void ComSys::sendDataDisplayAllAircraft() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "sending data to Display" << std::endl;
	}
// Prepare the data
	std::stringstream data;
	for (const auto &plane : aircraft) {
		data << plane.getId() << " " << plane.getPositionX() << " "
				<< plane.getPositionY() << " " << plane.getPositionZ() << "\n";
	}

// Connect to the Display
	int coid = name_open("DisplayServer", 0);
	if (coid == -1) {
		std::cerr << "Failed to connect to Display: " << strerror(errno)
				<< std::endl;
		return;
	}

// Send the data
	std::string allData = data.str();
	if (MsgSend(coid, allData.c_str(), allData.size() + 1, NULL, 0) == -1) {
		std::cerr << "Failed to send data to Display: " << strerror(errno)
				<< std::endl;
	}
// Close the connection
	name_close(coid);

}

