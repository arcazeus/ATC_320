#include <vector>
#include "Aircraft.h"
#include "Operator.h"
#include "ComSys.h"

using namespace std;


class Radar{
public:
	Radar();

	void addAircraft(Aircraft*);
	void scanForAircrafts();
	Aircraft** getAircrafts();
	int getTotalAircrafts();


private:


};

