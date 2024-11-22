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
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include "globals.h"
#include "Logger.h"

Radar::Radar(){
    // Initialize aircraftIDs with IDs from 1 to 120
    for (int i = 1; i <= 120; ++i) {
        aircraftIDs.push_back(i);
    }
    // Initialize the history file name
    History = "airspace_history.log";
}

void* Radar::startRadar(void* arg){
    ((Radar*) arg)->runRadar();
    return NULL;
}

void Radar::runRadar(){
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Radar running..." << std::endl;

	}
    while(1){
        this->scanForAircraft();
        this->storeAirSpaceHistory();
        // Sleep to prevent tight loop
        sleep(5); // Adjust sleep time as needed
    }
}

void Radar::addAircraft() {

}

void Radar::scanForAircraft() {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Scanning..." << std::endl;
		for (int i = 0; i < 120; i++) {
				//Aircrafts.pushback(shared->aircraft);
			std::cout<<"adding plane"<<std::endl;
			}
	}


    for (int aircraftID : aircraftIDs) {
        // Build the name for the aircraft
        std::string aircraftName = "Aircraft_" + std::to_string(aircraftID);

        // Open a connection to the aircraft
        int coid = name_open(aircraftName.c_str(), 0);
        if (coid == -1) {
            // Uncomment for debugging
            // std::cerr << "Failed to connect to Aircraft " << aircraftID << ": " << strerror(errno) << std::endl;
            continue;
        }

        // Send a request for data
        const char* request = "RadarRequest";
        char reply[256];
        if (MsgSend(coid, request, strlen(request) + 1, reply, sizeof(reply)) == -1) {
            // Uncomment for debugging
            // std::cerr << "Failed to receive data from Aircraft " << aircraftID << ": " << strerror(errno) << std::endl;
        } else {
            // Parse and store the data
            std::string data(reply);
            {
            	std::lock_guard<std::mutex> lock(coutMutex);
            	std::cout << "Received from Aircraft " << aircraftID << ": " << data << std::endl;
            }
            // Store data in a suitable data structure
            updateAircraftData(aircraftID, data);
        }

        // Close the connection
        name_close(coid);
    }
}

void Radar::updateAircraftData(int aircraftID, const std::string& data) {
    // Store the data in the map
    aircraftDataMap[aircraftID] = data;
}

void Radar::setAircraftIDs(const std::vector<int>& aircraftIDs) {
    this->aircraftIDs = aircraftIDs;
}


void Radar::storeAirSpaceHistory(){
    std::ofstream outfile;
    // Open the file in append mode
    outfile.open(History, std::ios_base::app);

    // Check if the file is open
    if (outfile.is_open()) {
        // Write content to the file
        outfile << "Airspace snapshot:\n";
        for (const auto& entry : aircraftDataMap) {
            outfile << "Aircraft " << entry.first << ": " << entry.second << "\n";
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

    Airspace.log_AirSpace(History,History);
}



