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

Radar::Radar() {

}

void* Radar::startRadar(void *arg) {

	((Radar*)arg)->runRadar();

	return NULL;

}

void Radar::runRadar() {
	std::cout << "Radar running..." << std::endl;
	while (1) {

		this->scanForAircraft();
		this->storeAirSpaceHistory();
	}
}

void Radar::addAircraft() {

}

void Radar::scanForAircraft() {
	std::cout << "Scanning...\n";
	sem_wait(&shared->aircraft_mutex[i]);
	for (int i = 0; i < 120; i++) {
		Aircrafts.pushback(shared->aircraft);
	}
	sem_post(&shared->aircraft_mutex[i]);
}

void Radar::storeAirSpaceHistory() {
	std::ofstream outfile;

	// Open the file in append mode (optional), or trunc mode (default)
	outfile.open(History);

	// Check if the file is open
	if (outfile.is_open()) {
		// Write content to the file
		//  outfile << Aircrafts << std::endl;

		// Close the file after writing
		outfile.close();
		std::cout << "Data written to file successfully!" << std::endl;
	} else {
		std::cerr << "Error: Could not open file " << History << std::endl;
	}

}

