#include "Display.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

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

// In Display.cpp

void* Display::startDisplay(void* arg) {
    ((Display*) arg)->runDisplay();
    return NULL;
}

void Display::runDisplay() {
    name_attach_t* attach = name_attach(NULL, "displayServer", 0);
    if (attach == NULL) {
        std::cerr << "Error: Failed to register Display with name service!" << std::endl;
        return;
    }

    while (true) {
        char msg[256];
        int rcvid = MsgReceive(attach->chid, msg, sizeof(msg), NULL);
        if (rcvid != -1) {
            // Parse and display the data
            std::string data(msg);
            std::cout << "Display Update: " << data << std::endl;

            // Send acknowledgment
            MsgReply(rcvid, 0, NULL, 0);
        }

        // Sleep or perform other tasks
        sleep(1);
    }

    name_detach(attach, 0);
}


