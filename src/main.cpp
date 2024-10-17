#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include "ComSys.h"
#include "Radar.h"
#include "Aircraft.h"
#include "Comms.h"
#include "Operator.h"
#include "Display.h"

#define MAX_AIRCRAFTS 120

int main() {
    // Thread IDs
    pthread_t ComSys_tid, Operator_tid, Radar_tid, Display_tid, Comms_tid, Aircraft_tid[MAX_AIRCRAFTS];

    // Instantiate objects
    ComSys ComSysObj;
    Radar RadarObj;
    Display DisplayObj;
    Comms CommsObj;
    Operator OperatorObj;

    std::vector<Aircraft> aircrafts;
    std::ifstream file("ENROUTE");

    if (!file.is_open()) {
        std::cerr << "Error opening file: ENROUTE" << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        float time;
        int ID;
        double x, y, z, xSpeed, ySpeed, zSpeed;

        if (!(iss >> time >> ID >> x >> y >> z >> xSpeed >> ySpeed >> zSpeed)) {
            std::cerr << "Error reading line: " << line << std::endl;
            continue; // Skip lines that do not match the expected format
        }

        Aircraft aircraft(time, ID, x, y, z, xSpeed, ySpeed, zSpeed);
        aircrafts.push_back(aircraft);
    }

    int size = aircrafts.size();
    int count = 0;

    // Creating Aircraft threads
    for (int i = 0; i < size; i++) {
        Aircraft* aircraftPtr = &aircrafts[i];  // Get the address of the current aircraft
        if (pthread_create(&Aircraft_tid[i], NULL, [](void* arg) -> void* {
            Aircraft* ac = static_cast<Aircraft*>(arg);
            ac->runAircraft();  // Call runAircraft for the Aircraft object
            return NULL;
        }, aircraftPtr) != 0) {
            perror("Failed to create airplane thread");
            return 1;
        }
        ++count;
        RadarObj.addAircraft();
    }

    // Creating Comsys thread
    if (pthread_create(&ComSys_tid, NULL, [](void* arg) -> void* {
        ComSys* comsys = static_cast<ComSys*>(arg);
        comsys->runComSys();
        return NULL;
    }, &ComSysObj) != 0) {
        perror("Failed to create ComSys thread");
        return 1;
    }

    // Creating Radar thread
    if (pthread_create(&Radar_tid, NULL, [](void* arg) -> void* {
        Radar* radar = static_cast<Radar*>(arg);
        radar->runRadar();
        return NULL;
    }, &RadarObj) != 0) {
        perror("Failed to create Radar thread");
        return 1;
    }

    // Creating Operator thread
    if (pthread_create(&Operator_tid, NULL, [](void* arg) -> void* {
        Operator* oper = static_cast<Operator*>(arg);
        oper->runOperator();
        return NULL;
    }, &OperatorObj) != 0) {
        perror("Failed to create Operator thread");
        return 1;
    }

    // Creating Display thread
    if (pthread_create(&Display_tid, NULL, [](void* arg) -> void* {
        Display* display = static_cast<Display*>(arg);
        display->runDisplay();
        return NULL;
    }, &DisplayObj) != 0) {
        perror("Failed to create Display thread");
        return 1;
    }

    // Creating Comms thread
    if (pthread_create(&Comms_tid, NULL, [](void* arg) -> void* {
        Comms* comms = static_cast<Comms*>(arg);
        comms->runComms();
        return NULL;
    }, &CommsObj) != 0) {
        perror("Failed to create Comms thread");
        return 1;
    }

    // Joining threads
    for (int i = 0; i < count; i++) {
        if (pthread_join(Aircraft_tid[i], NULL) != 0) {
            perror("Failed to join Aircraft Thread");
            return 1;
        }
    }

    if (pthread_join(ComSys_tid, NULL) != 0) {
        perror("Failed to join ComSys thread");
        return 1;
    }

    if (pthread_join(Operator_tid, NULL) != 0) {
        perror("Failed to join Operator thread");
        return 1;
    }

    if (pthread_join(Radar_tid, NULL) != 0) {
        perror("Failed to join Radar thread");
        return 1;
    }

    if (pthread_join(Display_tid, NULL) != 0) {
        perror("Failed to join Display thread");
        return 1;
    }

    if (pthread_join(Comms_tid, NULL) != 0) {
        perror("Failed to join Comms thread");
        return 1;
    }

    return 0;
}
