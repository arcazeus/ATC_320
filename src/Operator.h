#ifndef OPERATOR_CONSOLE_H
#define OPERATOR_CONSOLE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>


class Operator{
private:
	int connectionId;
	std::vector<int> aircraftList;  // List of aircraft being managed
	void logCommand(const std::string& command); // Log the commands
	const char *shm_name = "/shm_aircraft_data";
			const int SIZE = sizeof(SharedMemory);

			// Open shared memory
			int shm_fd = shm_open(shm_name, O_RDWR, 0666);
			SharedMemory *shared = static_cast<SharedMemory*>(mmap(0, SIZE,
			PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));

public:
	Operator();

	~Operator();

    // Public member functions
    void sendCommandToAircraft(int aircraftID, const std::string& command);
    void requestAircraftInfo(int aircraftID);
    void displayInfo(const std::string& info);
    void checkViolationFromCS();
    static void* startOperator(void* arg);
    void runOperator();

    void setConnectionId(int connectionId);
	int getConnectionId() const;
};

#endif
