#ifndef CPLUSPLUS_H
#define CPLUSPLUS_H

#include "cplusplus_global.h"
#include <string>

#include <iostream>
#include <chrono>
#include <thread>

struct data {
    //! Machine model data:
    double J0x=0,J0y=0,J0z=0;
    double J1x=0,J1y=0,J1z=0;
    double J2x=0,J2y=0,J2z=0;
    double J3x=0,J3y=0,J3z=0;
    double J4x=0,J4y=0,J4z=0;
    double J5x=0,J5y=0,J5z=0;

    //! Machine rotation flags:
    int J0_rotx=0,J0_roty=0,J0_rotz=0;
    int J1_rotx=0,J1_roty=0,J1_rotz=0;
    int J2_rotx=0,J2_roty=0,J2_rotz=0;
    int J3_rotx=0,J3_roty=0,J3_rotz=0;
    int J4_rotx=0,J4_roty=0,J4_rotz=0;
    int J5_rotx=0,J5_roty=0,J5_rotz=0;

    //! Machine movement values, J0-J5 and Eulers in radians.
    double J0=0,J1=0,J2=0,J3=0,J4=0,J5=0;
    double Cartx=0,Carty=0,Cartz=0;
    double Eulerx=0,Eulery=0,Eulerz=0;
};
// Example of extern variable:
extern data databucket;
extern char *stepfilepath_evar;
extern int display_hz_evar;
extern char gcode_filename[150];

class NEXT_EXPORT Cplusplus{
public:

    Cplusplus();

    void init_opencascade(char *stepfilepath);
    void start();
    void gui();

    void meat(data d);
    void meat_cplusplus(data d);

private:
    std::thread* Thread;
};

#endif // CPLUSPLUS_H
