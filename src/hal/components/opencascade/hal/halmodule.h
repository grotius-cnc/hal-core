#ifndef SCURVE_H
#define SCURVE_H

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

// In the makefile the external so lib has to be present with -L/path -llibname
struct data {
    //! Machine model data:
    double J0x,J0y,J0z;
    double J1x,J1y,J1z;
    double J2x,J2y,J2z;
    double J3x,J3y,J3z;
    double J4x,J4y,J4z;
    double J5x,J5y,J5z;

    //! Machine rotation flags:
    int J0_rotx,J0_roty,J0_rotz;
    int J1_rotx,J1_roty,J1_rotz;
    int J2_rotx,J2_roty,J2_rotz;
    int J3_rotx,J3_roty,J3_rotz;
    int J4_rotx,J4_roty,J4_rotz;
    int J5_rotx,J5_roty,J5_rotz;

    //! Machine movement values:
    double J0,J1,J2,J3,J4,J5;
    double Cartx,Carty,Cartz;
    double Eulerx,Eulery,Eulerz;
};

extern void init_opencascade(char *stepfilepath, int display_hz);
extern void meat(struct data d);

#endif // SCURVE_H































