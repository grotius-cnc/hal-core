#ifndef NEXT_H
#define NEXT_H

#include "next_global.h"
#include <string>

#ifndef ULAPI
#define ULAPI
#endif

#define SUCCESS 1
#undef Success //https://eigen.tuxfamily.org/bz/show_bug.cgi?id=253

#include <chainiksolverpos_lma.hpp>
#include <chainfksolverpos_recursive.hpp>
#include <chainiksolvervel_pinv.hpp>
#include <chainiksolverpos_nr_jl.hpp>

#include <iostream>
#include <chrono>
#include <thread>

struct point {
    double x,y,z;
};

struct data {

    // Joint 0
    double J0_x=0;
    double J0_y=0;
    double J0_z=0;

    double J0_init=0;
    double J0_min=0;
    double J0_max=0;

    // Joint 1
    double J1_x=0;
    double J1_y=0;
    double J1_z=0;

    double J1_init=0;
    double J1_min=0;
    double J1_max=0;

    // Joint 2
    double J2_x=0;
    double J2_y=0;
    double J2_z=0;

    double J2_init=0;
    double J2_min=0;
    double J2_max=0;

    // Joint 3
    double J3_x=0;
    double J3_y=0;
    double J3_z=0;

    double J3_init=0;
    double J3_min=0;
    double J3_max=0;

    // Joint 4
    double J4_x=0;
    double J4_y=0;
    double J4_z=0;

    double J4_init=0;
    double J4_min=0;
    double J4_max=0;

    // Joint 4
    double J5_x=0;
    double J5_y=0;
    double J5_z=0;

    double J5_init=0;
    double J5_min=0;
    double J5_max=0;

    // joints
    double J0=0,J1=0,J2=0,J3=0,J4=0,J5=0;
    // cartesian
    double Cartx=0,Carty=0,Cartz=0;
    // eulers
    double Eulerx=0,Eulery=0,Eulerz=0;

    int Fk_mode=0;
    int Ik_mode=0;
    int Ik_from_init;
    double Iterations; // Ik iteration value ~100
    int error=0;
};

class NEXT_EXPORT Next{
public:

    Next();

    data init_wrapper(data d);
    data init(data d);

    data fk_wrapper(data d);
    data fk(data d);

    data ik_wrapper(data d);
    data ik(data d);



};

#endif // NEXT_H
