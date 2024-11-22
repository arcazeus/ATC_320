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
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include <cmath>
#include "globals.h"
#include "ComSys.h"

using namespace std;

ComSys::ComSys() : TotalNumAircraft(0), n(0) {
    // Constructor implementation

    // Register with the name service here
    attach = name_attach(NULL, "computerSystemServer", 0);
    if (attach == NULL) {
        std::cerr << "Error: Failed to register Computer System with name service in constructor!" << std::endl;
        // Handle error appropriately (e.g., throw exception or set a flag)
    } else {
        std::cout << "Computer System registered with name service in constructor." << std::endl;
    }
}

ComSys::~ComSys() {
    if (attach != NULL) {
        name_detach(attach, 0);
        attach = NULL;
    }
}

void* ComSys::startComSys(void* arg) {
	((ComSys*) arg)->runComSys();
	return NULL;


}

void ComSys::runComSys() {
    std::cout << "Computer System Booting up..." << std::endl;

    if (attach == NULL) {
        std::cerr << "Error: Computer System is not registered with name service!" << std::endl;
        return;
    }

    while (true) {
        char msg[256];        // Buffer for received messages
        int rcvid;            // Receive ID

        // Wait for a message from the Operator
        rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
        if (rcvid != -1) {
            handleMessage(rcvid, msg);
        }

        // Check for violations
        checkViolations();

        // Send data to Display
        sendDataDisplayAllAircraft();

        sleep(5);
    }
}

void ComSys::sendDataDisplayAllAircraft(){
	for (int i=0; i < TotalNumAircraft; i++){
		sendDataDisplay(i);
	}
}

void ComSys::handleMessage(int rcvid, const char* msg) {
    char reply[256];
    std::string receivedMessage(msg);

    if (receivedMessage.find("RequestInfo") == 0) {
        // Extract aircraft ID
        int aircraftId = std::stoi(receivedMessage.substr(11));
        if (aircraftId >= 0 && aircraftId < TotalNumAircraft) {
            std::stringstream info;
            info << "Aircraft " << aircraft[aircraftId].getId()
                 << " Position: X=" << aircraft[aircraftId].getPositionX()
                 << ", Y=" << aircraft[aircraftId].getPositionY()
                 << ", Z=" << aircraft[aircraftId].getPositionZ();
            strcpy(reply, info.str().c_str());
        } else {
            strcpy(reply, "Error: Invalid Aircraft ID");
        }
    } else if (receivedMessage.find("ChangeN") == 0) {
        // Change the parameter n
        n = std::stoi(receivedMessage.substr(7));
        strcpy(reply, "Parameter n updated.");
    } else {
        strcpy(reply, "Error: Unknown command");
    }

    // Send a reply back to the sender
    MsgReply(rcvid, 0, reply, strlen(reply) + 1);
}

void ComSys::setAircraftList(const std::vector<Aircraft>& aircraftList) {
    this->aircraft = aircraftList;
    this->TotalNumAircraft = aircraftList.size();
}

//minimum Z=1000, X=3000, Y=3000
void ComSys::checkViolations() {
    for (int i = 0; i < TotalNumAircraft - 1; i++) {
        for (int j = i + 1; j < TotalNumAircraft; j++) {
            // Current positions
            double deltaX = aircraft[i].getPositionX() - aircraft[j].getPositionX();
            double deltaY = aircraft[i].getPositionY() - aircraft[j].getPositionY();
            double deltaZ = aircraft[i].getPositionZ() - aircraft[j].getPositionZ();


            // Future positions after n seconds
            double futureDeltaX = (aircraft[i].getPositionX() + aircraft[i].getSpeedX() * n) -
                                  (aircraft[j].getPositionX() + aircraft[j].getSpeedX() * n);
            double futureDeltaY = (aircraft[i].getPositionY() + aircraft[i].getSpeedY() * n) -
                                  (aircraft[j].getPositionY() + aircraft[j].getSpeedY() * n);
            double futureDeltaZ = (aircraft[i].getPositionZ() + aircraft[i].getSpeedZ() * n) -
                                  (aircraft[j].getPositionZ() + aircraft[j].getSpeedZ() * n);

            // Check current violation
            if (std::abs(deltaX) <= 3000 && std::abs(deltaY) <= 3000 && std::abs(deltaZ) <= 1000) {
                operatorAlert(aircraft[i].getId(), aircraft[j].getId());
            }

            // Check future violation
            if (std::abs(futureDeltaX) <= 3000 && std::abs(futureDeltaY) <= 3000 && std::abs(futureDeltaZ) <= 1000) {
                operatorAlert(aircraft[i].getId(), aircraft[j].getId());
            }
        }
    }

}

void ComSys::operatorAlert(int id_1, int id_2) {
    std::string alertMessage = "Violation detected between aircraft " + std::to_string(id_1) + " and aircraft " + std::to_string(id_2);

    // Connect to the Operator
    int coid = name_open("operatorServer", 0);
    if (coid == -1) {
    	{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr << "Failed to connect to Operator: " << strerror(errno) << std::endl;
		}
        return;
    }

    // Send the alert message
    if (MsgSend(coid, alertMessage.c_str(), alertMessage.size() + 1, NULL, 0) == -1) {
    	{
			std::lock_guard<std::mutex> lock(coutMutex);
			std::cerr << "Failed to send alert to Operator: " << strerror(errno) << std::endl;
		}
    }

    // Close the connection
    name_close(coid);
}

void ComSys::sendDataDisplay(int id) {
    // Prepare the data
    std::stringstream data;
    data << aircraft[id].getId() << " "
         << aircraft[id].getPositionX() << " "
         << aircraft[id].getPositionY() << " "
         << aircraft[id].getPositionZ();

    // Connect to the Display
    int coid = name_open("displayServer", 0);
    if (coid == -1) {
        std::cerr << "Failed to connect to Display: " << strerror(errno) << std::endl;
        return;
    }

    // Send the data
    if (MsgSend(coid, data.str().c_str(), data.str().size() + 1, NULL, 0) == -1) {
        std::cerr << "Failed to send data to Display: " << strerror(errno) << std::endl;
    }

    // Close the connection
    name_close(coid);
}

