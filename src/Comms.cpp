
#include "Comms.h"

Comms::Comms() {

}

void* Comms::startComms(void* arg) {
	((Comms*) arg)->runComms();
		return NULL;
}

void Comms::runComms(){
	std::cout << "Communication System started...";
	while(1){
		cout<<"help"<<endl;
	}
}
void Comms::sendCommand(const string &aircraftID, const Comms &command)

{

}

