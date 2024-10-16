#include "Aircraft.h"


// Would need to be able to store aircrafts and their details
// vector for size change at run time


Aircraft::Aircraft(float t, int i, double xs, double ys, double zs, double xp, double yp, double zp)
    : time(t), id(i), xspeed(xs), yspeed(ys), zspeed(zs), xpost(xp), ypost(yp), zpost(zp) {}


// Getters
float Aircraft::getTime() const { return time; }
int Aircraft::getId() const { return id; }
double Aircraft::getXspeed() const { return xspeed; }
double Aircraft::getYspeed() const { return yspeed; }
double Aircraft::getZspeed() const { return zspeed; }
double Aircraft::getXpost() const { return xpost; }
double Aircraft::getYpost() const { return ypost; }
double Aircraft::getZpost() const { return zpost; }

// Setters
void Aircraft::setTime(float t) { time = t; }
void Aircraft::setId(int i) { id = i; }
void Aircraft::setXspeed(double xs) { xspeed = xs; }
void Aircraft::setYspeed(double ys) { yspeed = ys; }
void Aircraft::setZspeed(double zs) { zspeed = zs; }
void Aircraft::setXpost(double xp) { xpost = xp; }
void Aircraft::setYpost(double yp) { ypost = yp; }
void Aircraft::setZpost(double zp) { zpost = zp; }


void display(const Aircraft& aircraft) {
    std::cout << "Aircraft ID: " << aircraft.id << "\n"
              << "Time: " << aircraft.time << "\n"
              << "X Speed: " << aircraft.xspeed << "\n"
              << "Y Speed: " << aircraft.yspeed << "\n"
              << "Z Speed: " << aircraft.zspeed << "\n"
              << "X Position: " << aircraft.xpost << "\n"
              << "Y Position: " << aircraft.ypost << "\n"
              << "Z Position: " << aircraft.zpost << "\n";
}
