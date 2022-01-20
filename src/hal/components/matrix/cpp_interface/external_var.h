#ifndef EXTERNAL_VAR_H
#define EXTERNAL_VAR_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

//! Represent a 3d point in xyz space.
struct POINT {
    double x,y,z;
};

//! Kdl euler angles. We do not use Yaw,Roll and Pitch values to avoid confusing.
struct EULER {
    double x,y,z;
};

//! Kinematic mode.
enum KINMODE{
    forward_kinematic,
    inverse_kinematic
};

//! In ik mode, it makes difference when rotating around xyz eulers. For example if rotating around euler y, the ik has to do the y rotation at last.
enum EULERMODE{
    rotation_x,
    rotation_y,
    rotation_z
};

//! Machinemode, manual, auto, estop.
enum MACHINESTATE{
    //! I suppose MDI & MANUAL can be used as a single mode.
    //! If in manual mode, we can give a mdi command, no problem.
    ESTOP,
    MANUAL,
    AUTO,
    MDI
};

//! Programstate, run, pause_resume, stop
enum PROGRAMSTATE{
    NONE,
    RUN,
    PAUSE_RESUME,
    STOP
};

//! Simulation or realtime motion, Offline or Online.
enum REALTIMESTATE{
    OFFLINE,
    ONLINE
};

//! Motion planner variables for every machine instance.
struct SCURVE {
    double at_time;
    double dtg_fwd, dtg_bck;
    double old_vs;
    double old_acc;
    double old_gain;

    bool pause_init, stop_init, run_init, gcode_init, startup_init;
    bool pause_request, stop_request, run_request;
    bool pos, neg, no_motion, end_of_traject;

    double feedrate;
    //! Feed overide factor 1~1.3
    double feed_overide;
    //! Scurve input values.
    double vs, vo, ve, s, a, gain;

    //! Auto mode, current exec gcodeline or block. Is readed when "run" is pressed.
    unsigned int cur_gcodevec;
    unsigned int cur_gcodeline;
    //! Run from line.
    unsigned int run_from_line;
    //! Gcode distancetogo dtg for current gcodeblock.
    double gc_dtg_curblock;
    //! Flag to highlight new gcode line.
    bool flag_highlight_new_gcodeline;

    bool mdi_ready_to_run, gcode_ready_to_run, mdi_reset, gcode_reset, mdi_remove_graphics, message_not_in_startpos;

    char message[250];
};

//! Every machine can have multiple joints to form a kinematic structure or chain.
struct JOINT {
    //! Joint type, rotation or translation.
    //! //! Kdl kinematic setup input values for a axis or joint.
    //! # rotationtype : rot_x = 0, rot_y = 1, rot_z = 2, trans_x = 3, trans_y = 4, trans_z = 5, rot_axis = 6, trans_axis =7, none = 8
    double rotationtype;
    //! Kinematic chain, coordinates end of axis.
    struct POINT axis_endpoint;
    //! Joint values in degrees
    double init;
    double curpos_abs, curpos_rel, rel_offset;
    //! The abs curpos when btn is pressed.
    double scurve_abs_joint_init;
    //! The abs target position to move to.
    double scurve_abs_joint_request;
    //! Jog timer.
    double at_jog_time;
    bool jog_joint_init;
    //! Joint is of type rotation. Set by halmodule.c btn press in jog online mode
    bool rot;

    double min;
    double max;
    int jointlimit;
    double curvel;
    double curacc;
    //! Scurve hal input values.
    double maxvel;
    double maxacc;
    double jerk;
    //! Joint matrix, holds data for current pose: translation & rotation.
    double matrix[4][4];
    char stepfile[256];
    //! Btn control state:
    bool joint_plus, joint_min;
};

struct MACHINE {
    //! Every machine id has it's mode. See structure above.
    enum MACHINESTATE state_machine;
    //! NONE, RUN, PAUSE_RESUME, STOP
    enum PROGRAMSTATE state_program;
    //! REALTIMESTATE, state_realtime.
    enum REALTIMESTATE state_realtime;
    //! Absolute machine offsets, hal input.
    struct POINT offset_cart;
    struct EULER offset_euler;
    //! Joint data structure
    struct JOINT *joint;
    //! Joint ammount array for each machine.
    unsigned int joints;
    char stepfile[256];
    //! Opencascade transparancy value : 0.0 - 1.0
    double transparancy;
    //! Current kinematic mode state.
    enum KINMODE kinmode;
    //! Current eulermode, state set by a euler x,y or z buttonpress.
    enum EULERMODE eulmode;
    //! Absolute machine positions.
    //! This are the non editable machine positions.
    struct POINT abs_cart;
    struct EULER abs_euler;
    //! Relative machine positions.
    //! This positions can be set to zero in 3d. The gcode will run from this 0.0.0 3d coordinate.
    struct POINT rel_offset_cart;
    struct EULER rel_offset_euler;
    struct POINT rel_cart;
    struct EULER rel_euler;
    //! Btn control state:
    double stepsize;
    bool btn_cartx_plus, btn_cartx_min;
    bool btn_carty_plus, btn_carty_min;
    bool btn_cartz_plus, btn_cartz_min;
    bool btn_eulerx_plus, btn_eulerx_min;
    bool btn_eulery_plus, btn_eulery_min;
    bool btn_eulerz_plus, btn_eulerz_min;
    bool btn_toolx_plus, btn_toolx_min;
    bool btn_tooly_plus, btn_tooly_min;
    bool btn_toolz_plus, btn_toolz_min;

