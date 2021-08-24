#!/bin/bash

# Give file permissions
chmod +x /opt/hal-core/src/clean
chmod +x /opt/hal-core/src/make
chmod +x /opt/hal-core/src/configure

# Compile hal-core
/opt/hal-core/src/./make

# Compile test component: 
chmod +x /opt/hal-core/src/hal/components/test/make
chmod +x /opt/hal-core/src/hal/components/test/runtest
/opt/hal-core/src/hal/components/test/./make

# Compile ethercat component: 
chmod +x /opt/hal-core/src/hal/components/ethercat/make 
chmod +x /opt/hal-core/src/hal/components/ethercat/runtest
/opt/hal-core/src/hal/components/ethercat./make

# Compile threads component: 
chmod +x /opt/hal-core/src/hal/components/threads/make 
chmod +x /opt/hal-core/src/hal/components/threads/runtest
/opt/hal-core/src/hal/components/threads/./make

# Compile dofs component: 
chmod +x /opt/hal-core/src/hal/components/dofs/make
/opt/hal-core/src/hal/components/dofs/./make

