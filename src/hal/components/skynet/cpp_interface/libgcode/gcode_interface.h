#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
#include <vector>
#include <fstream>
#include "math.h"
#include <libgcode/parser.h>
#include <external_var.h>

// Dialog file open library, https://github.com/samhocevar/portable-file-dialogs/blob/master/examples/example.cpp
#include <libdialog/portable-file-dialogs.h>
#if _WIN32
#define DEFAULT_PATH "C:\\"
#else
#define DEFAULT_PATH "/tmp"
#endif

//! LibRuckig scurve lib
//! https://github.com/pantor/ruckig, extra info: https://github.com/pantor/ruckig/issues/64
#include "ruckig/ruckig.hpp"

class gcode_interface
{
public:
    // https://github.com/dillonhuff/gpr/
    // https://www.cnccookbook.com/cnc-g-code-arc-circle-g02-g03/
    gcode_interface();
    GCODE load_gcode();

    //! Gcode runner
    MACHINE gcode_runner(MACHINE *ptr);

    //! Helper functions.
    BLOCK line_lenght(BLOCK b);
    BLOCK arc_lenght(BLOCK b);
    EULER interpolate_euler(BLOCK b, double distancetogo);
    POINT interpolate_line(BLOCK b, double distancetogo);
    POINT interpolate_arc(BLOCK b, double distancetogo);
};

#endif // GCODE_INTERFACE_H
