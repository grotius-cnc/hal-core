#!/usr/bin/bash

cd ..

cd bin
rm -rf rtapi_app
rm -rf linuxcnc_module_helper
rm -rf halcmd
rm -rf halscope
cd ..

cd lib
rm -rf *.so *.so.0
cd ..

cd rtlib
rm -rf *.so 
cd ..

cd tcl
rm -rf *.so
cd ..

cd src/objects/
# remove files
rm -rf *.*
# remove dirs
rm -dr *

cd ..
cd ..
cd src



