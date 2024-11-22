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

Comms::Comms() {

}

void* Comms::startComms(void* arg) {
	const char *shm_name = "/shm_aircraft_data";
			    const int SIZE = sizeof(SharedMemory);

			    // Open shared memory
			    int shm_fd = shm_open(shm_name, O_RDWR, 0666);
			    SharedMemory *shared = static_cast<SharedMemory*>(mmap(0, SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));

	((Comms*) arg)->runComms();
		return NULL;
}

void Comms::runComms(){
	std::cout << "Communication System started...";
	while(1){
		cout<<"help"<<endl;
	}
}
void Comms::sendCommand(const string &aircraftID, const Comms &command)

{

}

