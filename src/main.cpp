#include <pthread.h>
#include <iostream>
#include <vector>
#include <semaphore.h>
#include "ComSys.h"
#include "Radar.h"
#include "Aircraft.h"
#include "Comms.h"
#include "Operator.h"
#include "Display.h"

#define MAX_AIRCRAFTS 120

sem_t syncSemaphore; // Semaphore for synchronization

void initializeSynchronization(int count) {
    sem_init(&syncSemaphore, 0, count);
}

void signalSynchronization() {
    sem_post(&syncSemaphore);
}

void waitForSynchronization() {
    sem_wait(&syncSemaphore);
}

int main() {
    char buf[256];
    if (getcwd(buf, sizeof(buf)) != NULL) {
        std::cout << "Current working directory: " << buf << std::endl;
    }

    pthread_t ComSys_tid, Operator_tid, Radar_tid, Display_tid, Comms_tid, Aircraft_tid[MAX_AIRCRAFTS];
    ComSys ComSysObj;
    Radar RadarObj;
    Display DisplayObj;
    Comms CommsObj;
    Operator OperatorObj;
    std::vector<Aircraft> aircrafts;

    int numberOfAircrafts = 10; // Example aircraft count
    for (int i = 0; i < numberOfAircrafts; ++i) {
        Aircraft aircraft(i, 0.0, 0.0, 1000.0 + i * 1000.0, 500.0, 0.0, 0.0, 0.0f);
        aircrafts.push_back(aircraft);
    }

    // Initialize synchronization (1 semaphore per thread)
    initializeSynchronization(numberOfAircrafts + 5);

    // Create Aircraft threads
    for (int i = 0; i < numberOfAircrafts; ++i) {
        if (pthread_create(&Aircraft_tid[i], NULL, &Aircraft::startAircraft, &aircrafts[i]) != 0) {
            perror("Failed to create airplane thread");
            return 1;
        }
    }

    // Create other threads
    if (pthread_create(&ComSys_tid, NULL, &ComSys::startComSys, &ComSysObj) != 0 ||
        pthread_create(&Radar_tid, NULL, &Radar::startRadar, &RadarObj) != 0 ||
        pthread_create(&Operator_tid, NULL, &Operator::startOperator, &OperatorObj) != 0 ||
        pthread_create(&Display_tid, NULL, &Display::startDisplay, &DisplayObj) != 0 ||
        pthread_create(&Comms_tid, NULL, &Comms::startComms, &CommsObj) != 0) {
        perror("Failed to create one of the threads");
        return 1;
    }

    // Signal all threads to start after creation
    for (int i = 0; i < numberOfAircrafts + 5; ++i) {
        signalSynchronization();
    }

    // Join threads
    for (int i = 0; i < numberOfAircrafts; ++i) {
        if (pthread_join(Aircraft_tid[i], NULL) != 0) {
            perror("Failed to join Aircraft thread");
        }
    }
    pthread_join(ComSys_tid, NULL);
    pthread_join(Operator_tid, NULL);
    pthread_join(Radar_tid, NULL);
    pthread_join(Display_tid, NULL);
    pthread_join(Comms_tid, NULL);

    // Cleanup
    sem_destroy(&syncSemaphore);
    return 0;
}
