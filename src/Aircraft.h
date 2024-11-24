#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <iostream>
#include <vector>
#include <sys/neutrino.h>
#include <sys/dispatch.h>

using namespace std;

class Aircraft {
public:
    // Constructor matching the parameters in main.cpp
    Aircraft(int id, double xpost, double ypost, double zpost, double xspeed, double yspeed, double zspeed, float time);

    static void* startAircraft(void* arg);
    void runAircraft();
    void updatePosition();
    void executeCommand(const std::string& command);
    void checkForMessages(name_attach_t* attach);
	void handleMessage(int rcvid, const char *msg);

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
    void setTime(float time);
    void setId(int id);
    void setXspeed(double xs);
    void setYspeed(double ys);
    void setZspeed(double zs);
    void setXpost(double xp);
    void setYpost(double yp);
    void setZpost(double zp);



private:

    float time;
    int id;
    double xspeed;
    double yspeed;
    double zspeed;
    double xpost;
    double ypost;
    double zpost;

};

#endif

