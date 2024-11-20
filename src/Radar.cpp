#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include "Radar.h"
#include "ComSys.h"
#include "Aircraft.h"

Radar::Radar(){



}

	void* Radar::startRadar(void* arg){
		((Radar*) arg)->runRadar();
			return NULL;



}

void Radar::runRadar(){
	std::cout<<"Radar running..."<<std::endl;
	while(1){

		this->scanForAircraft();
			this->storeAirSpaceHistory();
	}
}

void Radar::addAircraft(){

}

void Radar::scanForAircraft(){
	std::cout<<"Scanning...\n";

	for(int i =0 ; i<120; i++){

	}

}

void Radar::storeAirSpaceHistory(){
	 std::ofstream outfile;

	    // Open the file in append mode (optional), or trunc mode (default)
	    outfile.open(History);

	    // Check if the file is open
	    if (outfile.is_open()) {
	        // Write content to the file
	      //  outfile << Aircrafts << std::endl;

	        // Close the file after writing
	        outfile.close();
	        std::cout << "Data written to file successfully!" << std::endl;
	    } else {
	        std::cerr << "Error: Could not open file " << History << std::endl;
	    }

}



