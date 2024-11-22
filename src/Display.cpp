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

// Constructor
Display::Display() {
	std::cout << "Display initialized." << std::endl;
}

// Destructor
Display::~Display() {
	std::cout << "Display shutting down." << std::endl;
}



// Method to show an alarm on the display
void Display::showAlarm(const std::string &alarmMessage) {
	std::cout << "ALARM: " << alarmMessage << std::endl;
}


// Method to update the display every 5 seconds
void Display::updateDisplay() {
    while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(5));  // Sleep for 5 seconds between updates

		// Simulate updating the display
		std::cout << "Display updated with the latest information..." << std::endl;

		// You can add more logic here to update the actual display with new data
    }
}

 void* Display::startDisplay(void* arg){
	 const char *shm_name = "/shm_aircraft_data";
	 		    const int SIZE = sizeof(SharedMemory);

	 		    // Open shared memory
	 		    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
	 		    SharedMemory *shared = static_cast<SharedMemory*>(mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));

	 ((Display*) arg)->runDisplay();
	 	return NULL;
}

void Display::runDisplay(){
	std::cout << "Initializing Display" << std::endl;
	this->updateDisplay();
}


