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
	void updateDisplay();
	static void* startDisplay(void*);
	void runDisplay();

private:
	//Actual Display Grid
		const int Z = 25000;
		const int X = 100000;
		//Scaled Display
		const int scaledZ= Z/1000;
		const int scaledX = X/1000;

		std::string buffer;
};

#endif // DISPLAY_H

