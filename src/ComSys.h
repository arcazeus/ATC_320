#include <vector>
#include "Aircraft.h"
#include "Operator.h"
#include "Radar.h"

using namespace std;


class ComSys{
public:

	ComSys();
	//checks to see if any collisions happened or will happen
	void checkViolations();
	//sends an alert to operator about current and future safety violation
	void operatorAlert(int id_1, int id_2);
	//sends all known aircrafts to the Display
	void sendDataDisplay();

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
