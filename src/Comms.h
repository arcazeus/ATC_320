//transmit controller commands to aircraft
//contain commands for operator console to tell aircraft what to do
//(change speed, altitude or position

// Make sure that commands are delivered
// Handle multiple commands from operators and responses from aircraft
// Thread safety

//Need communication between Operator and Aircraft
//Message Queues (which ones are more important)
// Socket Programming?
// Memory Sharing
#include <iostream>
#include <mutex>
#include "ComSys.h"
#include <string>

class Comms {

public:
//Constructor

	Comms();
	void runComms();
//send command to an aircraft
	void sendCommand(const string &aircraftID, const Comms &command);

//receive response from an aircraft
	void receiveResponse(const string &aircraftID);

private:

	void logCommand(const Comms &command); // just logging details of command
	void logResponse(const string &aircraftID, const string &response);

	bool isAricraftAvailable(const string &aircraftID);

};

