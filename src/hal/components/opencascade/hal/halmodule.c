/// Improved by Skynet 02-11-2021
/// Changed from halcompile cody style to c code style.
/// First loadrt the kinematics, then loadrt the opencascade, otherwise it could prevent opening the opencascade gui app.
#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "rtapi_math64.h"
#include "rtapi_math.h"
#include "halmodule.h"

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *pin_j0;
float_data_t *pin_j1;
float_data_t *pin_j2;
float_data_t *pin_j3;
float_data_t *pin_j4;
float_data_t *pin_j5;
float_data_t *pin_cartx;
float_data_t *pin_carty;
float_data_t *pin_cartz;
float_data_t *pin_eulerx;
float_data_t *pin_eulery;
float_data_t *pin_eulerz;

typedef struct {
    hal_bit_t *Pin;
} bit_data_t;

typedef struct {
    hal_s32_t *Pin;
} s32_data_t;

typedef struct {
    hal_u32_t *Pin;
} u32_data_t;

typedef struct {
    hal_port_t *Pin;
} port_data_t;
port_data_t *port;

typedef struct {
    hal_float_t Pin;
} param_float_data_t;
param_float_data_t *j0_x;
param_float_data_t *j0_y;
param_float_data_t *j0_z;
param_float_data_t *j1_x;
param_float_data_t *j1_y;
param_float_data_t *j1_z;
param_float_data_t *j2_x;
param_float_data_t *j2_y;
param_float_data_t *j2_z;
param_float_data_t *j3_x;
param_float_data_t *j3_y;
param_float_data_t *j3_z;
param_float_data_t *j4_x;
param_float_data_t *j4_y;
param_float_data_t *j4_z;
param_float_data_t *j5_x;
param_float_data_t *j5_y;
param_float_data_t *j5_z;

// Quaternion input values that can translate & rotate gcode.
param_float_data_t *gcode_x, *gcode_y, *gcode_z, *gcode_euler_x, *gcode_euler_y, *gcode_euler_z;

typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;
param_bit_data_t *j0_rotx;
param_bit_data_t *j0_roty;
param_bit_data_t *j0_rotz;
param_bit_data_t *j1_rotx;
param_bit_data_t *j1_roty;
param_bit_data_t *j1_rotz;
param_bit_data_t *j2_rotx;
param_bit_data_t *j2_roty;
param_bit_data_t *j2_rotz;
param_bit_data_t *j3_rotx;
param_bit_data_t *j3_roty;
param_bit_data_t *j3_rotz;
param_bit_data_t *j4_rotx;
param_bit_data_t *j4_roty;
param_bit_data_t *j4_rotz;
param_bit_data_t *j5_rotx;
param_bit_data_t *j5_roty;
param_bit_data_t *j5_rotz;

bool init;
struct data d;

// http://linuxcnc.org/docs/2.8/html/man/man3/RTAPI_MP_ARRAY_STRING.3rtapi.html
char *stepfilepath;
RTAPI_MP_STRING(stepfilepath,"");

int display_hz;
RTAPI_MP_INT(display_hz,"");

// Example of array
// char *joint0[6] = {0,0,0,0,0,0};
// RTAPI_MP_ARRAY_STRING(joint0, 6, "");

static void function();
static int setup_pins();

int rtapi_app_main(void) {

    comp_idx = hal_init("hal_opencascade");
    if(comp_idx < 0) return comp_idx;

    int r = 0;
    r = hal_export_funct("hal_opencascade", function, &skynet,0,0,comp_idx);
    r+=setup_pins();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }
    return r;
}

void rtapi_app_exit(void) {
    hal_exit(comp_idx);
}

