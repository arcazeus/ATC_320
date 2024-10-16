#ifndef AIRCRAFT_H
#define AIRCRAFT_H

<<<<<<< HEAD

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <vector>

struct Aircraft {
    float time;
    int id;
    double flevel;
=======
struct Aircraft {
    float time;
    int id;
>>>>>>> origin/Thomas
    double xspeed;
    double yspeed;
    double zspeed;
    double xpost;
    double ypost;
    double zpost;


    Aircraft() = default;

<<<<<<< HEAD
    Aircraft(float t, int i, double fl, double xs, double ys, double zs, double xp, double yp, double zp);
=======
    Aircraft(float t, int i, double xs, double ys, double zs, double xp, double yp, double zp);


>>>>>>> origin/Thomas
};

void display(const Aircraft& aircraft);


#endif // AIRCRAFT_LIST_H
