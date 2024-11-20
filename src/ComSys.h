
#ifndef ComSys_H
#define ComSys_H

#include <iostream>
#include <cstring>
#include <vector>
#include <mutex>
#include <thread>
#include "Aircraft.h"
#include "Operator.h"
#include "Radar.h"

#define MAX_AIRCRAFTS 120

using namespace std;

struct SharedMemory {
	Aircraft aircrafts[MAX_AIRCRAFTS];
	pthread_mutex_t MasterLock = PTHREAD_MUTEX_INITIALIZER;
};
class ComSys {
public:

	static void* startComSys(void* arg);
	void runComSys();
	//checks to see if any collisions happened or will happen
	void checkViolations();
	void checkOperatorAlert();
	//sends an alert to operator about current and future safety violation
	void operatorAlert(int id_1, int id_2);
	//sends all known aircrafts to the Display
	void sendDataDisplay(int id);

	//Common setters & getters
	void setTotalNumAircraft();
	int getNumAircraft();

private:
	int TotalNumAircraft;
//look ahead parameter;must be set by operator.
	int n;
//
	vector<Aircraft> aircraft;
};

#endif
