#ifndef HALMODULE_H
#define HALMODULE_H

#include "rtapi.h"          /* RTAPI realtime OS API */
#include "rtapi_app.h"		/* RTAPI realtime module decls */
#include "math.h"           /* Used for pow and sqrt */

#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

// Example function:
void myfirstvoid(float a, float b, float *result);

void myfirstvoid(float a, float b, float *result){
    *result=a+b;
}

// https://www.thegeekstuff.com/2013/01/mix-c-and-cpp/
// In the makefile the external so lib has to be present with -L/path -llibname

struct point {
    double x,y,z;
};

struct data {

    // Joint 0
    double J0_x;
    double J0_y;
    double J0_z;

    double J0_init;
    double J0_min;
    double J0_max;

    // Joint 1
    double J1_x;
    double J1_y;
    double J1_z;

    double J1_init;
    double J1_min;
    double J1_max;

    // Joint 2
    double J2_x;
    double J2_y;
    double J2_z;

    double J2_init;
    double J2_min;
    double J2_max;

    // Joint 3
    double J3_x;
    double J3_y;
    double J3_z;

    double J3_init;
    double J3_min;
    double J3_max;

    // Joint 4
    double J4_x;
    double J4_y;
    double J4_z;

    double J4_init;
    double J4_min;
    double J4_max;

    // Joint 4
    double J5_x;
    double J5_y;
    double J5_z;

    double J5_init;
    double J5_min;
    double J5_max;

    // joints
    double J0,J1,J2,J3,J4,J5;
    // cartesian
    double Cartx,Carty,Cartz;
    // eulers
    double Eulerx,Eulery,Eulerz;

    int Fk_mode;
    int Ik_mode;
    int Ik_from_init;
    double Iterations; // Ik iteration value ~100
    int error;
};

extern struct data init_wrapper(struct data d);
extern struct data fk_wrapper(struct data d);
extern struct data ik_wrapper(struct data d);

#endif // HALMODULE_H































