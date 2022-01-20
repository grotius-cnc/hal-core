#ifndef EXTERNAL_VAR_GCODE_H
#define EXTERNAL_VAR_GCODE_H

//! This file is a external variable file for the c++ side of the program.
//! It uses standard c++ library containers etc.

//! Opencascade
#include <AIS_Shape.hxx>
#include <gp_Quaternion.hxx>

#include "external_var.h"

//! Gcode rapid, line, arc.
//! Macro's. Add user defined macro's here and tell the gcode_interface what to do with it.
enum BLOCKTYPE {
    G0, G1, G2, G3,
    //! For example a macro can carry a value : M3 P1100
    M
};

//! Structs not sharing data with kernelmodule (halmodule.h):
//! Initialize values with 0!
struct BLOCK {
    enum BLOCKTYPE blocktype;
    //! Gcode Cartesian xyz
    double X=0,Y=0,Z=0;
    //! Gcode Euler xyz (x=a, y=b, z=c)
    double A=0,B=0,C=0;
    //! Optional
    double U=0,V=0,W=0;
    //! Gcode Arc center pos
    double I=0,J=0,K=0;
    //! Feedrate & Spindlespeed
    double F=0,S=0;
    //! Macro nr
    int Macro_nr=0;
    //! Macro carrying a value
    double P=0;
    struct POINT start, center, end;
    struct EULER euler_start, euler_end;
    //! current blocklenght of arc, line.
    double blocklenght=0;

    Handle(AIS_Shape) gcodeshape;

    //! The velocity optimizer function has a forward calculation and a backward calculation
    //! to ensure a optimal velocity when crossing waypoints.
    double vel_fwd_start=0, vel_fwd_end=0;
    double vel_bck_start=0, vel_bck_end=0;
    //! The result off above lowest values, to ensure optimal path velocity.
    double vel_start=0;
    double vel_end=0;
};

struct GCODE {
    //! Contains the gcode line data.
    std::vector<BLOCK> blockvec;
    //! Machine identification.
    unsigned int machine_id;
    //! The complete block stringvec to load in a textpreview.
    std::string fullblockstring;
    std::vector<std::string> blockstringvec;
};
//! For every machine instance :
//! gcodevec.at(0)=mdi          Machine_0
//! gcodevec.at(1)=program      Machine_0
//! gcodevec.at(2)=mdi          Machine_1
//! gcodevec.at(3)=program      Machine_1
//! etc.
extern std::vector<GCODE> gcodevec;

#endif // EXTERNAL_VAR_GCODE_H
