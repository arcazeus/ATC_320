#ifndef Radar_H
#define Radar_H

#include <vector>
#include <string>
#include "Aircraft.h"
#include "Operator.h"
#include "ComSys.h"
#include <map>

#define MAX_AIRCRAFT 120;

using namespace std;

class Radar {
public:
	Radar();

	static void* startRadar(void* arg);
	void runRadar();
	void addAircraft();
	void scanForAircraft();
	void storeAirSpaceHistory();
	void updateAircraftData(int aircraftID, const std::string& data);
	void setAircraftIDs(const std::vector<int>& aircraftIDs);

private:
	std::string History;
	std::vector<int> aircraftIDs;
	std::map<int, std::string> aircraftDataMap;

};

#endif
