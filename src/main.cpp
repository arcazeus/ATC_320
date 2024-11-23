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

#define MAX_AIRCRAFTS 120

int main() {

	char buf[256];
	if (getcwd(buf, sizeof(buf)) != NULL) {
	    std::cout << "Current working directory: " << buf << std::endl;
	}
	// thread IDs
	pthread_t ComSys_tid,Operator_tid, Radar_tid, Display_tid, Comms_tid, Aircraft_tid[MAX_AIRCRAFTS];

	ComSys ComSysObj;
	Radar RadarObj;
	Display DisplayObj;
	Comms CommsObj;
	Operator OperatorObj;
	std::vector<Aircraft> aircrafts;

	/*std::ifstream file(".txt");

			if (!file.is_open()) {
				std::cerr << "Error opening file: " << "ENROUTE" << std::endl;
			}

			std::string line;
			while (std::getline(file, line)) {
				std::istringstream iss(line);
				float time;
				int ID;
				double x, y, z, xSpeed, ySpeed, zSpeed;

				if (!(iss >> time >> ID >> x >> y >> z >> xSpeed >> ySpeed >> zSpeed)) {
					std::cerr << "Error reading line: " << line << std::endl;
					continue; // Skip lines that do not match the expected format
				} else {
					//cout << "Creating aircraft with ID: " << ID << endl;
				}

				Aircraft aircraft = Aircraft(time, ID, x, y, z, xSpeed, ySpeed, zSpeed);
				aircrafts.push_back(aircraft);
			}
*/
	int numberOfAircrafts = 2; // For example, create 5 aircraft
	for (int i = 0; i < numberOfAircrafts; ++i) {
		// Create an Aircraft object with some initial parameters
		// Adjust the parameters as needed
		Aircraft aircraft(i, 0.0, 0.0, 1000.0 + i * 1000.0, 500.0, 0.0, 0.0, 0.0f); // time is 0.0f
		aircrafts.push_back(aircraft);
	}

	// Set the aircraft list in the ComSys
	//ComSysObj.setAircraftList(aircrafts);

	// Set the aircraft IDs in the Radar (if needed)
	std::vector<int> aircraftIDs;
	for (const auto& aircraft : aircrafts) {
		aircraftIDs.push_back(aircraft.getId());
	}
	RadarObj.setAircraftIDs(aircraftIDs);

	// Creating Airplane threads
	int size = aircrafts.size();
	for (int i = 0; i < size; i++) {
		if (pthread_create(&Aircraft_tid[i], NULL, &Aircraft::startAircraft,&aircrafts[i]) != 0) {
			perror("Failed to create airplane thread");
			return 1;
		}
		//RadarObj.addAircraft();
	}

	// Creating Comsys thread
	if (pthread_create(&ComSys_tid, NULL, &ComSys::startComSys, &ComSysObj)
			!= 0) {
		perror("Failed to create ComSys thread");
		return 1;
	}
	// Creating Radar thread
	if (pthread_create(&Radar_tid, NULL, &Radar::startRadar, &RadarObj) != 0) {
		perror("Failed to create Radar thread");
		return 1;
	}

	// Creating Operator thread
	if (pthread_create(&Operator_tid, NULL, &Operator::startOperator,
			&OperatorObj) != 0) {
		perror("Failed to create Operator thread");
		return 1;
	}

	// Creating Display thread
	if (pthread_create(&Display_tid, NULL, &Display::startDisplay, &DisplayObj)
			!= 0) {
		perror("Failed to create Display thread");
		return 1;
	}
	// Creating Comms thread
	if (pthread_create(&Comms_tid, NULL, &Comms::startComms, &CommsObj) != 0) {
		perror("Failed to create Comms thread");
		return 1;
	}

	//joining threads
	for (int i = 0; i < size; i++) {
		if (pthread_join(Aircraft_tid[i], NULL) != 0) {
			perror("Failed to join Aircraft thread");
			return 1;
		}
	}

	if (pthread_join(ComSys_tid, NULL) != 0){
		perror("Failed to join ComSys thread");
		return 1;
	}

	if (pthread_join(Operator_tid, NULL) != 0) {
		perror("Failed to join ComSys thread");
		return 1;
	}

	if (pthread_join(Radar_tid, NULL) != 0) {
		perror("Failed to join ComSys thread");
		return 1;
	}

	if (pthread_join(Display_tid, NULL) != 0) {
		perror("Failed to join ComSys thread");
		return 1;
	}

	if (pthread_join(Comms_tid, NULL) != 0) {
		perror("Failed to join ComSys thread");
		return 1;
	}

	return 0;
}

