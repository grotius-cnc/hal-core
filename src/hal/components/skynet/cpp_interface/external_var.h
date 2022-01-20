#ifndef EXTERNAL_VAR_H
#define EXTERNAL_VAR_H

#include <stdlib.h>
#include <string>
#include <vector>

#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

//! This file is a duplicate of the halmodule.h file that is used in the "c" coded kernel module.
//! If the "c++" code uses the same data structure we are able to pass data between c and c++.

//! Represent a 3d point in xyz space.
struct POINT {
    double x,y,z;
};

//! Kdl euler angles. We do not use Yaw,Roll and Pitch values to avoid confusing.
struct EULER {
    double x,y,z;
};

//! fk_rapid, ik_rapid, ik_linear
enum KINMODE{
    fk_rapid,
    fk_rapid_tooldir,
    ik_rapid_tooldir,
    ik_rapid
};

enum MODE{
    MDI_MANUAL,AUTO
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
    double curpos;
    double min;
    double max;
    bool jointlimit;
    double curvel;
    double curacc;
    double tarpos;
    //! Ruckig hal input values.
    double maxvel;
    double maxacc;
    double jerk;
    //! Hold the data for the gui app.
    double gui_maxvel;
};

struct DATA {
    int machines;
    int *joints;
    int screenupdatems;
    char *stepfilepath;
};

struct MACHINE {
  //! Multiple joints for a machine are possible.
  struct JOINT *joint;
  //! Cartesian & euler result from inverse kinematics.
  double tarpos_cart_euler[6], curpos_cart_euler[6], curpos_cart_euler_restore[6];
  //! Machine cartestian & euler speed, acc, jerk. Absolute machine positions.
  double curvel_cart_euler[6], curacc_cart_euler[6], maxvel_cart, maxacc_cart, jerk_cart, maxvel_euler, maxacc_euler, jerk_euler;
  //! Machine relative positions. This coordinate system allows you to set your own point of reference.
  double curpos_cart_euler_rel[6], curoffset_cart_euler_rel[6], curpos_tool[6];
  //! Gcode Dofs variables
  double curvel_gcode, curacc_gcode, curpos_gcode;
  //! Current executed gcodeblock.
  unsigned int exec_gcodeblock;
  //! Gui values to display
  double gui_maxvel_cart, gui_maxvel_euler;
  //! Machine 3d position by quaternion.
  struct POINT machine_offset;
  struct EULER machine_rotation;
  //! Kinematic ammount of calculations to find optimal kinematic result ~100
  int kinematic_iterations;
  int kinematic_error;
  //! Wich type of calculation is requested.
  enum KINMODE kinmode;
  bool error;
  //! Can have multiple copies, but no problem.
  struct DATA data;
  bool ruckig_finished;
  struct POINT tooldir_cart, tooldir_euler;
  //! Activate ruckig realtime motion.
  bool rt_motion;
  //! Machine mode: auto,mdi,manual.
  enum MODE mode;
};
//! External variables for opencascade stand alone thread.
extern MACHINE *e_ptr;

//! Gcode rapid, line, arc.
enum BLOCKTYPE {
    G0, G1, G2, G3
};

//! Structs not sharing data with kernelmodule (halmodule.h):
struct BLOCK {
    BLOCKTYPE blocktype=G0;
    //! Gcode Cartesian xyz
    double X=0,Y=0,Z=0;
    //! Gcode Arc center pos & Feedrate
    double I=0,J=0,K=0,F=0;
    //! Gcode Euler xyz (x=u, y=v, z=w)
    double U=0, V=0, W=0;
    POINT start={0,0,0}, center={0,0,0}, end={0,0,0};
    EULER euler_start={0,0,0}, euler_end={0,0,0};
    //! calculated results:
    double blocklenght=0;
};

struct GCODE {
    std::vector<BLOCK> blockvec;
    //! The current toolcenterpoint where the gcode acutally is interpolated at the moment.
    POINT tcp;
    std::string gcodefilename;
    POINT offset;
    EULER rotation;
};
extern GCODE gcode;

#endif // EXTERNAL_VAR_H
