#include "rtapi.h"
#include "rtapi_ctype.h"
#include "rtapi_app.h"
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "rtapi_math64.h"
#include <rtapi_io.h>
#include "hal.h"
#include <halmodule.h>

/* module information */
MODULE_AUTHOR("Skynet");
MODULE_DESCRIPTION("Halmodule trajectory");
MODULE_LICENSE("GPL");

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
//! Input pins:
float_data_t *max_velocity;
float_data_t *max_acceleration;
float_data_t *max_jerk;
float_data_t *current_position;
float_data_t *current_velocity;
float_data_t *current_acceleration;
float_data_t *velocity_override;
float_data_t *power_rpm_in, *power_rpm_out;
float_data_t *tcp_offset_x, *tcp_offset_y, *tcp_offset_z;

//! Output pins:
float_data_t *tcp_x, *tcp_y, *tcp_z;
float_data_t *tcp_euler_x, *tcp_euler_y, *tcp_euler_z;

//! Pins
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t *enable;
bit_data_t *m3_in, *m7_in, *m8_in;
bit_data_t *m3_out, *m7_out, *m8_out;

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

//! Params
typedef struct {
    hal_float_t Pin;
} param_float_data_t;
param_float_data_t *start_from_gcodeblock;

typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;
param_bit_data_t *load_file, *load_mdi, *start_, *stop_, *run_from_line;
param_bit_data_t *mode_auto, *mode_jog;
param_bit_data_t *jog_x_plus, *jog_x_min, *jog_y_plus, *jog_y_min, *jog_z_plus, *jog_z_min;
param_bit_data_t *jog_euler_x_plus, *jog_euler_x_min, *jog_euler_y_plus, *jog_euler_y_min, *jog_euler_z_plus, *jog_euler_z_min;

static int comp_idx; /* component ID */

// Hal port char array.
char dest[140] = {};

bool trigger_auto=0, trigger_mdi=0, trigger_jog=0;
bool trigger_start=0, trigger_stop=0, trigger_pause=0, trigger_resume=0;

static void trajectory();
static int setup_pins();
static void perform_auto();
static void perform_jog();

int rtapi_app_main(void) {

    int r = 0;
    comp_idx = hal_init("hal_trajectory");
    if(comp_idx < 0) return comp_idx;
    r = hal_export_funct("hal_trajectory", trajectory, &skynet,0,0,comp_idx);

    r+=setup_pins();

    // Start up in pause. When user press start, program's begin.
    stop_->Pin=1;

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }
    return 0;
}

void rtapi_app_exit(void){
    hal_exit(comp_idx);
}

//! Perforn's every ms.
static void trajectory(){

    // For emergency, enable=0.
    // To reload or load a new gcode file, setp trajectory.enable 0 => setp.trajectory.load_file 1 => setp.trajectory.run 1
    if(*enable->Pin==1){

        if(mode_auto->Pin){
            trigger_auto=1;
            trigger_jog=0;
        }
        if(mode_jog->Pin){
            trigger_auto=0;
            trigger_jog=1;
        }

        if(trigger_auto){
            perform_auto();

        }
        if(trigger_jog){
            perform_jog();
        }

        tcp=get_tcp();

        // Update hal output before mode auto,jog,mdi.
        *tcp_x->Pin=tcp.p.x+*tcp_offset_x->Pin;
        *tcp_y->Pin=tcp.p.y+*tcp_offset_y->Pin;
        *tcp_z->Pin=tcp.p.z+*tcp_offset_z->Pin;

        *tcp_euler_x->Pin=tcp.e.euler_x;
        *tcp_euler_y->Pin=tcp.e.euler_y;
        *tcp_euler_z->Pin=tcp.e.euler_z;

        *current_velocity->Pin=tcp.vel;
        *current_acceleration->Pin=tcp.acc;
        *current_position->Pin=tcp.pos;
    }

    bool ok= hal_port_read(*port->Pin,dest,140);
    if(ok){
        rtapi_print_msg(RTAPI_MSG_ERR,"trajectory received status 0-1 :'%d'\n", ok);
        rtapi_print_msg(RTAPI_MSG_ERR,"trajectory port dest data :'%s'\n", dest);
    }

    // Input pin to load the gcode file, hal_port holds the filepath + .ngc file.
    if(load_file->Pin==1){ // Tested ok.
        wrapper_load_gcode(dest);
        load_file->Pin=0; // Reset pin.
    }

    if(load_mdi->Pin==1){
        wrapper_load_gcode(dest);
        load_mdi->Pin=0;
    }
}

