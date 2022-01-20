#ifndef KINEMATIC_INTERFACE_H
#define KINEMATIC_INTERFACE_H

//! KDL kinematics lib
#define SUCCESS 1
#undef Success //https://eigen.tuxfamily.org/bz/show_bug.cgi?id=253
#include <chainiksolverpos_lma.hpp>
#include <chainfksolverpos_recursive.hpp>
#include <chainiksolvervel_pinv.hpp>
#include <chainiksolverpos_nr_jl.hpp>

//! libocc
#include <libocct/opencascade.h>

#include "external_var.h"
#include "iostream"

class kinematic_interface
{
public:
    kinematic_interface();

    DATA fk(DATA *ptr);
    DATA ik(DATA *ptr);
};

#endif // KINEMATIC_INTERFACE_H
