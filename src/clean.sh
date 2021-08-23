#!/usr/bin/bash

# Go one step back to be on hal-core directory
cd .. 

# Remove the following directories from bin
rm -rf bin/rtapi_app
rm -rf bin/linuxcnc_module_helper
rm -rf bin/halcmd

# Remove .so and .so.0 files from lib
rm -rf lib/*.so lib/*.so.0

# Remove .so files from rtlib
rm -rf rtlib/*.so 

# Remove .so files from tcl
rm -rf tcl/*.so

# Remove files and directories from src/objects/
rm -rf src/objects/*
