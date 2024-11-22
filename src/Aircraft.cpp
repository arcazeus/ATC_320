
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


Aircraft::Aircraft(int id, double xpost, double ypost, double zpost,
                   double xspeed, double yspeed, double zspeed, float time)
    : id(id), xpost(xpost), ypost(ypost), zpost(zpost),
      xspeed(xspeed), yspeed(yspeed), zspeed(zspeed), time(time) {
}

void* Aircraft::startAircraft(void* arg) {
    ((Aircraft*) arg)->runAircraft();
    return NULL;
}

void Aircraft::runAircraft() {
    std::string aircraftName = "Aircraft_" + std::to_string(this->id);

    name_attach_t* attach = name_attach(NULL, aircraftName.c_str(), 0);
    if (attach == NULL) {
        std::cerr << "Error: Failed to register Aircraft " << this->id << " with name service!" << std::endl;
        return;
    }

    std::cout << "Aircraft " << this->id << " registered as '" << aircraftName << "'." << std::endl;

    while (true) {
        // Update position
        updatePosition();

        // Check for messages
        checkForMessages(attach);

        // Sleep for 1 second
        sleep(1);
    }

    name_detach(attach, 0);
}

void Aircraft::updatePosition() {
    // Update position based on speed
    this->xpost += this->xspeed;
    this->ypost += this->yspeed;
    this->zpost += this->zspeed;

    this->time += 1.0f;
    {
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Aircraft " << this->id << " at time " << this->time
				  << " position updated to ("
				  << this->xpost << ", "
				  << this->ypost << ", "
				  << this->zpost << ")." << std::endl;
	}
}

void Aircraft::checkForMessages(name_attach_t* attach) {
    char msg[256];
    int rcvid;

    // Non-blocking receive
    rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
    if (rcvid != -1) {
        handleMessage(rcvid, msg);
    }
}

void Aircraft::handleMessage(int rcvid, const char* msg) {
    std::string receivedMessage(msg);

    if (receivedMessage == "RadarRequest") {
        // Respond to Radar
        std::stringstream response;
        response << "ID:" << this->id
                 << " X:" << this->xpost
                 << " Y:" << this->ypost
                 << " Z:" << this->zpost
                 << " SpeedX:" << this->xspeed
                 << " SpeedY:" << this->yspeed
                 << " SpeedZ:" << this->zspeed;

        MsgReply(rcvid, 0, response.str().c_str(), response.str().size() + 1);
    } else if (receivedMessage.find("Command:") == 0) {
        // Execute command
        std::string command = receivedMessage.substr(8);
        executeCommand(command);

        // Acknowledge
        MsgReply(rcvid, 0, "Command Executed", strlen("Command Executed") + 1);
    } else {
        MsgReply(rcvid, 0, "Unknown Message", strlen("Unknown Message") + 1);
    }
}

void Aircraft::executeCommand(const std::string& command) {
    if (command.find("ChangeSpeedX:") == 0) {
        double newSpeedX = std::stod(command.substr(13));
        this->xspeed = newSpeedX;
        std::cout << "Aircraft " << this->id << " changed SpeedX to " << newSpeedX << std::endl;
    } else if (command.find("ChangeSpeedY:") == 0) {
        double newSpeedY = std::stod(command.substr(13));
        this->yspeed = newSpeedY;
        std::cout << "Aircraft " << this->id << " changed SpeedY to " << newSpeedY << std::endl;
    } else if (command.find("ChangeSpeedZ:") == 0) {
        double newSpeedZ = std::stod(command.substr(13));
        this->zspeed = newSpeedZ;
        std::cout << "Aircraft " << this->id << " changed SpeedZ to " << newSpeedZ << std::endl;
    } else if (command.find("ChangeAltitude:") == 0) {
        double newAltitude = std::stod(command.substr(15));
        this->zpost = newAltitude;
        std::cout << "Aircraft " << this->id << " changed Altitude to " << newAltitude << std::endl;
    } else {
        std::cout << "Aircraft " << this->id << " received unknown command: " << command << std::endl;
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

void display(const Aircraft &aircraft) {
    std::cout << "Aircraft ID: " << aircraft.getId() << "\n"
              << "Time: " << aircraft.getTime() << "\n"
              << "X Speed: " << aircraft.getSpeedX() << "\n"
              << "Y Speed: " << aircraft.getSpeedY() << "\n"
              << "Z Speed: " << aircraft.getSpeedZ() << "\n"
              << "X Position: " << aircraft.getPositionX() << "\n"
              << "Y Position: " << aircraft.getPositionY() << "\n"
              << "Z Position: " << aircraft.getPositionZ() << "\n";
}
