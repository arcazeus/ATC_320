#ifndef AIRCRAFT_H
#define AIRCRAFT_H

struct Aircraft {
    float time;
    int id;
    double xspeed;
    double yspeed;
    double zspeed;
    double xpost;
    double ypost;
    double zpost;


    Aircraft() = default;

    Aircraft(float t, int i, double xs, double ys, double zs, double xp, double yp, double zp);


};

void display(const Aircraft& aircraft);


#endif // AIRCRAFT_LIST_H
