#ifndef OPERATOR_CONSOLE_H
#define OPERATOR_CONSOLE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"


class Operator{
private:
	int connectionId;
	std::vector<int> aircraftList;  // List of aircraft being managed
	void logCommand(const std::string& command); // Log the commands


public:
	Operator();

	~Operator();

    // Public member functions
    void sendCommandToAircraft(int aircraftID, const std::string& command);
    void requestAircraftInfo(int aircraftID);
    void changeParameterN(int newN);
    void checkForMessages(name_attach_t *attach);
    void handleMessage(int rcvid, const char *msg);
    static void* startOperator(void* arg);
    void runOperator();
    //get data from user
    // construct a msg from that
    // isolate ID and use function that does send(R,m)
    //send to Comms
    void giveCommand(int aircraftID, int speed);
    void send(int R, string m)

private:
	Logger OperatorLog;
//	const char*  commands="Operatorcommands";
	std::ofstream logFile;
};

#endif
