#ifndef CPP_INTERFACE_H
#define CPP_INTERFACE_H

#include "cpp_interface_global.h"
#include <iostream>
#include <vector>
#include <fstream>

//! https://github.com/pantor/ruckig
//! A online-offline trajectory planner.
#include "libruckig/ruckig.hpp"

//! Filedialog library.
#include <libdialog/portable-file-dialogs.h>
#if _WIN32
#define DEFAULT_PATH "C:\\"
#else
#define DEFAULT_PATH "/tmp"
#endif

//! Gcode reader library.
#include <libgcoderead/parser.h>

#include <libskynet/directory.h>

//! A struct that represent a 3d point.
struct POINT {
    double x,y,z;
};

//! A struct that represents a direction vector.
struct EULER{
    double euler_x, euler_y, euler_z;
};

struct TCP {
    POINT p;
    EULER e;
    double vel,acc,jerk,pos;
    bool m3,m7,m8;
    double power_rpm;
};
// TCP tcp;

TCP tcp;

class CPP_INTERFACE_EXPORT Cpp_interface
{
public:
    Cpp_interface();

    void load_gcode(char* name);
    void trajectory_auto(double velocity_override,
                        bool start, bool stop,
                        double vel, double acc, double jerk, unsigned int startfromblock, bool run_from_line);

    // Manual movements
    void trajectory_jog(int jog_x, int jog_y, int jog_z,
                       double vel, double acc, double jerk);

    // This is a backend of the trajectory_auto function.
    void trajectory_jog_auto(double x, double y, double z,
                                      double vel, double acc, double jerk);

private:
    //! auto mode

};

#endif // CPP_INTERFACE_H
