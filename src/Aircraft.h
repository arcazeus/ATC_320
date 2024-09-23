#include <iostream>
#include <vector>

class Aircraft
{
public:
	Aircraft(); // default constructor
	Aircraft(float, int, double, double, double); //Constructor
	Aircraft(const Aircraft& copy); // Copy constructor


	//Setters & Getters
	void setTime(float time);
	float getTime();
	void setID(int id);
	int getID();
	void setFL(double flevel);
	double getFL();
	void setXSpeed(double xspeed);
	double getXSpeed();
	void setYSpeed(double yspeed);
	double getYSpeed();
	void setZSpeed(double zspeed);
	double getZSpeed();
	void setXPosition(double xpost);
	double getXPosition();
	void setYPosition(double ypost);
	double getYPosition();
	void setZPosition(double zpost);
	double getZPosition();
	void display(); // will send data when signal is detected\

private:
	float time;
	int id;
	double flevel;
	double xspeed;
	double yspeed;
	double zspeed;
	double xpost;
	double ypost;
	double zpost;



};