//! Perforn's every servo-period 1ms.
static void function(){

    if(!init){
        init_opencascade(stepfilepath,display_hz);
        init=1;
    }

    d.J0=*pin_j0->Pin; // Copy hal parameter values to the shared (parallel) c++ program data bucket.
    d.J1=*pin_j1->Pin;
    d.J2=*pin_j2->Pin;
    d.J3=*pin_j3->Pin;
    d.J4=*pin_j4->Pin;
    d.J5=*pin_j5->Pin;
    d.Cartx=*pin_cartx->Pin;
    d.Carty=*pin_carty->Pin;
    d.Cartz=*pin_cartz->Pin;
    d.Eulerx=*pin_eulerx->Pin;
    d.Eulery=*pin_eulery->Pin;
    d.Eulerz=*pin_eulerz->Pin;

    d.J0x=j0_x->Pin;
    d.J0y=j0_y->Pin;
    d.J0z=j0_z->Pin;
    d.J0_rotx=j0_rotx->Pin;
    d.J0_roty=j0_roty->Pin;
    d.J0_rotz=j0_rotz->Pin;

    d.J1x=j1_x->Pin;
    d.J1y=j1_y->Pin;
    d.J1z=j1_z->Pin;
    d.J1_rotx=j1_rotx->Pin;
    d.J1_roty=j1_roty->Pin;
    d.J1_rotz=j1_rotz->Pin;

    d.J2x=j2_x->Pin;
    d.J2y=j2_y->Pin;
    d.J2z=j2_z->Pin;
    d.J2_rotx=j2_rotx->Pin;
    d.J2_roty=j2_roty->Pin;
    d.J2_rotz=j2_rotz->Pin;

    d.J3x=j3_x->Pin;
    d.J3y=j3_y->Pin;
    d.J3z=j3_z->Pin;
    d.J3_rotx=j3_rotx->Pin;
    d.J3_roty=j3_roty->Pin;
    d.J3_rotz=j3_rotz->Pin;

    d.J4x=j4_x->Pin;
    d.J4y=j4_y->Pin;
    d.J4z=j4_z->Pin;
    d.J4_rotx=j4_rotx->Pin;
    d.J4_roty=j4_roty->Pin;
    d.J4_rotz=j4_rotz->Pin;

    d.J5x=j5_x->Pin;
    d.J5y=j5_y->Pin;
    d.J5z=j5_z->Pin;
    d.J5_rotx=j5_rotx->Pin;
    d.J5_roty=j5_roty->Pin;
    d.J5_rotz=j5_rotz->Pin;

    d.gcode_x=gcode_x->Pin;
    d.gcode_y=gcode_y->Pin;
    d.gcode_z=gcode_z->Pin;
    d.gcode_euler_x=gcode_euler_x->Pin;
    d.gcode_euler_y=gcode_euler_y->Pin;
    d.gcode_euler_z=gcode_euler_z->Pin;

    meat(d); // Give meat the values.
}


EXTRA_SETUP(){
if (stepfilepath == 0) {
rtapi_print_msg(RTAPI_MSG_ERR,"stepfilepath string missing\n");
return -EINVAL;
}
rtapi_print_msg(RTAPI_MSG_ERR,"stepfilepath: %s\n",stepfilepath);

if (display_hz == 0) {
rtapi_print_msg(RTAPI_MSG_ERR,"display_hz string missing\n");
return -EINVAL;
}
rtapi_print_msg(RTAPI_MSG_ERR,"display_hz: %d\n",display_hz);
return(0);

// Example of array
//myidx = extra_arg;
//if(myidx<0 || myidx >2)
//{
//    rtapi_print_msg(RTAPI_MSG_ERR,"arg is outside the allowable range 0..2\n");
//    return -EINVAL;
//}
//if(arg[myidx] == 0)
//{
//    rtapi_print_msg(RTAPI_MSG_ERR,"arg string for index %d missing\n", myidx);
//    return -EINVAL;
//}
//rtapi_print_msg(RTAPI_MSG_ERR,"stepfilepath : %s\n",arg[0]);
//rtapi_print_msg(RTAPI_MSG_ERR,"opencascade screen hz : %s\n",arg[1]);
//return(0);
}


