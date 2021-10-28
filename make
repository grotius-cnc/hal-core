#!/bin/bash

# Give file permissions
chmod +x /opt/hal-core/runtest
chmod +x /opt/hal-core/src/clean
chmod +x /opt/hal-core/src/make
chmod +x /opt/hal-core/src/configure
chmod +x /opt/hal-core/scripts/halrun
chmod +x /opt/hal-core/scripts/realtime

# Compile hal-core
cd /opt/hal-core/src/
./configure --disable-gtk --with-realtime=uspace
./make

# Set user able to insert kernel modules
chown 777 -R /opt/hal-core/bin/rtapi_app
chown 777 -R /opt/hal-core/bin/module_helper
chmod 777 /opt/hal-core/bin/rtapi_app
chmod 777 /opt/hal-core/bin/module_helper 

# Compile test component: 
chmod +x /opt/hal-core/src/hal/components/test/make
chmod +x /opt/hal-core/src/hal/components/test/runtest
cd /opt/hal-core/src/hal/components/test/ && ./make

# Compile ethercat component: 
chmod +x /opt/hal-core/src/hal/components/ethercat/make 
chmod +x /opt/hal-core/src/hal/components/ethercat/runtest
cd /opt/hal-core/src/hal/components/ethercat && ./make

# Compile threads component: 
chmod +x /opt/hal-core/src/hal/components/threads/make 
chmod +x /opt/hal-core/src/hal/components/threads/runtest
cd /opt/hal-core/src/hal/components/threads/ && ./make

# Compile dofs component: 
chmod +x /opt/hal-core/src/hal/components/dofs/make
chmod +x /opt/hal-core/src/hal/components/dofs/runtest
cd /opt/hal-core/src/hal/components/dofs/ && ./make

# Compile hostmot2 component: 
chmod +x /opt/hal-core/src/hal/components/hostmot2/make
chmod +x /opt/hal-core/src/hal/components/hostmot2/runtest
cd /opt/hal-core/src/hal/components/hostmot2/ && ./make

# Compile hm2_eth component: 
chmod +x /opt/hal-core/src/hal/components/hm2_eth/make
chmod +x /opt/hal-core/src/hal/components/hm2_eth/runtest
cd /opt/hal-core/src/hal/components/hm2_eth/ && ./make


