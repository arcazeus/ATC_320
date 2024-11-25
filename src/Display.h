#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <vector>
#include <unordered_map>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

class Display {
public:
	// Constructor and Destructor
	Display();
	~Display();

	// Public methods
	void updateDisplay();
	static void* startDisplay(void*);
	void runDisplay();
	void checkForMessages(name_attach_t *t);
	void handleMessages(int rcvid, const char *msg);

private:
	//Actual Display Grid
		const int Z = 25000;
		const int X = 100000;
		//Scaled Display
		const int scaledZ= Z/1000;
		const int scaledX = X/1000;

		std::string buffer;
		std::vector<char> grid;
};

#endif // DISPLAY_H

