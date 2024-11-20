#ifndef Radar_H
#define Radar_H

#include <vector>
#include <string>
#include "Aircraft.h"
#include "Operator.h"
#include "ComSys.h"

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

private:
	std::string History = "History.txt";
	std::vector<Aircraft> Aircrafts;

};

#endif
