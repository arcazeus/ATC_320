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
#include "ComSys.h"
#include "Radar.h"
#include "Aircraft.h"
#include "Comms.h"
#include "Operator.h"
#include "Display.h"
#include <chrono>
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include "cTimer.h"
#include "globals.h"
#include <tuple>

// Constructor
Display::Display() {

}

// Destructor
Display::~Display() {
	std::cout << "Display shutting down." << std::endl;
}

void* Display::startDisplay(void *arg) {

	((Display*) arg)->runDisplay();
	return NULL;

}

void Display::runDisplay() {

	/*name_attach_t *attach = name_attach(NULL, "DisplayServer", 0);
	 if (attach == NULL) {
	 std::cerr << "Error: Failed to register Display with name service!"
	 << std::endl;
	 return;
	 }*/
	cTimer time = cTimer(5, 0);

	time.startTimer();
	while (true) {
		/*	char msg[256];
		 int rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
		 if (rcvid != -1) {
		 // Parse and display the data
		 std::string data(msg);
		 std::cout << "Display Update: " << data << std::endl;

		 // Send acknowledgment
		 MsgReply(rcvid, 0, NULL, 0);
		 }*/
		time.tick();

		updateDisplay();

		time.waitTimer();
		time.tock();
	}

	//name_detach(attach, 0);

}

void Display::checkForMessages(name_attach_t *attach) {

	/*char msg[256];
	 int rcvid;
	 {
	 std::lock_guard<std::mutex> lock(coutMutex);
	 std::cout<<"Display checking for messages"<<std::endl;
	 }
	 // Non-blocking receive
	 rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
	 if (rcvid != -1) {
	 handleMessages(rcvid, msg);
	 }*/

}

void Display::handleMessages(int rcvid, const char *msg) {
	/*
	 std::string receivedMessage(msg);
	 std::string message = "there is a potential collision";
	 if (receivedMessage == "ComSysRequest") {
	 // Respond to Radar
	 std::cout << "there is a potential collision" << std::endl;

	 MsgReply(rcvid, 0, &message, sizeof(message));
	 }*/

}

void Display::updateDisplay() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Display Updating" << std::endl;
	}

	int coid = name_open("computerSystemServer", 0);

	if (coid == -1) {
		std::cerr << "Failed to connect to ComSys" << 1 << ": "
				<< strerror(errno) << std::endl;
		return;
	}
	const char *request = "DisplayRequest";
	std::vector<Aircraft> PLANES; // Placeholder for incoming Aircraft data

	char receivedData[4096]; // Adjust size based on expected payload
	memset(receivedData, 0, sizeof(receivedData));

	if (MsgSend(coid, request, strlen(request) + 1, receivedData,
			sizeof(receivedData)) == -1) {
		std::cerr << "Failed to receive data from ComSys: " << strerror(errno)
				<< std::endl;
	} else {
		std::lock_guard<std::mutex> lock(coutMutex);

		// Log the received raw data
		std::cout << "Raw data received from ComSys: " << receivedData
				<< std::endl;

		// Parse the received data
		std::istringstream iss(receivedData);
		std::string line;
		std::vector<std::tuple<int, int, int>> aircraftPositions;
		while (std::getline(iss, line)) {
			std::cout << "Parsed line: " << line << std::endl;
			int id, x, z;

			std::istringstream linestream(line);
			linestream >> id >> x >> z;
			aircraftPositions.emplace_back(id, x, z);

		}

		std::vector<std::vector<char>> grid(scaledZ,
				std::vector<char>(scaledX, '.'));

		for (const auto &pos : aircraftPositions) {

			int x = std::get<1>(pos)/1000;
			int z = std::get<2>(pos)/1000;

			if (z >= 0 && z < scaledZ && x >= 0 && x < scaledX) {
				grid[z][x] = 'P'; // 'P' for Plane
			}
		}

			 for (int i = scaledZ - 1; i >= 0; --i) { // Top-down display
			            std::cout << "|";
			            for (int j = 0; j < scaledX; ++j) {
			                std::cout << grid[i][j];
			            }
			            std::cout << "|" << std::endl;
			        }

	}

	name_close(coid);

}