static void perform_jog(){
    int x,y,z,eulx,euly,eulz;

    if(jog_x_plus->Pin){
        jog_x_min->Pin=0; // Only one way is permitted.
        x=1;
    }
    if(jog_x_min->Pin){
        jog_x_plus->Pin=0;
        x=-1;
    }
    if(!jog_x_plus->Pin && !jog_x_min->Pin){
        x=0;
    }

    if(jog_y_plus->Pin){
        jog_y_min->Pin=0;
        y=1;
    }
    if(jog_y_min->Pin){
        jog_y_plus->Pin=0;
        y=-1;
    }
    if(!jog_y_plus->Pin && !jog_y_min->Pin){
        y=0;
    }

    if(jog_z_plus->Pin){
        jog_z_min->Pin=0;
        z=1;
    }
    if(jog_z_min->Pin){
        jog_z_plus->Pin=0;
        z=-1;
    }
    if(!jog_z_plus->Pin && !jog_z_min->Pin){
        z=0;
    }

    if(jog_euler_x_plus->Pin){
        jog_euler_x_min->Pin=0;
        eulx=1;
    }
    if(jog_euler_x_min->Pin){
        jog_euler_x_plus->Pin=0;
        eulx=-1;
    }
    if(!jog_euler_x_plus->Pin && !jog_euler_x_min->Pin){
        eulx=0;
    }

    if(jog_euler_y_plus->Pin){
        jog_euler_y_min->Pin=0;
        euly=1;
    }
    if(jog_euler_y_min->Pin){
        jog_euler_y_plus->Pin=0;
        euly=-1;
    }
    if(!jog_euler_y_plus->Pin && !jog_euler_y_min->Pin){
        euly=0;
    }

    if(jog_euler_z_plus->Pin){
        jog_euler_z_min->Pin=0;
        eulz=1;
    }
    if(jog_euler_z_min->Pin){
        jog_euler_z_plus->Pin=0;
        eulz=-1;
    }
    if(!jog_euler_z_plus->Pin && !jog_euler_z_min->Pin){
        eulz=0;
    }

    // Every ms:
    wrapper_trajectory_jog(x,y,z,*max_velocity->Pin,*max_acceleration->Pin,*max_jerk->Pin);

    *m3_out->Pin=*m3_in->Pin;
    *m7_out->Pin=*m7_in->Pin;
    *m8_out->Pin=*m8_in->Pin;
    *power_rpm_out->Pin=*power_rpm_in->Pin;
}

static void perform_auto(){
    // Every ms:
    wrapper_trajectory_auto(*velocity_override->Pin,
                           start_->Pin,
                           stop_->Pin,
                           *max_velocity->Pin,*max_acceleration->Pin,*max_jerk->Pin, start_from_gcodeblock->Pin, run_from_line->Pin);

    start_->Pin=0;
    stop_->Pin=0;
    run_from_line->Pin=0;

    *m3_out->Pin=tcp.m3;
    *m7_out->Pin=tcp.m7;
    *m8_out->Pin=tcp.m8;
    *power_rpm_out->Pin=tcp.power_rpm;
}

