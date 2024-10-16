#ifndef AIRCRAFT_H
#define AIRCRAFT_H

<<<<<<< HEAD

#include <iostream>
#include <vector>

using namespace std;

struct Aircraft {
    float time;
    int id;
<<<<<<< HEAD
    double flevel;
=======
struct Aircraft {
    float time;
    int id;
>>>>>>> origin/Thomas
=======
>>>>>>> 12d2607e2aa5658e69c1cf2297cd7fd5e61e7361
    double xspeed;
    double yspeed;
    double zspeed;
    double xpost;
    double ypost;
    double zpost;


    Aircraft() = default;

<<<<<<< HEAD
<<<<<<< HEAD
    Aircraft(float t, int i, double fl, double xs, double ys, double zs, double xp, double yp, double zp);
=======
    Aircraft(float t, int i, double xs, double ys, double zs, double xp, double yp, double zp);


>>>>>>> origin/Thomas
=======
    Aircraft(float t, int i, double xs, double ys, double zs, double xp, double yp, double zp);
>>>>>>> 12d2607e2aa5658e69c1cf2297cd7fd5e61e7361
};

void display(const Aircraft& aircraft);



