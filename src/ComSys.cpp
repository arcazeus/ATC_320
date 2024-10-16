#include <iostream>
#include "ComSys.h"
#include "Aircraft.h"
#include "Operator.h"
#include <vector>

using namespace std;

//ComSys::ComSys::(){
//
//}

//minimum Z=1000, X=3000, Y=3000
void ComSys::checkViolations(){
	int temp_X;
	int temp_Y;
	int temp_Z;

	//checks for any current violations
	for(int i=0;i<TotalNumAircraft;i++){
		for(int j =1;j<TotalNumAircraft;j++){
			temp_X=aircraft[i].getPositionX() - aircraft[j].getPositionX();
			temp_Y=aircraft[i].getPositionY() - aircraft[j].getPositionY();
			temp_Z=aircraft[i].getPositionZ() - aircraft[j].getPositionZ();

			if(temp_X<=3000 || temp_Y<=3000 || temp_Z<=1000){
				operatorAlert(i,j);
			}

			else{
				break;
			}
		}
	}

	// checks for future violations
	for(int i=0;i<TotalNumAircraft;i++){
			for(int j =1;j<TotalNumAircraft;j++){
				temp_X=aircraft[i].getPositionX()+n*aircraft[i].getSpeedX() - aircraft[j].getPositionX()+n*aircraft[j].getSpeedX();
				temp_Y=aircraft[i].getPositionY()+n*aircraft[i].getSpeedY() - aircraft[j].getPositionY()+n*aircraft[j].getSpeedY();
				temp_Z=aircraft[i].getPositionY()+n*aircraft[i].getSpeedY() - aircraft[j].getPositionY()+n*aircraft[j].getSpeedY();

				if(temp_X<=3000 || temp_Y<=3000 || temp_Z<=1000){
					operatorAlert(i,j);
				}

				else{
					break;
				}
			}
		}

}

void ComSys::operatorAlert(int id_1, int id_2){

	std::cout<<"There is a current\future violation with plane"<<id_1<<"and plane"<<id_2<<std::endl;

}

voidComSys::sendDataDisplay(int id){

std::cout<<aircraft[id].getID()<<""<<aircraft[id].getX()<<""<<aircraft[id].getY()<<""<<aircraft[id].getZ()<<std::endl;
}


