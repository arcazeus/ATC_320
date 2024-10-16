#include "Aircraft.h"
#include <vector>


// Would need to be able to store aircrafts and their details
// vector for size change at run time


Aircraft::Aircraft(float t, int i, double fl, double xs, double ys, double zs, double xp, double yp, double zp)
    : time(t), id(i), xspeed(xs), yspeed(ys), zspeed(zs), xpost(xp), ypost(yp), zpost(zp) {}

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
