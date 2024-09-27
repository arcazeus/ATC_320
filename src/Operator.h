#ifndef OPERATOR_CONSOLE_H
#define OPERATOR_CONSOLE_H

#include <string>
#include <vector>


// Class representing an aircraft command
class Command {
public:
    std::string aircraftID;
    double newSpeedX;
    double newSpeedY;
    double newSpeedZ;
    double newAltitude;

    Command(const std::string& id, double speedX, double speedY, double speedZ, double altitude);
};

class Operator{
private:
	void communicateWithSystem(const Command& cmd);

public:
	Operator();

	void sendCommand(const Command& cmd);

	std::string requestAircraftInfo(const std::string& aircraftID);

	void displayConsole();
};

#endif
