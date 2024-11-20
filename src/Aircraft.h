#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <iostream>
#include <vector>

using namespace std;

struct Aircraft {
	float time;
	int id;
	double xspeed;
	double yspeed;
	double zspeed;
	double xpost;
	double ypost;
	double zpost;

	static void* startAircraft(void* arg);

	Aircraft(float t, int i, double xs, double ys, double zs, double xp,
			double yp, double zp);
	void display(const Aircraft &aircraft);
	// Getters
	float getTime() const;
	int getId() const;
	double getSpeedX() const;
	double getSpeedY() const;
	double getSpeedZ() const;
	double getPositionX() const;
	double getPositionY() const;
	double getPositionZ() const;

	// Setters
	void setTime(float t);
	void setId(int i);
	void setXspeed(double xs);
	void setYspeed(double ys);
	void setZspeed(double zs);
	void setXpost(double xp);
	void setYpost(double yp);
	void setZpost(double zp);

};

#endif

