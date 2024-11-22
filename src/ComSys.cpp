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


using namespace std;

	void* ComSys::startComSys(void* arg) {
		const char *shm_name = "/shm_aircraft_data";
				    const int SIZE = sizeof(SharedMemory);

				    // Open shared memory
				    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
				    SharedMemory *shared = static_cast<SharedMemory*>(mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));

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

	sem_wait(&shared->aircraft_mutex[i]);
	//checks for any current violations
	for (int i = 0; i < TotalNumAircraft; i++) {
		for (int j = 1; j < TotalNumAircraft; j++) {
			temp_X = shared->aircraft[i].getPositionX() - shared->aircraft[j].getPositionX();
			temp_Y = shared->aircraft[i].getPositionY() - shared->aircraft[j].getPositionY();
			temp_Z = shared->aircraft[i].getPositionZ() - shared->aircraft[j].getPositionZ();

			if (temp_X <= 3000 || temp_Y <= 3000 || temp_Z <= 1000) {
				operatorAlert(i, j);
			}

			else {
				break;
			}
		}
	}
	sem_post(&shared->aircraft_mutex[i]);

	sem_wait(&shared->aircraft_mutex[i]);
	// checks for future violations
	for (int i = 0; i < TotalNumAircraft; i++) {
		for (int j = 1; j < TotalNumAircraft; j++) {
			temp_X = shared->aircraft[i].getPositionX() + n * shared->aircraft[i].getSpeedX()
					- shared->aircraft[j].getPositionX() + n * shared->aircraft[j].getSpeedX();
			temp_Y = shared->aircraft[i].getPositionY() + n * shared->aircraft[i].getSpeedY()
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
	sem_post(&shared->aircraft_mutex[i]);
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

