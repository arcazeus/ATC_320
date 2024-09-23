#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <string>
#include "Radar.h"
#include "ComSys.h"
#include "Aircraft.h"

void Radar::addAircraft(){

}

void Radar::scanForaAircraft(){
	std:cout<<"Scanning...\n";
	for(int i =0; i<MAX_AIRCRAFTS;i++){

	}

}

void Radar::storeAirSpaceHistory(History){
	 std::ofstream outfile;

	    // Open the file in append mode (optional), or trunc mode (default)
	    outfile.open(History);

	    // Check if the file is open
	    if (outfile.is_open()) {
	        // Write content to the file
	        outfile << Aircrafts << std::endl;

	        // Close the file after writing
	        outfile.close();
	        std::cout << "Data written to file successfully!" << std::endl;
	    } else {
	        std::cerr << "Error: Could not open file " << filename << std::endl;
	    }

}



