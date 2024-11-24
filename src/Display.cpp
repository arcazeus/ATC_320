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
	name_attach_t *attach = name_attach(NULL, "displayServer", 0);
	if (attach == NULL) {
		std::cerr << "Error: Failed to register Display with name service!"
				<< std::endl;
		return;
	}
	cTimer time = cTimer(5, 0);
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

		time.startTimer();
		time.tick();

		updateDisplay();

		time.waitTimer();
		time.tock();
	}

	name_detach(attach, 0);

}

void Display::updateDisplay() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Display..." << std::endl;
	}
	string RadarName = "Radar_1";
	int coid = name_open("Radar_1", 0);
	if (coid == -1) {
		std::cerr << "Failed to connect to Radar " << 1 << ": "
				<< strerror(errno) << std::endl;
		return;
	}
	const char *request = "DisplayRequest";
	std::vector<Aircraft> PLANES; // Placeholder for incoming Aircraft data
	if (MsgSend(coid, request, strlen(request) + 1, &PLANES,
			PLANES.size() * sizeof(Aircraft)) == -1) {
		std::cerr << "Failed to receive data from Radar " << 1 << ": "
				<< strerror(errno) << std::endl;
	} else {
std::cout<<"DISPLAY SMT"<<std::endl;
	/*	// Add the new Aircraft to the vector
		for (long unsigned int i = 0; i < PLANES.size(); i++) {
			int gridX = PLANES[i].getPositionX() / 1000;
			int gridZ = PLANES[i].getPositionZ() / 1000;

			if (gridX >= 0 && gridX < scaledX && gridZ >= 0
					&& gridZ < scaledZ) {

				grid[gridZ][gridX] = '*';
			}
		}

		for (int i = scaledZ - 1; i >= 0; i--) {
			std::cout << "|";
			buffer += "|";
			for (int j = 0; j < scaledX; j++) {
				std::cout << grid[i][j];
				buffer += grid[i][j];
			}
			buffer += "|\n";
			std::cout << "|" << std::endl;

		}*/
	}

	name_close(coid);

}

