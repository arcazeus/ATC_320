#include "Radar.h"

#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <__mutex_base>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include "globals.h"
#include "cTimer.h"
#include "Aircraft.h"
#include <iostream>

Radar::Radar() {
	// Initialize the history file name
	History = "airspace_history.log";
}

void Radar::checkForMessages(name_attach_t *attach) {
	char msg[256];
	int rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);

	if (rcvid != -1) {
		handleMessage(rcvid, msg);

	}

}
void Radar::handleMessage(int rcvid, const char *msg) {
	std::string receivedMessage(msg);
	if (receivedMessage == "ComSysRequest") {
		//respond to ComSys
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Received ComSysRequest. Sending aircraft data..."
				<< std::endl;
		MsgReply(rcvid, 0, planes.data(), planes.size()*sizeof(Aircraft));

	} else {
		std::cout << "HOOOGA BOOGA" << std::endl;
	}
}

void* Radar::startRadar(void *arg) {
	((Radar*) arg)->runRadar();
	return NULL;
}

void Radar::runRadar() {

	std::cout << "Radar running..." << std::endl;

	std::string RadarName = "Radar_1";

	name_attach_t *attach = name_attach(NULL, "Radar_1", 0);
	if (attach == NULL) {
		std::cerr << "Error: Failed to register Radar " << 1
				<< " with name service!" << std::endl;
		return;
	}

	std::cout << "Radar " << 1 << " registered as '" << RadarName << "'."
			<< std::endl;

	cTimer time = cTimer(1, 0);
	time.startTimer();
	while (1) {

		time.tick();
		scanForAircraft();
		checkForMessages(attach);
		storeAirSpaceHistory();
		time.waitTimer();
		time.tock();

	}
}

void Radar::addAircraft(Aircraft plane) {
	planes.push_back(plane);

}

void Radar::scanForAircraft() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);

		std::cout << "Scanning..." << std::endl;
	}
	for (int aircraftID : aircraftIDs) {
		std::string aircraftName = "Aircraft_" + std::to_string(aircraftID);
		int coid = name_open(aircraftName.c_str(), 0);
		if (coid == -1) {
			std::cerr << "Failed to connect to Aircraft " << aircraftID << ": "
					<< strerror(errno) << std::endl;
			continue;
		}

		const char *request = "RadarRequest";
		Aircraft receivedAircraft = Aircraft(0, 0, 0, 0, 0, 0, 0, 0); // Placeholder for incoming Aircraft data
		if (MsgSend(coid, request, strlen(request) + 1, &receivedAircraft,
				sizeof(receivedAircraft)) == -1) {
			std::cerr << "Failed to receive data from Aircraft " << aircraftID
					<< ": " << strerror(errno) << std::endl;
		} else {
			// Add the new Aircraft to the vector
			addAircraft(receivedAircraft);
		}

		name_close(coid);
	}

}

void Radar::updateAircraftData(int aircraftID, const std::string &data) {
	// Store the data in the map
	aircraftDataMap[aircraftID] = data;
}

void Radar::setAircraftIDs(const std::vector<int> &aircraftIDs) {
	this->aircraftIDs = aircraftIDs;
}

void Radar::storeAirSpaceHistory() {
	std::ofstream outfile;
	// Open the file in append mode
	outfile.open(History, std::ios_base::app);

	// Check if the file is open
	if (outfile.is_open()) {
		// Write content to the file
		outfile << "Airspace snapshot:\n";
		for (const auto &entry : aircraftDataMap) {
			outfile << "Aircraft " << entry.first << ": " << entry.second
					<< "\n";
		}
		outfile << "---------------------------\n";

		// Close the file after writing
		outfile.close();
		{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cout << "Data written to file successfully!" << std::endl;
		}
	} else {
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cerr << "Error: Could not open file " << History << std::endl;
	}

	Airspace.log_AirSpace(History, History);
}