static int setup_pins(){
    int r=0;

    //! Input port.
    port = (port_data_t*)hal_malloc(sizeof(char[150]));
    r+=hal_pin_port_new("hal_trajectory.port",HAL_IN,&(port->Pin),comp_idx);

    //hal_signal_new("signal_filename",HAL_PORT); // Is created from libhalcmdgui. This is the trajectory gui program.
    hal_link("hal_trajectory.port","signal_filename");

    //! Input pins, type bit.
    enable = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_trajectory.enable",HAL_IN,&(enable->Pin),comp_idx);

    m3_in = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_trajectory.m3_in",HAL_IN,&(m3_in->Pin),comp_idx);
    m7_in = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_trajectory.m7_in",HAL_IN,&(m7_in->Pin),comp_idx);
    m8_in = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_trajectory.m8_in",HAL_IN,&(m8_in->Pin),comp_idx);

    //! Output pins, type bit.
    m3_out = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_trajectory.m3_out",HAL_OUT,&(m3_out->Pin),comp_idx);
    m7_out = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_trajectory.m7_out",HAL_OUT,&(m7_out->Pin),comp_idx);
    m8_out = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_trajectory.m8_out",HAL_OUT,&(m8_out->Pin),comp_idx);


    //! Input parameter, type bit.
    load_file = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.load_file",HAL_RW,&(load_file->Pin),comp_idx);

    load_mdi = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.load_mdi",HAL_RW,&(load_mdi->Pin),comp_idx);

    mode_auto = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.mode_auto",HAL_RW,&(mode_auto->Pin),comp_idx);
    mode_jog = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.mode_jog",HAL_RW,&(mode_jog->Pin),comp_idx);

    //! Jog input pins.
    jog_x_plus = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_x_plus",HAL_RW,&(jog_x_plus->Pin),comp_idx);
    jog_x_min = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_x_min",HAL_RW,&(jog_x_min->Pin),comp_idx);

    jog_y_plus = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_y_plus",HAL_RW,&(jog_y_plus->Pin),comp_idx);
    jog_y_min = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_y_min",HAL_RW,&(jog_y_min->Pin),comp_idx);

    jog_z_plus = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_z_plus",HAL_RW,&(jog_z_plus->Pin),comp_idx);
    jog_z_min = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_z_min",HAL_RW,&(jog_z_min->Pin),comp_idx);

    jog_euler_x_plus = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_euler_x_plus",HAL_RW,&(jog_euler_x_plus->Pin),comp_idx);
    jog_euler_x_min = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_euler_x_min",HAL_RW,&(jog_euler_x_min->Pin),comp_idx);

    jog_euler_y_plus = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_euler_y_plus",HAL_RW,&(jog_euler_y_plus->Pin),comp_idx);
    jog_euler_y_min = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_euler_y_min",HAL_RW,&(jog_euler_y_min->Pin),comp_idx);

    jog_euler_z_plus = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_euler_z_plus",HAL_RW,&(jog_euler_z_plus->Pin),comp_idx);
    jog_euler_z_min = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.jog_euler_z_min",HAL_RW,&(jog_euler_z_min->Pin),comp_idx);

    //! Names like start, stop, will give error in hal.
    start_ = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.start",HAL_RW,&(start_->Pin),comp_idx);

    stop_ = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.stop",HAL_RW,&(stop_->Pin),comp_idx);

    run_from_line = (param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("hal_trajectory.run_from_line",HAL_RW,&(run_from_line->Pin),comp_idx);

    //! Input parameter, type float.
    start_from_gcodeblock = (param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_trajectory.start_from_gcodeblock",HAL_RW,&(start_from_gcodeblock->Pin),comp_idx);

    //! Input pins, type float.
    max_velocity = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.max_velocity",HAL_IN,&(max_velocity->Pin),comp_idx);

    max_acceleration = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.max_acceleration",HAL_IN,&(max_acceleration->Pin),comp_idx);

    max_jerk = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.max_jerk",HAL_IN,&(max_jerk->Pin),comp_idx);

    velocity_override = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.velocity_override",HAL_IN,&(velocity_override->Pin),comp_idx);

    power_rpm_in = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.power_rpm_in",HAL_IN,&(power_rpm_in->Pin),comp_idx);

    tcp_offset_x = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_offset_x",HAL_IN,&(tcp_offset_x->Pin),comp_idx);

    tcp_offset_y = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_offset_y",HAL_IN,&(tcp_offset_y->Pin),comp_idx);

    tcp_offset_z = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_offset_z",HAL_IN,&(tcp_offset_z->Pin),comp_idx);

    //! Output pins, type float.
    tcp_x = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_x",HAL_OUT,&(tcp_x->Pin),comp_idx);

    tcp_y = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_y",HAL_OUT,&(tcp_y->Pin),comp_idx);

    tcp_z = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_z",HAL_OUT,&(tcp_z->Pin),comp_idx);

    tcp_euler_x = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_euler_x",HAL_OUT,&(tcp_euler_x->Pin),comp_idx);

    tcp_euler_y = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_euler_y",HAL_OUT,&(tcp_euler_y->Pin),comp_idx);

    tcp_euler_z = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.tcp_euler_z",HAL_OUT,&(tcp_euler_z->Pin),comp_idx);


    current_position = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.current_position",HAL_OUT,&(current_position->Pin),comp_idx);

    current_velocity = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.current_velocity",HAL_OUT,&(current_velocity->Pin),comp_idx);

    current_acceleration = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.current_acceleration",HAL_OUT,&(current_acceleration->Pin),comp_idx);

    power_rpm_out = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_trajectory.power_rpm_out",HAL_OUT,&(power_rpm_out->Pin),comp_idx);

    return r;
}







































