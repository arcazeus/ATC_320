

#include <iostream>
#include <vector>

using namespace std;

struct Aircraft {
    float time;
    int id;
    double flevel;
    double xspeed;
    double yspeed;
    double zspeed;
    double xpost;
    double ypost;
    double zpost;


    Aircraft() = default;

    Aircraft(float t, int i, double fl, double xs, double ys, double zs, double xp, double yp, double zp);
};

void display(const Aircraft& aircraft);



