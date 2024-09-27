#include <Operator.h>
#include <iostream>

/////Constructors/////
// Constructor for Command
Command::Command(const std::string& id, double speedX, double speedY, double speedZ, double altitude)
    : aircraftID(id), newSpeedX(speedX), newSpeedY(speedY), newSpeedZ(speedZ), newAltitude(altitude) {}

// OperatorConsole constructor
OperatorConsole::OperatorConsole() {
    // Initialize the console
}

/////Aircraft related commands/////
//send command to aircraft
void OperatorConsole::sendCommand(const Command& cmd) {
    std::cout << "Sending command to aircraft " << cmd.aircraftID << ":\n";
    std::cout << "New Speed (X, Y, Z): " << cmd.newSpeedX << ", " << cmd.newSpeedY << ", " << cmd.newSpeedZ << "\n";
    std::cout << "New Altitude: " << cmd.newAltitude << "\n";
    communicateWithSystem(cmd);
}

//request aircraft info
std::string OperatorConsole::requestAircraftInfo(const std::string& aircraftID) {
    std::cout << "Requesting info for aircraft " << aircraftID << "\n";
    // Should involve a request to the computer system
    // For now, return something temporary
    return "Aircraft ID: " + aircraftID + ", Speed: (500, 200, 0), Altitude: 35000";
}

//display operator console logs
void OperatorConsole::displayConsole() {
    std::cout << "Operator Console Display\n";
    std::cout << "Commands and requests can be sent from here.\n";
}

//function to communicate with system
void OperatorConsole::communicateWithSystem(const Command& cmd) {
    // Simulate communication with the system for now
    std::cout << "Command transmitted to system for processing.\n";
}


