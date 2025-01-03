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
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <pthread.h>
#include "globals.h"
#include <sys/neutrino.h>
#include <sys/dispatch.h>
#include "Aircraft.h"
#include "Comms.h"
#include "Operator.h"
#include "Display.h"
#include "cTimer.h"

Aircraft::Aircraft() :
		id(0), xpost(0.0), ypost(0.0), zpost(0.0), xspeed(0.0), yspeed(0.0), zspeed(
				0.0), time(0.0f) {
}

Aircraft::Aircraft(int id, double xpost, double ypost, double zpost,
		double xspeed, double yspeed, double zspeed, float time) :
		id(id), xpost(xpost), ypost(ypost), zpost(zpost), xspeed(xspeed), yspeed(
				yspeed), zspeed(zspeed), time(time) {
}

void* Aircraft::startAircraft(void *arg) {
	((Aircraft*) arg)->runAircraft();
	return NULL;
}

void Aircraft::runAircraft() {

	std::string aircraftName = "Aircraft_" + std::to_string(this->id);

	name_attach_t *attach = name_attach(NULL, aircraftName.c_str(), 0);
	if (attach == NULL) {
		std::cerr << "Error: Failed to register Aircraft " << this->id
				<< " with name service!" << std::endl;
		return;
	}

	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Aircraft " << this->id << " registered as '"
				<< aircraftName << "'." << std::endl;
	}


	cTimer time = cTimer(1, 0);
	time.startTimer();
	while (true) {
		time.tick();
		time.waitTimer();
		// Check for messages
		checkForMessages(attach);
		// Update position
		updatePosition();
		time.tock();
	}

	name_detach(attach, 0);
}

void Aircraft::updatePosition() {
	std::lock_guard<std::mutex> lock(coutMutex);

	// Update position based on speed
	this->xpost += this->xspeed;
	this->ypost += this->yspeed;
	this->zpost += this->zspeed;

	this->time += 1.0f;
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//	display(*this);
	/*

	 std::cout << "Aircraft " << this->id << " at time " << this->time
	 << " position updated to (" << this->xpost << ", "
	 << this->ypost << ", " << this->zpost << ")." << std::endl;
	 */

}

void Aircraft::checkForMessages(name_attach_t *attach) {
	char msg[256];
	int rcvid;

	{
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Aircraft waiting for messages" << std::endl;
	}

	// Non-blocking receive
	rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
	if (rcvid != -1) {
		handleMessage(rcvid, msg);
	}
}



void Aircraft::listenForCommands() {
    std::string aircraftName = "Aircraft_" + std::to_string(id);
    name_attach_t *attach = name_attach(NULL, aircraftName.c_str(), 0);
    if (attach == NULL) {
        std::cerr << "Error: Failed to register Aircraft with name service!" << std::endl;
        return;
    }

    char msg[256];
    while (true) {
        int rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
        if (rcvid != -1) {
            handleMessage(rcvid, msg);
        }
    }
}

void Aircraft::handleMessage(int rcvid, const char *msg) {
    std::string receivedMessage(msg);

    if (receivedMessage.find("RadarRequest") == 0) {
        // Respond to Radar
        Aircraft data = Aircraft(id, xpost, ypost, zpost, xspeed, yspeed, zspeed, time);
        MsgReply(rcvid, 0, &data, sizeof(data));
    } else if (receivedMessage.find("ChangeSpeedZ:") == 0) {
    	std::cout<<"Previous Z Speed:" << getSpeedZ() << endl;
        double newYspeed = std::stod(receivedMessage.substr(13));
        zspeed = newYspeed;

        std::cout << "Aircraft " << id << " Z-axis speed changed to " << newYspeed << std::endl;
        this->setZspeed(yspeed);
        std::cout<<"AirCraft new speed: " << getSpeedZ();
        MsgReply(rcvid, 0, "Z-axis Speed Changed", strlen("Z-axis Speed Changed") + 1);
    } else {
        MsgReply(rcvid, 0, "Unknown Command", strlen("Unknown Command") + 1);
    }
}




void Aircraft::executeCommand(const std::string &command) {
	{
		std::lock_guard<std::mutex> lock(coutMutex);
		if (command.find("ChangeSpeedX:") == 0) {
			double newSpeedX = std::stod(command.substr(13));
			this->xspeed = newSpeedX;
			std::cout << "Aircraft " << this->id << " changed SpeedX to "
					<< newSpeedX << std::endl;
		} else if (command.find("ChangeSpeedY:") == 0) {
			double newSpeedY = std::stod(command.substr(13));
			this->yspeed = newSpeedY;
			std::cout << "Aircraft " << this->id << " changed SpeedY to "
					<< newSpeedY << std::endl;
		} else if (command.find("ChangeSpeedZ:") == 0) {
			double newSpeedZ = std::stod(command.substr(13));
			this->zspeed = newSpeedZ;
			std::cout << "Aircraft " << this->id << " changed SpeedZ to "
					<< newSpeedZ << std::endl;
		} else {
			std::cout << "Aircraft " << this->id
					<< " received unknown command: " << command << std::endl;
		}

	}

}

// Getters
float Aircraft::getTime() const {
	return time;
}
int Aircraft::getId() const {
	return id;
}
double Aircraft::getSpeedX() const {
	return xspeed;
}
double Aircraft::getSpeedY() const {
	return yspeed;
}
double Aircraft::getSpeedZ() const {
	return zspeed;
}
double Aircraft::getPositionX() const {
	return xpost;
}
double Aircraft::getPositionY() const {
	return ypost;
}
double Aircraft::getPositionZ() const {
	return zpost;
}

// Setters
void Aircraft::setTime(float t) {
	time = t;
}
void Aircraft::setId(int i) {
	id = i;
}
void Aircraft::setXspeed(double xs) {
	xspeed = xs;
}
void Aircraft::setYspeed(double ys) {
	yspeed = ys;
}
void Aircraft::setZspeed(double zs) {
	zspeed = zs;
}
void Aircraft::setXpost(double xp) {
	xpost = xp;
}
void Aircraft::setYpost(double yp) {
	ypost = yp;
}
void Aircraft::setZpost(double zp) {
	zpost = zp;
}

//void Aircraft::display(const Aircraft &aircraft) {
//	{
//		std::cout << "Aircraft ID: " << aircraft.getId() << "\n" << "Time: "
//
//		<< aircraft.getTime() << "\n" << "X Speed: " << aircraft.getSpeedX()
//				<< "\n" << "Y Speed: " << aircraft.getSpeedY() << "\n"
//				<< "Z Speed: " << aircraft.getSpeedZ() << "\n" << "X Position: "
//				<< aircraft.getPositionX() << "\n" << "Y Position: "
//				<< aircraft.getPositionY() << "\n" << "Z Position: "
//				<< aircraft.getPositionZ() << "\n";
//	}
//}
