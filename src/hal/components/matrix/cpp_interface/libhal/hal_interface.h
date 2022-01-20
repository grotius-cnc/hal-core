#ifndef HAL_INTERFACE_H
#define HAL_INTERFACE_H

#ifndef ULAPI
#define ULAPI
#endif

#include "/opt/hal-core/src/hal/hal.h"
#include "/opt/hal-core/src/hal/hal_priv.h"
#include "iostream"



//! Hal float pin in-out
typedef struct {
    hal_float_t *Pin;
} float_data_t;

//! Hal bool pin in-out
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;

//! Int pin in-out
typedef struct {
    hal_s32_t *Pin;
} s32_data_t;

//! Unsigned int pin in-out
typedef struct {
    hal_u32_t *Pin;
} u32_data_t;

//! Char array port in-out
typedef struct {
    hal_port_t *Pin;
} port_data_t;

//! Parameter pin rw float
typedef struct {
    hal_float_t Pin;
} param_float_data_t;

//! Parameter pin rw bool
typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;

class hal_interface
{
public:



    hal_interface();
    ~hal_interface();
    void get_info();
    double print_servo_thread_time();
    double print_base_thread_time();
};

#endif // HAL_INTERFACE_H
