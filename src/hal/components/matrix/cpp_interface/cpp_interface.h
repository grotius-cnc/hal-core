#ifndef CPP_INTERFACE_H
#define CPP_INTERFACE_H

//! Author : Skynet
//! Optimalized kinematic library implementation
//!  - Dynamic kinematic setup.
//!  - Setup multiple independent kinematic chains.

extern struct DATA *ptr;

#include "cpp_interface_global.h"
#include "external_var.h"

#include "mainwindow.h"

class CPP_INTERFACE_EXPORT Cpp_interface{
public:
    Cpp_interface();
    void start_gui();
    void gui_thread();
private:
    std::thread *Thread;
};

#endif // CPP_INTERFACE_H
