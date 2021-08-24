#ifndef CPP_INTERFACE_H
#define CPP_INTERFACE_H

#include "cpp_interface_global.h"
#include <iostream>
#include <vector>
#include <fstream>

//! https://github.com/pantor/ruckig
#include "libruckig/ruckig.hpp"

/*
int i=1;
ruckig::Ruckig<ruckig::DynamicDOFs> otg {static_cast<size_t>(i), 0.001};
ruckig::InputParameter<ruckig::DynamicDOFs> in {static_cast<size_t>(i)};
ruckig::OutputParameter<ruckig::DynamicDOFs> out {static_cast<size_t>(i)};
std::array<double, 3> pos, vel, acc;
*/


ruckig::Ruckig<6> otg {0.001};
ruckig::InputParameter<6> in;
ruckig::OutputParameter<6> out;
std::array<double, 6> vel, acc, pos;

enum interface {
    position,
    velocity
};

enum synchronization {
    Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
    Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
    TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
    None, ///< Calculate every DoF independently
};

enum durationdiscretization {
    Continuous, ///< Every trajectory duration is allowed (Default)
    Discrete, ///< The trajectory duration must be a multiple of the control cycle
};

struct result {
    unsigned int size;
    double curvel,curacc,curpos,tarpos;
    double maxvel,maxacc,maxjerk;
    bool enable;
    interface interfacetype;
    synchronization synchronizationtype;
    durationdiscretization durationdiscretizationtype;
};

class CPP_INTERFACE_EXPORT Cpp_interface
{
public:

public:
    Cpp_interface();
    result* dofs(result input[]);
private:

};

#endif // CPP_INTERFACE_H
