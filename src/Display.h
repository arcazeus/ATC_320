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
    name_attach_t *attach;

};

#endif // DISPLAY_H

