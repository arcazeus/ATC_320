#include <vector>
#include "Aircraft.h"
#include "Operator.h"
#include "Radar.h"

using namespace std;


class ComSys{
public:
	ComSys();
	//checks to see if any collisions happened or will happen
	std::vector<int> checkViolations();
	//sends an alert to operator about current and future safety violation
	void operatorAlert();
	//sends all known aircrafts to the Display
	std::vector<int> sendDataDisplay();

	//Common setters & getters
	void setTotalNumAircraft();
	int getNumAircraft();

private:
int TotalNumAircraft;
//look ahead parameter;must be set by operator.
int n;
};
