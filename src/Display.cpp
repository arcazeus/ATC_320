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

// Method to update the screen every 5 seconds
void Display::updateScreen() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));  // Refresh every 5 seconds
        render();  // Update the display with the latest aircraft data
    }
}

// Method to show information about a specific aircraft
void Display::showAircraftInfo(int aircraftID, const std::string& info) {
    std::cout << "Displaying information for Aircraft " << aircraftID << ": " << info << std::endl;
    aircraftData[aircraftID] = info;  // Update the internal data
}

// Method to show an alarm on the display
void Display::showAlarm(const std::string& alarmMessage) {
    std::cout << "ALARM: " << alarmMessage << std::endl;
}

// Update aircraft data that will be displayed
void Display::updateAircraftData(int aircraftID, const std::string& data) {
    aircraftData[aircraftID] = data;  // Update the internal aircraft data
}

// Internal method to render the display
void Display::render() {
    std::cout << "Updating display..." << std::endl;

    // Iterate over all the aircraft data and display it
    for (const auto& entry : aircraftData) {
        std::cout << "Aircraft " << entry.first << ": " << entry.second << std::endl;
    }
}

