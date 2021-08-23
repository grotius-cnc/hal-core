#!/bin/bash

# Compile to .o file
gcc -o test.o -Os -g -I. -I/opt/hal-core/src/rtapi -I/opt/hal-core/src/hal -DUSPACE -fno-fast-math -fno-unsafe-math-optimizations -DRTAPI -D_GNU_SOURCE -Drealtime -D__MODULE__ -I/opt/hal-core/src/rtapi -I/opt/hal-core/src/hal -DSIM -fPIC -Os -c test.c

# Compile to .so file
gcc -shared -o test.so test.o  -Wl,-rpath,/opt/hal-core/lib -L/opt/hal-core/lib -lhalcore -lrt -I/opt/hal-core/src/rtapi/

# Install to the rtlib
cp -rf test.so /opt/hal-core/rtlib/

