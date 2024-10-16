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


    Aircraft() = default;
    Aircraft(float t, int i, double xs, double ys, double zs, double xp, double yp, double zp);

    // Getters
       float getTime() const;
       int getId() const;
       double getXspeed() const;
       double getYspeed() const;
       double getZspeed() const;
       double getXpost() const;
       double getYpost() const;
       double getZpost() const;

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

void display(const Aircraft& aircraft);



