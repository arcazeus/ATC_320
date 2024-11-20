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
#include <pthread.h>
#include <errno.h>
#include "ComSys.h"
#include "Radar.h"
#include "Aircraft.h"
#include "Comms.h"
#include "Operator.h"
#include "Display.h"

#define MAX_AIRCRAFTS 120

int main() {
	// thread IDs
			pthread_t ComSys_tid,Operator_tid, Radar_tid, Display_tid, Comms_tid, Aircraft_tid[MAX_AIRCRAFTS];

	ComSys ComSysObj = ComSys();
	Radar RadarObj = Radar();
	Display DisplayObj = Display();
	Comms CommsObj = Comms();
	Operator OperatorObj = Operator();
	std::vector<Aircraft> aircrafts;
	std::ifstream file("ENROUTE");

	if (!file.is_open()) {
		std::cerr << "Error opening file: " << "ENROUTE" << std::endl;
		return 1;
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

	int size = aircrafts.size();
	int count = 0;

	// Creating Airplane threads
	for (int i = 0; i < size; i++) {
		if (pthread_create(&Aircraft_tid[i], NULL, &Aircraft::startAircraft,&aircrafts[i]) != 0) {
			perror("Failed to create airplane thread");
			return 1;
		}
		++count;
		RadarObj.addAircraft();
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
	for (int i = 0; i < count; i++) {
		if (pthread_join(Aircraft_tid[i], NULL) != 0) {
			perror("Failed to join Aircraft Thread");
			return 1;
		}
	}

	if (pthread_join(ComSys_tid, NULL) != 0) {
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

