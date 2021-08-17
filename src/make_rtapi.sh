#!/bin/bash

#Compiling rtapi/uspace_rtapi_app.cc
g++ -c -I. -Irtapi -Ihal -DULAPI  -g -Wall -Os -fwrapv -Woverloaded-virtual -g -O2   -Wno-psabi  -std=gnu++11 -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/uspace_rtapi_app.d" -MT "objects/rtapi/uspace_rtapi_app.o" \
        rtapi/uspace_rtapi_app.cc -o objects/rtapi/uspace_rtapi_app.o

#Compiling rtapi/uspace_rtapi_parport.cc
g++ -c -I. -Irtapi -Ihal -DULAPI  -g -Wall -Os -fwrapv -Woverloaded-virtual -g -O2   -Wno-psabi  -std=gnu++11 -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/uspace_rtapi_parport.d" -MT "objects/rtapi/uspace_rtapi_parport.o" \
        rtapi/uspace_rtapi_parport.cc -o objects/rtapi/uspace_rtapi_parport.o
        
#Compiling rtapi/uspace_rtapi_string.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv  -g -Wall  -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2  -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/uspace_rtapi_string.d" -MT "objects/rtapi/uspace_rtapi_string.o" \
        rtapi/uspace_rtapi_string.c -o objects/rtapi/uspace_rtapi_string.o
        
#Compiling rtapi/rtapi_pci.cc
g++ -c -I. -Irtapi -Ihal -DULAPI  -g -Wall -Os -fwrapv -Woverloaded-virtual -g -O2   -Wno-psabi  -std=gnu++11  -O0 -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/rtapi_pci.d" -MT "objects/rtapi/rtapi_pci.o" \
        rtapi/rtapi_pci.cc -o objects/rtapi/rtapi_pci.o
        
#Linking rtapi_app
g++ -rdynamic -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib   -o ../bin/rtapi_app objects/rtapi/uspace_rtapi_app.o objects/rtapi/uspace_rtapi_parport.o objects/rtapi/uspace_rtapi_string.o objects/rtapi/rtapi_pci.o -ldl  -pthread -lrt -ludev -ldl

#Compiling rtapi/uspace_ulapi.c
gcc -c -I. -Irtapi -Ihal -I/opt/hal-core/src/include -I/usr/include/python3.9 -Os -fwrapv  -g -Wall  -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2  -fPIC  \
        -MP -MD -MF "objects/rtapi/uspace_ulapi.d" -MT "objects/rtapi/uspace_ulapi.o" \
        rtapi/uspace_ulapi.c -o objects/rtapi/uspace_ulapi.o
        
#Creating shared library libhalcore.so.0
gcc -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib   -Wl,-soname,libhalcore.so.0 -shared -o ../lib/libhalcore.so.0 objects/hal/hal_lib.o objects/rtapi/uspace_ulapi.o -pthread -lrt 

#Linking halcmd
gcc -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib   -o ../bin/halcmd objects/hal/utils/halcmd.o objects/hal/utils/halcmd_commands.o objects/hal/utils/halcmd_main.o objects/hal/utils/halcmd_completion.o ../lib/libhalcore.so.0 -lreadline

#You now need to run 'sudo make setuid' in order to run in place with access to hardware.
sudo make setuid
