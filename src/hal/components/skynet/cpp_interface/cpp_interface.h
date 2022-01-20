#ifndef CPP_INTERFACE_H
#define CPP_INTERFACE_H

//! Author : Skynet
//! Optimalized kinematic library implementation
//!  - Dynamic kinematic setup.
//!  - Setup multiple independent kinematic chains, strains.

#include "cpp_interface_global.h"
#include <string>
#include <iostream>
#include <thread>
#include "external_var.h"

#ifndef ULAPI
#define ULAPI
#endif

//! KDL kinematics lib
#define SUCCESS 1
#undef Success //https://eigen.tuxfamily.org/bz/show_bug.cgi?id=253
#include <chainiksolverpos_lma.hpp>
#include <chainfksolverpos_recursive.hpp>
#include <chainiksolvervel_pinv.hpp>
#include <chainiksolverpos_nr_jl.hpp>

//! LibRuckig scurve lib
//! https://github.com/pantor/ruckig, extra info: https://github.com/pantor/ruckig/issues/64
#include "ruckig/ruckig.hpp"

#include "mainwindow.h"

class CPP_INTERFACE_EXPORT Cpp_interface{
public:
    Cpp_interface();
    void start_gui();
    void gui_thread();

    MACHINE rtruckig(MACHINE *ptr);
    MACHINE forward_kinematic(MACHINE *ptr);
    MACHINE forward_kinematic_tooldir(MACHINE *ptr);
    MACHINE inverse_kinematic(MACHINE *ptr);
private:
    std::thread *Thread;
};

#endif // CPP_INTERFACE_H
