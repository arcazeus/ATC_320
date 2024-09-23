#include <vector>
#include "Aircraft.h"
#include "Operator.h"
#include "Radar.h"

using namespace std;


class ComSys{
public:
	ComSys();

	std::vector<int> checkViolations();
	void operatorAlert();
	std::vector<int> sendDataDisplay();
private:


};
