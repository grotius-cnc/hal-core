#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "rtapi_math64.h"

static int comp_id;

#ifdef MODULE_INFO
MODULE_INFO(linuxcnc, "component:kinematic:kinematic component");
MODULE_INFO(linuxcnc, "descr:\nkinematic component\n\nCompile slogan :\n/opt/linuxcnc/bin/halcompile --compile /opt/linuxcnc/src/hal/components/kinematic.comp\n\nPreprocess slogan :\n/opt/linuxcnc/bin/halcompile --preprocess /opt/linuxcnc/src/hal/components/kinematic.comp\n\n/opt/linuxcnc/bin/halcompile --preprocess /home/user/Desktop/Linux-Embedded/linux-hal/halcompile-kinematic/kinematic.comp\n\n\nThen copy it to the rtlib.\n\nThen in halview : loadrt kinematic\n\nUsage :\n\n");
MODULE_INFO(linuxcnc, "author:Skynet 2021");
MODULE_INFO(linuxcnc, "license:GPLv2 or greater");
MODULE_INFO(linuxcnc, "pin:init:bit:0:out:init values at component startup:None:None");
MODULE_INFO(linuxcnc, "param:j0_x:float:0:rw:joint 0, x_input:None:None");
MODULE_INFO(linuxcnc, "param:j0_y:float:0:rw:joint 0, y_input:None:None");
MODULE_INFO(linuxcnc, "param:j0_z:float:0:rw:joint 0, z_input:None:None");
MODULE_INFO(linuxcnc, "param:j0_init:float:0:rw:joint 0, initial degrees:None:None");
MODULE_INFO(linuxcnc, "param:j0_min:float:0:rw:joint 0, min degrees:None:None");
MODULE_INFO(linuxcnc, "param:j0_max:float:0:rw:joint 0, max degrees:None:None");
MODULE_INFO(linuxcnc, "param:j1_x:float:0:rw:joint 1, x_input:None:None");
MODULE_INFO(linuxcnc, "param:j1_y:float:0:rw:joint 1, y_input:None:None");
MODULE_INFO(linuxcnc, "param:j1_z:float:0:rw:joint 1, z_input:None:None");
MODULE_INFO(linuxcnc, "param:j1_init:float:0:rw:joint 1, initial degrees:None:None");
MODULE_INFO(linuxcnc, "param:j1_min:float:0:rw:joint 1, min degrees:None:None");
MODULE_INFO(linuxcnc, "param:j1_max:float:0:rw:joint 1, max degrees:None:None");
MODULE_INFO(linuxcnc, "param:j2_x:float:0:rw:joint 2, x_input:None:None");
MODULE_INFO(linuxcnc, "param:j2_y:float:0:rw:joint 2, y_input:None:None");
MODULE_INFO(linuxcnc, "param:j2_z:float:0:rw:joint 2, z_input:None:None");
MODULE_INFO(linuxcnc, "param:j2_init:float:0:rw:joint 2, initial degrees:None:None");
MODULE_INFO(linuxcnc, "param:j2_min:float:0:rw:joint 2, min degrees:None:None");
MODULE_INFO(linuxcnc, "param:j2_max:float:0:rw:joint 2, max degrees:None:None");
MODULE_INFO(linuxcnc, "param:j3_x:float:0:rw:joint 3, x_input:None:None");
MODULE_INFO(linuxcnc, "param:j3_y:float:0:rw:joint 3, y_input:None:None");
MODULE_INFO(linuxcnc, "param:j3_z:float:0:rw:joint 3, z_input:None:None");
MODULE_INFO(linuxcnc, "param:j3_init:float:0:rw:joint 3, initial degrees:None:None");
MODULE_INFO(linuxcnc, "param:j3_min:float:0:rw:joint 3, min degrees:None:None");
MODULE_INFO(linuxcnc, "param:j3_max:float:0:rw:joint 3, max degrees:None:None");
MODULE_INFO(linuxcnc, "param:j4_x:float:0:rw:joint 4, x_input:None:None");
MODULE_INFO(linuxcnc, "param:j4_y:float:0:rw:joint 4, y_input:None:None");
MODULE_INFO(linuxcnc, "param:j4_z:float:0:rw:joint 4, z_input:None:None");
MODULE_INFO(linuxcnc, "param:j4_init:float:0:rw:joint 4, initial degrees:None:None");
MODULE_INFO(linuxcnc, "param:j4_min:float:0:rw:joint 4, min degrees:None:None");
MODULE_INFO(linuxcnc, "param:j4_max:float:0:rw:joint 4, max degrees:None:None");
MODULE_INFO(linuxcnc, "param:j5_x:float:0:rw:joint 5, x_input:None:None");
MODULE_INFO(linuxcnc, "param:j5_y:float:0:rw:joint 5, y_input:None:None");
MODULE_INFO(linuxcnc, "param:j5_z:float:0:rw:joint 5, z_input:None:None");
MODULE_INFO(linuxcnc, "param:j5_init:float:0:rw:joint 5, initial degrees:None:None");
MODULE_INFO(linuxcnc, "param:j5_min:float:0:rw:joint 5, min degrees:None:None");
MODULE_INFO(linuxcnc, "param:j5_max:float:0:rw:joint 5, max degrees:None:None");
MODULE_INFO(linuxcnc, "param:j0_rw:float:0:rw:joint 0 position:None:None");
MODULE_INFO(linuxcnc, "param:j1_rw:float:0:rw:joint 1 position:None:None");
MODULE_INFO(linuxcnc, "param:j2_rw:float:0:rw:joint 2 position:None:None");
MODULE_INFO(linuxcnc, "param:j3_rw:float:0:rw:joint 3 position:None:None");
MODULE_INFO(linuxcnc, "param:j4_rw:float:0:rw:joint 4 position:None:None");
MODULE_INFO(linuxcnc, "param:j5_rw:float:0:rw:joint 5 position:None:None");
MODULE_INFO(linuxcnc, "param:cart_x:float:0:rw:machine cartesian x:None:None");
MODULE_INFO(linuxcnc, "param:cart_y:float:0:rw:machine cartesian y:None:None");
MODULE_INFO(linuxcnc, "param:cart_z:float:0:rw:machine cartesian z:None:None");
MODULE_INFO(linuxcnc, "param:euler_x:float:0:rw:machine euler angle x:None:None");
MODULE_INFO(linuxcnc, "param:euler_y:float:0:rw:machine euler angle y:None:None");
MODULE_INFO(linuxcnc, "param:euler_z:float:0:rw:machine euler angle z:None:None");
MODULE_INFO(linuxcnc, "pin:perform_fk:bit:0:in:calculate the forward kinematics from the joint input values:None:None");
MODULE_INFO(linuxcnc, "pin:perform_ik:bit:0:in:calculate the inverse kinematics from the xyz and euler input values:None:None");
MODULE_INFO(linuxcnc, "pin:ik_from_init:bit:0:in:perform a inverse kinematics from init position, else perform it from current position:None:None");
MODULE_INFO(linuxcnc, "pin:iterations:float:0:in:set the ammount of iterations for the inverse kinematic calculations. Normal value=100:None:None");
MODULE_INFO(linuxcnc, "pin:debug:bit:0:in:print results:None:None");
MODULE_INFO(linuxcnc, "pin:pin_j0:float:0:out:pin out j0:None:None");
MODULE_INFO(linuxcnc, "pin:pin_j1:float:0:out:pin out j1:None:None");
MODULE_INFO(linuxcnc, "pin:pin_j2:float:0:out:pin out j2:None:None");
MODULE_INFO(linuxcnc, "pin:pin_j3:float:0:out:pin out j3:None:None");
MODULE_INFO(linuxcnc, "pin:pin_j4:float:0:out:pin out j4:None:None");
MODULE_INFO(linuxcnc, "pin:pin_j5:float:0:out:pin out j5:None:None");
MODULE_INFO(linuxcnc, "pin:pin_cartx:float:0:out:pin out cartx:None:None");
MODULE_INFO(linuxcnc, "pin:pin_carty:float:0:out:pin out carty:None:None");
MODULE_INFO(linuxcnc, "pin:pin_cartz:float:0:out:pin out cartz:None:None");
MODULE_INFO(linuxcnc, "pin:pin_eulerx:float:0:out:pin out eulerx:None:None");
MODULE_INFO(linuxcnc, "pin:pin_eulery:float:0:out:pin out eulery:None:None");
MODULE_INFO(linuxcnc, "pin:pin_eulerz:float:0:out:pin out eulerz:None:None");
MODULE_INFO(linuxcnc, "pin:pin_cartx_in:float:0:in:test input from axis_mm gui:None:None");
MODULE_INFO(linuxcnc, "pin:pin_carty_in:float:0:in:test input from axis_mm gui:None:None");
MODULE_INFO(linuxcnc, "pin:pin_cartz_in:float:0:in:test input from axis_mm gui:None:None");
MODULE_INFO(linuxcnc, "funct:_:1:");
MODULE_LICENSE("GPLv2 or greater");
#endif // MODULE_INFO


