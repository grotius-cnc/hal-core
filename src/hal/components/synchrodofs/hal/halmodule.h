#ifndef HALMODULE_H
#define HALMODULE_H

#include <stdio.h>
#include <stdbool.h>

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
    enum interface interfacetype;
    enum synchronization synchronizationtype;
    enum durationdiscretization durationdiscretizationtype;
};

extern struct result wrapper_get_pos_array(struct result input_array[]);

#endif // HALMODULE_H
