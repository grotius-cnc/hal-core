#!/usr/bin/bash

cd scripts
. ./rip-environment
cd ..

cd bin

halcmd stop
halcmd loadrt threads name1=base-thread fp1=0 period1=25000 name2=servo-thread period2=1000000
halcmd loadrt test
halcmd addf the_function servo-thread
halcmd start

halcmd show

# To clean the hal environment :
# $ /opt/hal-core$ /opt/linuxcnc/scripts/halrun -U