struct __comp_state {
    struct __comp_state *_next;
    hal_bit_t *init;
    hal_bit_t *perform_fk;
    hal_bit_t *perform_ik;
    hal_bit_t *ik_from_init;
    hal_float_t *iterations;
    hal_bit_t *debug;
    hal_float_t *pin_j0;
    hal_float_t *pin_j1;
    hal_float_t *pin_j2;
    hal_float_t *pin_j3;
    hal_float_t *pin_j4;
    hal_float_t *pin_j5;
    hal_float_t *pin_cartx;
    hal_float_t *pin_carty;
    hal_float_t *pin_cartz;
    hal_float_t *pin_eulerx;
    hal_float_t *pin_eulery;
    hal_float_t *pin_eulerz;
    hal_float_t *pin_cartx_in;
    hal_float_t *pin_carty_in;
    hal_float_t *pin_cartz_in;
    hal_float_t j0_x;
    hal_float_t j0_y;
    hal_float_t j0_z;
    hal_float_t j0_init;
    hal_float_t j0_min;
    hal_float_t j0_max;
    hal_float_t j1_x;
    hal_float_t j1_y;
    hal_float_t j1_z;
    hal_float_t j1_init;
    hal_float_t j1_min;
    hal_float_t j1_max;
    hal_float_t j2_x;
    hal_float_t j2_y;
    hal_float_t j2_z;
    hal_float_t j2_init;
    hal_float_t j2_min;
    hal_float_t j2_max;
    hal_float_t j3_x;
    hal_float_t j3_y;
    hal_float_t j3_z;
    hal_float_t j3_init;
    hal_float_t j3_min;
    hal_float_t j3_max;
    hal_float_t j4_x;
    hal_float_t j4_y;
    hal_float_t j4_z;
    hal_float_t j4_init;
    hal_float_t j4_min;
    hal_float_t j4_max;
    hal_float_t j5_x;
    hal_float_t j5_y;
    hal_float_t j5_z;
    hal_float_t j5_init;
    hal_float_t j5_min;
    hal_float_t j5_max;
    hal_float_t j0_rw;
    hal_float_t j1_rw;
    hal_float_t j2_rw;
    hal_float_t j3_rw;
    hal_float_t j4_rw;
    hal_float_t j5_rw;
    hal_float_t cart_x;
    hal_float_t cart_y;
    hal_float_t cart_z;
    hal_float_t euler_x;
    hal_float_t euler_y;
    hal_float_t euler_z;
    char text;

};
struct __comp_state *__comp_first_inst=0, *__comp_last_inst=0;

