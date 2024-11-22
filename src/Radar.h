#ifndef Radar_H
#define Radar_H

#include <vector>
#include <string>
#include "Aircraft.h"
#include "Operator.h"
#include "ComSys.h"
#include <map>

#define MAX_AIRCRAFT 120

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
<<<<<<< HEAD
	std::string History = "History.txt";
	std::vector<Aircraft> Aircrafts;
	const char *shm_name = "/shm_aircraft_data";
			const int SIZE = sizeof(SharedMemory);

			// Open shared memory
			int shm_fd = shm_open(shm_name, O_RDWR, 0666);
			SharedMemory *shared = static_cast<SharedMemory*>(mmap(0, SIZE,
			PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));
=======
	std::string History;
	std::vector<int> aircraftIDs;
	std::map<int, std::string> aircraftDataMap;
>>>>>>> peter

};

#endif
