#ifndef Radar
#define Radar

#include <vector>
#include "Aircraft.h"
#include "Operator.h"
#include "ComSys.h"

#define MAX_AIRCRAFTS 120

using namespace std;

class Radar{
public:
	Radar();

	void addAircraft(Aircraft*);
	void scanForAircrafts();
	void storeAirSpaceHistory(std::string s);

private:
	std::string History = "History.txt";
	vector<Aircraft> Aircrafts;

};

#endif