static void _(struct __comp_state *__comp_inst, long period);
static int __comp_get_data_size(void);
#undef TRUE
#define TRUE (1)
#undef FALSE
#define FALSE (0)
#undef true
#define true (1)
#undef false
#define false (0)

static int export(char *prefix, long extra_arg) {
    char buf[HAL_NAME_LEN + 1];
    int r = 0;
    int sz = sizeof(struct __comp_state) + __comp_get_data_size();
    struct __comp_state *inst = hal_malloc(sz);
    memset(inst, 0, sz);
    r = hal_pin_bit_newf(HAL_OUT, &(inst->init), comp_id,
        "%s.init", prefix);
    if(r != 0) return r;
    r = hal_pin_bit_newf(HAL_IN, &(inst->perform_fk), comp_id,
        "%s.perform-fk", prefix);
    if(r != 0) return r;
    r = hal_pin_bit_newf(HAL_IN, &(inst->perform_ik), comp_id,
        "%s.perform-ik", prefix);
    if(r != 0) return r;
    r = hal_pin_bit_newf(HAL_IN, &(inst->ik_from_init), comp_id,
        "%s.ik-from-init", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_IN, &(inst->iterations), comp_id,
        "%s.iterations", prefix);
    if(r != 0) return r;
    r = hal_pin_bit_newf(HAL_IN, &(inst->debug), comp_id,
        "%s.debug", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_j0), comp_id,
        "%s.pin-j0", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_j1), comp_id,
        "%s.pin-j1", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_j2), comp_id,
        "%s.pin-j2", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_j3), comp_id,
        "%s.pin-j3", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_j4), comp_id,
        "%s.pin-j4", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_j5), comp_id,
        "%s.pin-j5", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_cartx), comp_id,
        "%s.pin-cartx", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_carty), comp_id,
        "%s.pin-carty", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_cartz), comp_id,
        "%s.pin-cartz", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_eulerx), comp_id,
        "%s.pin-eulerx", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_eulery), comp_id,
        "%s.pin-eulery", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_OUT, &(inst->pin_eulerz), comp_id,
        "%s.pin-eulerz", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_IN, &(inst->pin_cartx_in), comp_id,
        "%s.pin-cartx-in", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_IN, &(inst->pin_carty_in), comp_id,
        "%s.pin-carty-in", prefix);
    if(r != 0) return r;
    r = hal_pin_float_newf(HAL_IN, &(inst->pin_cartz_in), comp_id,
        "%s.pin-cartz-in", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j0_x), comp_id,
        "%s.j0-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j0_y), comp_id,
        "%s.j0-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j0_z), comp_id,
        "%s.j0-z", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j0_init), comp_id,
        "%s.j0-init", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j0_min), comp_id,
        "%s.j0-min", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j0_max), comp_id,
        "%s.j0-max", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j1_x), comp_id,
        "%s.j1-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j1_y), comp_id,
        "%s.j1-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j1_z), comp_id,
        "%s.j1-z", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j1_init), comp_id,
        "%s.j1-init", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j1_min), comp_id,
        "%s.j1-min", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j1_max), comp_id,
        "%s.j1-max", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j2_x), comp_id,
        "%s.j2-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j2_y), comp_id,
        "%s.j2-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j2_z), comp_id,
        "%s.j2-z", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j2_init), comp_id,
        "%s.j2-init", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j2_min), comp_id,
        "%s.j2-min", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j2_max), comp_id,
        "%s.j2-max", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j3_x), comp_id,
        "%s.j3-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j3_y), comp_id,
        "%s.j3-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j3_z), comp_id,
        "%s.j3-z", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j3_init), comp_id,
        "%s.j3-init", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j3_min), comp_id,
        "%s.j3-min", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j3_max), comp_id,
        "%s.j3-max", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j4_x), comp_id,
        "%s.j4-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j4_y), comp_id,
        "%s.j4-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j4_z), comp_id,
        "%s.j4-z", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j4_init), comp_id,
        "%s.j4-init", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j4_min), comp_id,
        "%s.j4-min", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j4_max), comp_id,
        "%s.j4-max", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j5_x), comp_id,
        "%s.j5-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j5_y), comp_id,
        "%s.j5-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j5_z), comp_id,
        "%s.j5-z", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j5_init), comp_id,
        "%s.j5-init", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j5_min), comp_id,
        "%s.j5-min", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j5_max), comp_id,
        "%s.j5-max", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j0_rw), comp_id,
        "%s.j0-rw", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j1_rw), comp_id,
        "%s.j1-rw", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j2_rw), comp_id,
        "%s.j2-rw", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j3_rw), comp_id,
        "%s.j3-rw", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j4_rw), comp_id,
        "%s.j4-rw", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->j5_rw), comp_id,
        "%s.j5-rw", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->cart_x), comp_id,
        "%s.cart-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->cart_y), comp_id,
        "%s.cart-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->cart_z), comp_id,
        "%s.cart-z", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->euler_x), comp_id,
        "%s.euler-x", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->euler_y), comp_id,
        "%s.euler-y", prefix);
    if(r != 0) return r;
    r = hal_param_float_newf(HAL_RW, &(inst->euler_z), comp_id,
        "%s.euler-z", prefix);
    if(r != 0) return r;
    rtapi_snprintf(buf, sizeof(buf), "%s", prefix);
    r = hal_export_funct(buf, (void(*)(void *inst, long))_, inst, 1, 0, comp_id);
    if(r != 0) return r;
    if(__comp_last_inst) __comp_last_inst->_next = inst;
    __comp_last_inst = inst;
    if(!__comp_first_inst) __comp_first_inst = inst;
    return 0;
}
int rtapi_app_main(void) {
    int r = 0;
    comp_id = hal_init("hal_kinematic");
    if(comp_id < 0) return comp_id;
    r = export("hal_kinematic", 0);
    if(r) {
        hal_exit(comp_id);
    } else {
        hal_ready(comp_id);
    }
    return r;
}

