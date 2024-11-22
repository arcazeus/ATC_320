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
<<<<<<< HEAD
#include "Operator.h"
#include "Display.h"
=======
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include "globals.h"
>>>>>>> peter

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

void Comms::runComms() {
    std::cout << "Communication System started..." << std::endl;

    // Register with the name service
    name_attach_t* attach = name_attach(NULL, "commsServer", 0);
    if (attach == NULL) {
        std::cerr << "Error: Failed to register Comms with name service!" << std::endl;
        return;
    }

    while (true) {
        char msg[256];
        int rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
        if (rcvid != -1) {
            // Process the message
            std::string receivedMessage(msg);

            // Assume the message format is "SendCommand:<AircraftID>:<Command>"
            if (receivedMessage.find("SendCommand:") == 0) {
                size_t firstColon = receivedMessage.find(":");
                size_t secondColon = receivedMessage.find(":", firstColon + 1);

                if (firstColon != std::string::npos && secondColon != std::string::npos) {
                    int aircraftID = std::stoi(receivedMessage.substr(firstColon + 1, secondColon - firstColon - 1));
                    std::string command = receivedMessage.substr(secondColon + 1);

                    // Send the command to the aircraft
                    sendCommand(aircraftID, command);

                    // Send acknowledgment
                    MsgReply(rcvid, 0, "Command Sent", strlen("Command Sent") + 1);
                } else {
                    MsgReply(rcvid, 0, "Error: Invalid Command Format", strlen("Error: Invalid Command Format") + 1);
                }
            } else {
                MsgReply(rcvid, 0, "Error: Unknown Command", strlen("Error: Unknown Command") + 1);
            }
        }

        // Sleep briefly to prevent CPU overutilization
        usleep(1000);
    }

    name_detach(attach, 0);
}

void Comms::sendCommand(int aircraftID, const std::string& command) {
    // Build the name for the aircraft
    std::string aircraftName = "Aircraft_" + std::to_string(aircraftID);

    // Open a connection to the aircraft
    int coid = name_open(aircraftName.c_str(), 0);
    if (coid == -1) {
    	{
    		std::lock_guard<std::mutex> lock(coutMutex);
    		std::cerr << "Failed to connect to Aircraft " << aircraftID << ": " << strerror(errno) << std::endl;
    	}

        return;
    }

    // Prefix the command with "Command:"
    std::string fullCommand = "Command:" + command;

    // Send the command
    if (MsgSend(coid, fullCommand.c_str(), fullCommand.size() + 1, NULL, 0) == -1) {
    	{
    		std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr << "Failed to send command to Aircraft " << aircraftID << ": " << strerror(errno) << std::endl;
    	}
    } else {
    	{
    		std::lock_guard<std::mutex> lock(coutMutex);
			std::cout << "Command sent to Aircraft " << aircraftID << ": " << command << std::endl;
    	}
    }

    // Close the connection
    name_close(coid);
}

