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

<<<<<<< HEAD
	// Public methods
	void updateScreen(); // Updates the display with current aircraft information
	void showAircraftInfo(int aircraftID, const std::string &info); // Show specific info about an aircraft
	void showAlarm(const std::string &alarmMessage); // Show an alarm on the display

	// Method to update the aircraft data
	void updateAircraftData(int aircraftID, const std::string &data);

private:
	std::unordered_map<int, std::string> aircraftData; // Stores aircraft info for display
	void render();  // Internal method to render the display
=======
    // Public methods
    void showAlarm(const std::string& alarmMessage);  // Show an alarm on the display
    void updateDisplay();
    void runDisplay();

private:
>>>>>>> origin/peter
};

#endif // DISPLAY_H

