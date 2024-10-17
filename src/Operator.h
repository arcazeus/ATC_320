#ifndef OPERATOR_CONSOLE_H
#define OPERATOR_CONSOLE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>


class Operator{
private:
	std::vector<int> aircraftList;  // List of aircraft being managed
	void logCommand(const std::string& command); // Log the commands

public:
	Operator();

	~Operator();

    // Public member functions
    void sendCommandToAircraft(int aircraftID, const std::string& command);
    void requestAircraftInfo(int aircraftID);
    void displayInfo(const std::string& info);
};

#endif
