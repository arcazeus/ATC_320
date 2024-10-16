#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ComSys.h"
#include "Radar.h"
#include "Aircraft.h"
#include "Comms.h"
#include "Operator.h"
#include "Display.h"
#include "SharedMem.h"

//Thread functions
void* airplane_thread_func(void* arg){
	Aircaft();
}
void* comsys_thread_func(void* arg){
	ComSys();
}
void* operator_thread_func(void* arg){
	Operator();
}
void* display_thread_func(void* arg){
	Display();
}
void* comms_thread_func(void* arg){
	Comms();
}
void* radar_thread_func(void* arg){
	Radar();
}


int main(){
	pthread_mutex_t mutexAirplane = PTHREAD_MUTEX_INITIALIZER;

	// thread IDs
	pthread_t ComSys_tid, Operator_tid, Radar_tid, Display_tid,Comms_tid,Aircraft_tid[120];

	std::vector<Aircraft> aircrafts;
		std::ifstream file("ENROUTE");

		if (!file.is_open()) {
			std::cerr << "Error opening file: " << "ENROUTE" << std::endl;
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
			} else {
				//cout << "Creating aircraft with ID: " << ID << endl;
			}

			Aircraft aircraft = Aircraft(time, ID, x, y, z, xSpeed, ySpeed, zSpeed);
			aircrafts.push_back(aircraft);
		}

		int size = aircrafts.size();
	// Creating Airplane threads
	  for (int i = 0; i < size; i++) {
	        if (pthread_create(&Aircraft_tid[i], NULL, airplane_thread_func, (void*)&mutexAirplane) != 0) {
	            perror("Failed to create airplane thread");
	            return 1;
	        }
	    }
	// Creating Comsys thread
	  if (pthread_create(&ComSys_tid, NULL, comsys_thread_func, (void*)&mutexAirplane) != 0) {
	         perror("Failed to create ComSys thread");
	         return 1;
	     }

	// Creating Operator thread
	  if (pthread_create(&Operator_tid, NULL, operator_thread_func, (void*)&mutexAirplane) != 0) {
	 	         perror("Failed to create Operator thread");
	 	         return 1;
	 	     }

	// Creating Radar thread
	  if (pthread_create(&Radar_tid, NULL, radar_thread_func, (void*)&mutexAirplane) != 0) {
	 	         perror("Failed to create Radar thread");
	 	         return 1;
	 	     }
	// Creating Display thread
	  if (pthread_create(&Display_tid, NULL, display_thread_func, (void*)&mutexAirplane) != 0) {
	 	         perror("Failed to create Display thread");
	 	         return 1;
	 	     }
	// Creating Comms thread
	  if (pthread_create(&Comms_tid, NULL, comms_thread_func, (void*)&mutexAirplane) != 0) {
	 	         perror("Failed to create Comms thread");
	 	         return 1;
	 	     }

	//joining threads
	  if (pthread_join(ComSys_tid, NULL) != 0) {
	          perror("Failed to join ComSys thread");
	          return 1;
	      }

	  if (pthread_join(Operator_tid, NULL) != 0) {
	          perror("Failed to join ComSys thread");
	          return 1;
	      }

	  if (pthread_join(Radar_tid, NULL) != 0) {
	          perror("Failed to join ComSys thread");
	          return 1;
	      }

	  if (pthread_join(Display_tid, NULL) != 0) {
	          perror("Failed to join ComSys thread");
	          return 1;
	      }

	  if (pthread_join(Comms_tid, NULL) != 0) {
	          perror("Failed to join ComSys thread");
	          return 1;
	      }


	return 0;
}
