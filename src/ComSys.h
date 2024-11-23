#ifndef ComSys_H
#define ComSys_H

#include <iostream>
#include <semaphore.h>
#include <cstring>
#include <vector>
#include <mutex>
#include <thread>
#include "Aircraft.h"
#include "Operator.h"
#include "Radar.h"
#include <sys/neutrino.h>
#include <sys/dispatch.h>

#define MAX_AIRCRAFTS 120

using namespace std;

class ComSys {
public:

	ComSys();

	~ComSys();
	static void* startComSys(void *arg);
	void runComSys();

	void handleMessage(int rcvid, const char *msg);
	void checkViolations();
	void operatorAlert(int id_1, int id_2);
	void sendDataDisplay(int id);
	void sendDataDisplayAllAircraft();
	void setAircraftList();

private:
	int TotalNumAircraft;
	int n;
	vector<Aircraft> aircraft;
	name_attach_t *attach;
};

#endif
