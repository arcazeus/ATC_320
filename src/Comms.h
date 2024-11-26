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
	static void* startComms(void *arg);
	void runComms();
	void checkForMessage(name_attach_t *attach);
	void handleMessage(int rcvid, const char *msg);
    void sendCommandToAircraft(int aircraftID, const std::string &message);

private:
	int aircraftID;
	std::string command;

};

