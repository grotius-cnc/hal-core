#!/bin/bash

./configure

mkdir -p objects/rthal
mkdir -p objects/rtapi
mkdir -p objects/module_helper
mkdir -p objects/hal/utils

# Compiling rtapi/uspace_rtapi_app.cc
g++ -c -I. -Irtapi -Ihal -DULAPI  -g -Wall -Os -fwrapv -Woverloaded-virtual -g -O2   -Wno-psabi  -std=gnu++11 -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/uspace_rtapi_app.d" -MT "objects/rtapi/uspace_rtapi_app.o" \
        rtapi/uspace_rtapi_app.cc -o objects/rtapi/uspace_rtapi_app.o
# Compiling rtapi/uspace_rtapi_parport.cc
g++ -c -I. -Irtapi -Ihal -DULAPI  -g -Wall -Os -fwrapv -Woverloaded-virtual -g -O2   -Wno-psabi  -std=gnu++11 -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/uspace_rtapi_parport.d" -MT "objects/rtapi/uspace_rtapi_parport.o" \
        rtapi/uspace_rtapi_parport.cc -o objects/rtapi/uspace_rtapi_parport.o
# Compiling rtapi/uspace_rtapi_string.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv  -g -Wall  -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2  -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/uspace_rtapi_string.d" -MT "objects/rtapi/uspace_rtapi_string.o" \
        rtapi/uspace_rtapi_string.c -o objects/rtapi/uspace_rtapi_string.o
# Compiling rtapi/rtapi_pci.cc
g++ -c -I. -Irtapi -Ihal -DULAPI  -g -Wall -Os -fwrapv -Woverloaded-virtual -g -O2   -Wno-psabi  -std=gnu++11  -O0 -DSIM -UULAPI -DRTAPI -pthread  \
        -MP -MD -MF "objects/rtapi/rtapi_pci.d" -MT "objects/rtapi/rtapi_pci.o" \
        rtapi/rtapi_pci.cc -o objects/rtapi/rtapi_pci.o
# Linking rtapi_app
g++ -rdynamic -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib -o ../bin/rtapi_app objects/rtapi/uspace_rtapi_app.o objects/rtapi/uspace_rtapi_parport.o objects/rtapi/uspace_rtapi_string.o objects/rtapi/rtapi_pci.o -ldl  -pthread -lrt -ludev -ldl

# Compiling hal/utils/halcmd.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv -g -Wall -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2 \
        -MP -MD -MF "objects/hal/utils/halcmd.d" -MT "objects/hal/utils/halcmd.o" \
        hal/utils/halcmd.c -o objects/hal/utils/halcmd.o
# Compiling hal/utils/halcmd_commands.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv -g -Wall -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2 \
        -MP -MD -MF "objects/hal/utils/halcmd_commands.d" -MT "objects/hal/utils/halcmd_commands.o" \
        hal/utils/halcmd_commands.c -o objects/hal/utils/halcmd_commands.o
# Compiling hal/utils/halcmd_main.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv -g -Wall -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2 \
        -MP -MD -MF "objects/hal/utils/halcmd_main.d" -MT "objects/hal/utils/halcmd_main.o" \
        hal/utils/halcmd_main.c -o objects/hal/utils/halcmd_main.o
# Compiling hal/utils/halcmd_completion.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv -g -Wall -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2 \
        -MP -MD -MF "objects/hal/utils/halcmd_completion.d" -MT "objects/hal/utils/halcmd_completion.o" \
        hal/utils/halcmd_completion.c -o objects/hal/utils/halcmd_completion.o
# Compiling hal/hal_lib.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv -g -Wall -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2 -fPIC \
        -MP -MD -MF "objects/hal/hal_lib.d" -MT "objects/hal/hal_lib.o" \
        hal/hal_lib.c -o objects/hal/hal_lib.o
# Compiling rtapi/uspace_ulapi.c
gcc -c -I. -Irtapi -Ihal -Os -fwrapv -g -Wall -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2 -fPIC \
        -MP -MD -MF "objects/rtapi/uspace_ulapi.d" -MT "objects/rtapi/uspace_ulapi.o" \
        rtapi/uspace_ulapi.c -o objects/rtapi/uspace_ulapi.o

# Creating shared library libhalcore.so.0
gcc -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib -Wl,-soname,libhalcore.so.0 -shared -o ../lib/libhalcore.so.0 objects/hal/hal_lib.o objects/rtapi/uspace_ulapi.o -pthread -lrt 
# Linking halcmd
gcc -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib -o ../bin/halcmd objects/hal/utils/halcmd.o objects/hal/utils/halcmd_commands.o objects/hal/utils/halcmd_main.o objects/hal/utils/halcmd_completion.o ../lib/libhalcore.so.0 -lreadline
ln -sf libhalcore.so.0 ../lib/libhalcore.so

# Compiling module_helper/module_helper.c
gcc -c -I. -Irtapi -Ihal  -Os -fwrapv  -g -Wall  -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2 -Wall -Werror \
        -MP -MD -MF "objects/module_helper/module_helper.d" -MT "objects/module_helper/module_helper.o" \
        module_helper/module_helper.c -o objects/module_helper/module_helper.o
# Linking linuxcnc_module_helper
gcc -o ../bin/module_helper objects/module_helper/module_helper.o -Wall

# Compiling realtime hal/hal_lib.c
gcc -c -Os -fwrapv  -g -Wall  -DRTAPI \
        -I. -I/opt/hal-core/src/include -DUSPACE -DRTAPI -D_GNU_SOURCE -Drealtime -D__MODULE__ -I/opt/hal-core/src/../include -I/opt/hal-core/src -I/opt/hal-core/src/libnml/linklist -I/opt/hal-core/src/libnml/cms -I/opt/hal-core/src/libnml/rcs -I/opt/hal-core/src/libnml/inifile -I/opt/hal-core/src/libnml/os_intf -I/opt/hal-core/src/libnml/nml -I/opt/hal-core/src/libnml/buffer -I/opt/hal-core/src/libnml/posemath -I/opt/hal-core/src/rtapi -I/opt/hal-core/src/hal -I/opt/hal-core/src/emc/nml_intf -I/opt/hal-core/src/emc/kinematics -I/opt/hal-core/src/emc/tp -I/opt/hal-core/src/emc/motion -DSEQUENTIAL_SUPPORT -DHAL_SUPPORT -DDYNAMIC_PLCSIZE -DRT_SUPPORT -DOLD_TIMERS_MONOS_SUPPORT -DMODBUS_IO_MASTER -fno-fast-math -fno-unsafe-math-optimizations -Wframe-larger-than=2560 -Wno-declaration-after-statement -fwrapv -fno-builtin-sin -fno-builtin-cos -fno-builtin-sincos -fPIC -Os \
        -MP -MD -MF "objects/rthal/hal_lib.d" -MT "objects/rthal/hal_lib.o" \
        hal/hal_lib.c -o objects/rthal/hal_lib.o
# Linking ../rtlib/hal_lib.so
ld -d -r -o objects/hal_lib.tmp objects/rthal/hal_lib.o
objcopy -j .rtapi_export -O binary objects/hal_lib.tmp objects/hal_lib.sym
(echo '{ global : '; tr -s '\0' < objects/hal_lib.sym | xargs -r0 printf '%s;\n' | grep .; echo 'local : * ; };') > objects/hal_lib.ver
gcc -shared -Bsymbolic -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib   -Wl,--version-script,objects/hal_lib.ver -o ../rtlib/hal_lib.so objects/rthal/hal_lib.o -lm

# You now need to run 'sudo make setuid' in order to run in place with access to hardware.
sudo make setuid
