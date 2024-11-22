#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>
#include <unordered_map>

class Display {
public:
	// Constructor and Destructor
	Display();
	~Display();



    // Public methods
    void showAlarm(const std::string& alarmMessage);  // Show an alarm on the display
    void updateDisplay();
    static void* startDisplay(void*);
    void runDisplay();

private:
    const char *shm_name = "/shm_aircraft_data";
    		const int SIZE = sizeof(SharedMemory);

    		// Open shared memory
    		int shm_fd = shm_open(shm_name, O_RDWR, 0666);
    		SharedMemory *shared = static_cast<SharedMemory*>(mmap(0, SIZE,
    		PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0));
};

#endif // DISPLAY_H