    bool btn_zero_rel_all;
    bool btn_zero_rel_cartx;
    bool btn_zero_rel_carty;
    bool btn_zero_rel_cartz;
    bool btn_zero_rel_eulerx;
    bool btn_zero_rel_eulery;
    bool btn_zero_rel_eulerz;
    bool btn_zero_rel_J0;
    bool btn_zero_rel_J1;
    bool btn_zero_rel_J2;
    bool btn_zero_rel_J3;
    bool btn_zero_rel_J4;
    bool btn_zero_rel_J5;;

    bool btn_mdi_exec;
    bool btn_mode_auto;
    bool btn_mode_manual;
    bool btn_mode_mdi;
    bool btn_run;
    bool btn_stop;
    bool btn_pause_resume;
    bool btn_estop;
    bool btn_offline, btn_online;

    //! Hal inputs
    double maxvel_cart,maxacc_cart,gain_cart;
    //! Used by motion planner to trigger a velmax change.
    double maxvel_cart_old;
    //! Hal inputs
    double maxvel_euler,maxacc_euler,gain_euler;

    struct SCURVE sc;

    //! Ferror in mm. Path following error.
    double ferror;
    struct POINT tcp;

    //! The cart pos when btn is pressed.
    double scurve_jog_abs_cartx_init;
    //! The abs target position to move to.
    double scurve_jog_abs_cartx_request;
    //! Scurve timer.
    double at_cartx_time;
    //! Scurve flag.
    bool jog_cartx_init;

    //! The cart pos when btn is pressed.
    double scurve_jog_abs_carty_init;
    //! The abs target position to move to.
    double scurve_jog_abs_carty_request;
    //! Scurve timer.
    double at_carty_time;
    //! Scurve flag.
    bool jog_carty_init;

    //! The cart pos when btn is pressed.
    double scurve_jog_abs_cartz_init;
    //! The abs target position to move to.
    double scurve_jog_abs_cartz_request;
    //! Scurve timer.
    double at_cartz_time;
    //! Scurve flag.
    bool jog_cartz_init;

    //! The cart pos when btn is pressed.
    double scurve_jog_abs_eulerx_init;
    //! The abs target position to move to.
    double scurve_jog_abs_eulerx_request;
    //! Scurve timer.
    double at_eulerx_time;
    //! Scurve flag.
    bool jog_eulerx_init;

    //! The cart pos when btn is pressed.
    double scurve_jog_abs_eulery_init;
    //! The abs target position to move to.
    double scurve_jog_abs_eulery_request;
    //! Scurve timer.
    double at_eulery_time;
    //! Scurve flag.
    bool jog_eulery_init;

    //! The cart pos when btn is pressed.
    double scurve_jog_abs_eulerz_init;
    //! The abs target position to move to.
    double scurve_jog_abs_eulerz_request;
    //! Scurve timer.
    double at_eulerz_time;
    //! Scurve flag.
    bool jog_eulerz_init;

    //! The tool pos when btn is pressed.
    struct POINT scurve_jog_abs_toolx_init;
    //! The abs target position to move to.
    struct POINT scurve_jog_abs_toolx_request;
    //! Scurve timer.
    double at_toolx_time;
    //! Scurve flag.
    bool jog_toolx_init;

    //! The tool pos when btn is pressed.
    struct  POINT scurve_jog_abs_tooly_init;
    //! The abs target position to move to.
    struct POINT scurve_jog_abs_tooly_request;
    //! Scurve timer.
    double at_tooly_time;
    //! Scurve flag.
    bool jog_tooly_init;

    //! The tool pos when btn is pressed.
    struct POINT scurve_jog_abs_toolz_init;
    //! The abs target position to move to.
    struct POINT scurve_jog_abs_toolz_request;
    //! Scurve timer.
    double at_toolz_time;
    //! Scurve flag.
    bool jog_toolz_init;

    //! Hal macro message's.
    unsigned int gcode_macro_nr;
    double gcode_macro_value;

};

struct DATA {
    //! Machine data structure
    struct MACHINE *machine;
    //! Ammount of machines.
    unsigned int machines;
    //! Kinematic ammount of calculations to find optimal kinematic result ~100
    int iterations;
    //! Becomes true if *ptr data is ready to recieve.
    bool ready;
    //! Servo period.
    double servoperiod;
    //! Base period.
    double baseperiod;
};

#endif // EXTERNAL_VAR_H
