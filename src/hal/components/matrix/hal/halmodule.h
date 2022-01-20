#ifndef HALMODULE_H
#define HALMODULE_H

#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "hal_priv.h"
#include "rtapi_math64.h"
#include "rtapi_math.h"
#include <stdio.h>
#include <stdlib.h>

#include "/opt/hal-core/src/hal/components/matrix/cpp_interface/external_var.h"

#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

//! Start the c++ app.
extern void start_gui(struct DATA *p);
extern void c_to_cpp_ptr(struct DATA *p);

//! KDL kinematics forward kinmematics "fk", inverse kinematics "ik"
extern struct DATA fk(struct DATA *ptr);
extern struct DATA ik(struct DATA *ptr);

//! Gcode runner "gr"
extern struct DATA gr(struct DATA *ptr);

//! Manual runner "mr"
extern struct DATA mr(struct DATA *ptr);

// Example function:
void myfirstvoid(float a, float b, float *result);

void myfirstvoid(float a, float b, float *result){
    *result=a+b;
}

#endif // HALMODULE_H