static int setup_pins(){
    int r=0;

    // Pins:
    pin_j0=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-j0",HAL_IN,&(pin_j0->Pin),comp_idx);

    pin_j1=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-j1",HAL_IN,&(pin_j1->Pin),comp_idx);

    pin_j2=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-j2",HAL_IN,&(pin_j2->Pin),comp_idx);

    pin_j3=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-j3",HAL_IN,&(pin_j3->Pin),comp_idx);

    pin_j4=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-j4",HAL_IN,&(pin_j4->Pin),comp_idx);

    pin_j5=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-j5",HAL_IN,&(pin_j5->Pin),comp_idx);

    pin_cartx=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-cartx",HAL_IN,&(pin_cartx->Pin),comp_idx);

    pin_carty=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-carty",HAL_IN,&(pin_carty->Pin),comp_idx);

    pin_cartz=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-cartz",HAL_IN,&(pin_cartz->Pin),comp_idx);

    pin_eulerx=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-eulerx",HAL_IN,&(pin_eulerx->Pin),comp_idx);

    pin_eulery=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-eulery",HAL_IN,&(pin_eulery->Pin),comp_idx);

    pin_eulerz=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_opencascade.pin-eulerz",HAL_IN,&(pin_eulerz->Pin),comp_idx);

    // Parameters float:
    j0_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j0-x",HAL_RW,&(j0_x->Pin),comp_idx);

    j0_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j0-y",HAL_RW,&(j0_y->Pin),comp_idx);

    j0_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j0-z",HAL_RW,&(j0_z->Pin),comp_idx);

    j1_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j1-x",HAL_RW,&(j1_x->Pin),comp_idx);

    j1_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j1-y",HAL_RW,&(j1_y->Pin),comp_idx);

    j1_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j1-z",HAL_RW,&(j1_z->Pin),comp_idx);

    j2_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j2-x",HAL_RW,&(j2_x->Pin),comp_idx);

    j2_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j2-y",HAL_RW,&(j2_y->Pin),comp_idx);

    j2_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j2-z",HAL_RW,&(j2_z->Pin),comp_idx);

    j3_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j3-x",HAL_RW,&(j3_x->Pin),comp_idx);

    j3_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j3-y",HAL_RW,&(j3_y->Pin),comp_idx);

    j3_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j3-z",HAL_RW,&(j3_z->Pin),comp_idx);

    j4_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j4-x",HAL_RW,&(j4_x->Pin),comp_idx);

    j4_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j4-y",HAL_RW,&(j4_y->Pin),comp_idx);

    j4_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j4-z",HAL_RW,&(j4_z->Pin),comp_idx);

    j5_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j5-x",HAL_RW,&(j5_x->Pin),comp_idx);

    j5_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j5-y",HAL_RW,&(j5_y->Pin),comp_idx);

    j5_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.j5-z",HAL_RW,&(j5_z->Pin),comp_idx);

    gcode_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.gcode-x",HAL_RW,&(gcode_x->Pin),comp_idx);

    gcode_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.gcode-y",HAL_RW,&(gcode_y->Pin),comp_idx);

    gcode_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.gcode-z",HAL_RW,&(gcode_z->Pin),comp_idx);

    gcode_euler_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.gcode-euler-x",HAL_RW,&(gcode_euler_x->Pin),comp_idx);

    gcode_euler_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.gcode-euler-y",HAL_RW,&(gcode_euler_y->Pin),comp_idx);

    gcode_euler_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_opencascade.gcode-euler-z",HAL_RW,&(gcode_euler_z->Pin),comp_idx);

    // Parameters bit:
    j0_rotx=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j0-rotx",HAL_RW,&(j0_rotx->Pin),comp_idx);

    j0_roty=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j0-roty",HAL_RW,&(j0_roty->Pin),comp_idx);

    j0_rotz=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j0-rotz",HAL_RW,&(j0_rotz->Pin),comp_idx);

    j1_rotx=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j1-rotx",HAL_RW,&(j1_rotx->Pin),comp_idx);

    j1_roty=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j1-roty",HAL_RW,&(j1_roty->Pin),comp_idx);

    j1_rotz=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j1-rotz",HAL_RW,&(j1_rotz->Pin),comp_idx);

    j2_rotx=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j2-rotx",HAL_RW,&(j2_rotx->Pin),comp_idx);

    j2_roty=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j2-roty",HAL_RW,&(j2_roty->Pin),comp_idx);

    j2_rotz=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j2-rotz",HAL_RW,&(j2_rotz->Pin),comp_idx);

    j3_rotx=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j3-rotx",HAL_RW,&(j3_rotx->Pin),comp_idx);

    j3_roty=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j3-roty",HAL_RW,&(j3_roty->Pin),comp_idx);

    j3_rotz=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j3-rotz",HAL_RW,&(j3_rotz->Pin),comp_idx);

    j4_rotx=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j4-rotx",HAL_RW,&(j4_rotx->Pin),comp_idx);

    j4_roty=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j4-roty",HAL_RW,&(j4_roty->Pin),comp_idx);

    j4_rotz=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j4-rotz",HAL_RW,&(j4_rotz->Pin),comp_idx);

    j5_rotx=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j5-rotx",HAL_RW,&(j5_rotx->Pin),comp_idx);

    j5_roty=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j5-roty",HAL_RW,&(j5_roty->Pin),comp_idx);

    j5_rotz=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_opencascade.j5-rotz",HAL_RW,&(j5_rotz->Pin),comp_idx);

    return r;
}





















