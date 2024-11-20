#include "Display.h"
#include <iostream>
#include <thread>
#include <chrono>

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
	 ((Display*) arg)->runDisplay();
	 	return NULL;
}

void Display::runDisplay(){
	std::cout << "Initializing Display" << std::endl;
	this->updateDisplay();
}


