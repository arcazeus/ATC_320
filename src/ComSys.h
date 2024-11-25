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
	void checkForMessages(name_attach_t *t);
	void handleMessage(int rcvid, const char *msg);
	void checkViolations();
	void operatorAlert(int id_1, int id_2);
	void sendDataDisplayAllAircraft();
	void listenForMessages(name_attach_t *attach);
	void setAircraftList();

private:
	int TotalNumAircraft;
	int n;
	vector<Aircraft> aircraft;

};

#endif
