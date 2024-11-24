#ifndef Radar_H
#define Radar_H

#include <vector>
#include <string>
#include "Aircraft.h"
#include "Operator.h"
#include "ComSys.h"
#include <map>
#include "Logger.h"

#define MAX_AIRCRAFT 120

using namespace std;

class Radar {
public:
	Radar();

	static void* startRadar(void *arg);
	void checkForMessages(name_attach_t *attach);
	void handleMessage(int rcvid, const char *msg);
	void runRadar();
	void addAircraft(Aircraft);
	void scanForAircraft();
	void storeAirSpaceHistory();
	void updateAircraftData(int aircraftID, const std::string &data);
	void setAircraftIDs(const std::vector<int> &aircraftIDs);

private:

	const char *History = "logFile";
	std::vector<int> aircraftIDs;
	std::map<int, std::string> aircraftDataMap;
	std::vector<Aircraft> planes;
	Logger Airspace;

};

#endif
