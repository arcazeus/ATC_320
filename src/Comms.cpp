
#include "Comms.h"

CommunicationSys::Communicationsys(){}


void Communication::sendCommand(const string& aicraftID, const Command& command)
{
	lock_guard<mutex> lock (mtx);






}