void rtapi_app_exit(void) {
    hal_exit(comp_id);
}

#undef FUNCTION
#define FUNCTION(name) static void name(struct __comp_state *__comp_inst, long period)
#undef EXTRA_SETUP
#define EXTRA_SETUP() static int extra_setup(struct __comp_state *__comp_inst, char *prefix, long extra_arg)
#undef EXTRA_CLEANUP
#define EXTRA_CLEANUP() static void extra_cleanup(void)
#undef fperiod
#define fperiod (period * 1e-9)
#undef init
#define init (*__comp_inst->init)
#undef perform_fk
#define perform_fk (0+*__comp_inst->perform_fk)
#undef perform_ik
#define perform_ik (0+*__comp_inst->perform_ik)
#undef ik_from_init
#define ik_from_init (0+*__comp_inst->ik_from_init)
#undef iterations
#define iterations (0+*__comp_inst->iterations)
#undef debug
#define debug (0+*__comp_inst->debug)
#undef pin_j0
#define pin_j0 (*__comp_inst->pin_j0)
#undef pin_j1
#define pin_j1 (*__comp_inst->pin_j1)
#undef pin_j2
#define pin_j2 (*__comp_inst->pin_j2)
#undef pin_j3
#define pin_j3 (*__comp_inst->pin_j3)
#undef pin_j4
#define pin_j4 (*__comp_inst->pin_j4)
#undef pin_j5
#define pin_j5 (*__comp_inst->pin_j5)
#undef pin_cartx
#define pin_cartx (*__comp_inst->pin_cartx)
#undef pin_carty
#define pin_carty (*__comp_inst->pin_carty)
#undef pin_cartz
#define pin_cartz (*__comp_inst->pin_cartz)
#undef pin_eulerx
#define pin_eulerx (*__comp_inst->pin_eulerx)
#undef pin_eulery
#define pin_eulery (*__comp_inst->pin_eulery)
#undef pin_eulerz
#define pin_eulerz (*__comp_inst->pin_eulerz)
#undef pin_cartx_in
#define pin_cartx_in (0+*__comp_inst->pin_cartx_in)
#undef pin_carty_in
#define pin_carty_in (0+*__comp_inst->pin_carty_in)
#undef pin_cartz_in
#define pin_cartz_in (0+*__comp_inst->pin_cartz_in)
#undef j0_x
#define j0_x (__comp_inst->j0_x)
#undef j0_y
#define j0_y (__comp_inst->j0_y)
#undef j0_z
#define j0_z (__comp_inst->j0_z)
#undef j0_init
#define j0_init (__comp_inst->j0_init)
#undef j0_min
#define j0_min (__comp_inst->j0_min)
#undef j0_max
#define j0_max (__comp_inst->j0_max)
#undef j1_x
#define j1_x (__comp_inst->j1_x)
#undef j1_y
#define j1_y (__comp_inst->j1_y)
#undef j1_z
#define j1_z (__comp_inst->j1_z)
#undef j1_init
#define j1_init (__comp_inst->j1_init)
#undef j1_min
#define j1_min (__comp_inst->j1_min)
#undef j1_max
#define j1_max (__comp_inst->j1_max)
#undef j2_x
#define j2_x (__comp_inst->j2_x)
#undef j2_y
#define j2_y (__comp_inst->j2_y)
#undef j2_z
#define j2_z (__comp_inst->j2_z)
#undef j2_init
#define j2_init (__comp_inst->j2_init)
#undef j2_min
#define j2_min (__comp_inst->j2_min)
#undef j2_max
#define j2_max (__comp_inst->j2_max)
#undef j3_x
#define j3_x (__comp_inst->j3_x)
#undef j3_y
#define j3_y (__comp_inst->j3_y)
#undef j3_z
#define j3_z (__comp_inst->j3_z)
#undef j3_init
#define j3_init (__comp_inst->j3_init)
#undef j3_min
#define j3_min (__comp_inst->j3_min)
#undef j3_max
#define j3_max (__comp_inst->j3_max)
#undef j4_x
#define j4_x (__comp_inst->j4_x)
#undef j4_y
#define j4_y (__comp_inst->j4_y)
#undef j4_z
#define j4_z (__comp_inst->j4_z)
#undef j4_init
#define j4_init (__comp_inst->j4_init)
#undef j4_min
#define j4_min (__comp_inst->j4_min)
#undef j4_max
#define j4_max (__comp_inst->j4_max)
#undef j5_x
#define j5_x (__comp_inst->j5_x)
#undef j5_y
#define j5_y (__comp_inst->j5_y)
#undef j5_z
#define j5_z (__comp_inst->j5_z)
#undef j5_init
#define j5_init (__comp_inst->j5_init)
#undef j5_min
#define j5_min (__comp_inst->j5_min)
#undef j5_max
#define j5_max (__comp_inst->j5_max)
#undef j0_rw
#define j0_rw (__comp_inst->j0_rw)
#undef j1_rw
#define j1_rw (__comp_inst->j1_rw)
#undef j2_rw
#define j2_rw (__comp_inst->j2_rw)
#undef j3_rw
#define j3_rw (__comp_inst->j3_rw)
#undef j4_rw
#define j4_rw (__comp_inst->j4_rw)
#undef j5_rw
#define j5_rw (__comp_inst->j5_rw)
#undef cart_x
#define cart_x (__comp_inst->cart_x)
#undef cart_y
#define cart_y (__comp_inst->cart_y)
#undef cart_z
#define cart_z (__comp_inst->cart_z)
#undef euler_x
#define euler_x (__comp_inst->euler_x)
#undef euler_y
#define euler_y (__comp_inst->euler_y)
#undef euler_z
#define euler_z (__comp_inst->euler_z)
#undef text
#define text (__comp_inst->text)

