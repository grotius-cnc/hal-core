#!/bin/bash

cd ../lib
ln -sf libhalcore.so.0 libhalcore.so
cd ../src
# Compiling realtime hal/hal_lib.c
gcc -c -Os -fwrapv  -g -Wall  -DRTAPI \
        -I. -I/opt/hal-core/src/include -DUSPACE -DRTAPI -D_GNU_SOURCE -Drealtime -D__MODULE__ -I/opt/hal-core/src -I/opt/hal-core/src/rtapi -I/opt/hal-core/src/hal \
        -DSEQUENTIAL_SUPPORT -DHAL_SUPPORT -DDYNAMIC_PLCSIZE -DRT_SUPPORT -DOLD_TIMERS_MONOS_SUPPORT -DMODBUS_IO_MASTER \
         -fno-fast-math -mieee-fp -fno-unsafe-math-optimizations -Wframe-larger-than=2560 -Wno-declaration-after-statement -fwrapv -fno-builtin-sin -fno-builtin-cos -fno-builtin-sincos -fPIC -Os \
        -MP -MD -MF "objects/rthal/hal_lib.d" -MT "objects/rthal/hal_lib.o" \
        hal/hal_lib.c -o objects/rthal/hal_lib.o
# Linking ../rtlib/hal_lib.so
ld -d -r -o objects/hal_lib.tmp objects/rthal/hal_lib.o
objcopy -j .rtapi_export -O binary objects/hal_lib.tmp objects/hal_lib.sym
(echo '{ global : '; tr -s '\0' < objects/hal_lib.sym | xargs -r0 printf '%s;\n' | grep .; echo 'local : * ; };') > objects/hal_lib.ver
gcc -shared -Bsymbolic -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib   -Wl,--version-script,objects/hal_lib.ver -o ../rtlib/hal_lib.so objects/rthal/hal_lib.o -lm
