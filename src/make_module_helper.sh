#!/bin/bash

#Compiling module_helper/module_helper.c
gcc -c -I. -Irtapi -Ihal -I/opt/hal-core/src/include -I/usr/include/python3.9 -Os -fwrapv  -g -Wall  -DULAPI -std=gnu99 -fgnu89-inline -Werror=implicit-function-declaration -g -O2  -Wall -Werror  \
        -MP -MD -MF "objects/module_helper/module_helper.d" -MT "objects/module_helper/module_helper.o" \
        module_helper/module_helper.c -o objects/module_helper/module_helper.o
#Linking linuxcnc_module_helper

# Translate this smoke curtain shit : @$(CC) -o $@ $^

# The actual rules are a little more complicated since we like to use
# make's shorthand variables. These are:

# $@	for the target
# $<	for the first dependency
# $^	for all the dependencies

gcc -o ../bin/linuxcnc_module_helper objects/module_helper/module_helper.o -Wall