#include "rtapi_math.h"
#include "halmodule.h"

double ms;
double seconds;
double totalnsec;
struct data d; // give meat to the c++ code.
struct data r; // recieve meat from the c++ code.

FUNCTION(_) {
    totalnsec = totalnsec + period;
    ms = totalnsec * 0.000001;          // rtapi_print_msg(RTAPI_MSG_ERR,"totalms: %f \n", totalms);
    seconds = totalnsec * 0.000000001;  // rtapi_print_msg(RTAPI_MSG_ERR,"totalseconds: %f \n", totalseconds);

    d.J0_x=j0_x;
    d.J0_y=j0_y;
    d.J0_z=j0_z;
    d.J0_init=j0_init*toRadians;
    d.J0=j0_rw*toRadians;
    d.J0_min=j0_min*toRadians;
    d.J0_max=j0_max*toRadians;

    d.J1_x=j1_x;
    d.J1_y=j1_y;
    d.J1_z=j1_z;
    d.J1_init=j1_init*toRadians;
    d.J1=j1_rw*toRadians;
    d.J1_min=j1_min*toRadians;
    d.J1_max=j1_max*toRadians;

    d.J2_x=j2_x;
    d.J2_y=j2_y;
    d.J2_z=j2_z;
    d.J2_init=j2_init*toRadians;
    d.J2=j2_rw*toRadians;
    d.J2_min=j2_min*toRadians;
    d.J2_max=j2_max*toRadians;

    d.J3_x=j3_x;
    d.J3_y=j3_y;
    d.J3_z=j3_z;
    d.J3_init=j3_init*toRadians;
    d.J3=j3_rw*toRadians;
    d.J3_min=j3_min*toRadians;
    d.J3_max=j3_max*toRadians;

    d.J4_x=j4_x;
    d.J4_y=j4_y;
    d.J4_z=j4_z;
    d.J4_init=j4_init*toRadians;
    d.J4=j4_rw*toRadians;
    d.J4_min=j4_min*toRadians;
    d.J4_max=j4_max*toRadians;

    d.J5_x=j5_x;
    d.J5_y=j5_y;
    d.J5_z=j5_z;
    d.J5_init=j5_init*toRadians;
    d.J5=j5_rw*toRadians;
    d.J5_min=j5_min*toRadians;
    d.J5_max=j5_max*toRadians;

    // Test axis_mm.
    cart_x=pin_cartx_in;
    cart_y=pin_carty_in;
    cart_z=pin_cartz_in;

    d.Cartx=cart_x;
    d.Carty=cart_y;
    d.Cartz=cart_z;

    d.Eulerx=euler_x*toRadians;
    d.Eulery=euler_y*toRadians;
    d.Eulerz=euler_z*toRadians;
    
    d.Fk_mode=perform_fk; // Forward kinematics selected.
    d.Ik_mode=perform_ik;
    d.Ik_from_init=ik_from_init;
    d.Iterations=iterations;

    //struct data r=init_wrapper(d);

    if(!init){
        d.Fk_mode=1; // Always start up in fk mode. Set the cart values for one time.

        init=1;
    }

    if(d.Fk_mode){
        r=fk_wrapper(d);

        // Update cart and euler rw params. Joint params where the input and can stay the same.
        cart_x=r.Cartx;
        cart_y=r.Carty;
        cart_z=r.Cartz;
        euler_x=r.Eulerx*toDegrees;
        euler_y=r.Eulery*toDegrees;
        euler_z=r.Eulerz*toDegrees;

    }

    if(d.Ik_mode){
        r.error=0;
        r=ik_wrapper(d);

        // Update joint values
        j0_rw=r.J0*toDegrees;
        j1_rw=r.J1*toDegrees;
        j2_rw=r.J2*toDegrees;
        j3_rw=r.J3*toDegrees;
        j4_rw=r.J4*toDegrees;
        j5_rw=r.J5*toDegrees;

        if(r.error){
            rtapi_print_msg(RTAPI_MSG_ERR,"Kinematic error");
        }

    }

    // Pins to opencascade, net linked in opencascade.hal.
    if(!r.error && (d.Fk_mode || d.Ik_mode)){
        pin_j0=r.J0*toDegrees;
        pin_j1=r.J1*toDegrees;
        pin_j2=r.J2*toDegrees;
        pin_j3=r.J3*toDegrees;
        pin_j4=r.J4*toDegrees;
        pin_j5=r.J5*toDegrees;

        pin_cartx=r.Cartx;
        pin_carty=r.Carty;
        pin_cartz=r.Cartz;

        pin_eulerx=r.Eulerx*toDegrees;
        pin_eulery=r.Eulery*toDegrees;
        pin_eulerz=r.Eulerz*toDegrees;
    }

    if(debug){
        rtapi_print_msg(RTAPI_MSG_ERR,"MACHINE SETUP VALUES AFTER FIRST FORWARD KINEMATICS (fk): \n");
        rtapi_print_msg(RTAPI_MSG_ERR,"J0: %f \n", r.J0*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J1: %f \n", r.J1*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J2: %f \n", r.J2*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J3: %f \n", r.J3*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J4: %f \n", r.J4*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J5: %f \n", r.J5*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"cart x: %f \n", r.Cartx);
        rtapi_print_msg(RTAPI_MSG_ERR,"cart y: %f \n", r.Carty);
        rtapi_print_msg(RTAPI_MSG_ERR,"cart z: %f \n", r.Cartz);
        rtapi_print_msg(RTAPI_MSG_ERR,"euler x: %f \n", r.Eulerx*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"euler y: %f \n", r.Eulery*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"euler z: %f \n", r.Eulerz*toDegrees);
    }
}

static int __comp_get_data_size(void) { return 0; }
