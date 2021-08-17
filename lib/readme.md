In this area libhalcore.so is present.

This library is produced by :

Creating shared library libhalcore.so.0
gcc -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib   -Wl,-soname,libhalcore.so.0 -shared -o ../lib/libhalcore.so.0 objects/hal/hal_lib.o objects/rtapi/uspace_ulapi.o -pthread -lrt 
Linking halcmd
gcc -L/opt/hal-core/lib -Wl,-rpath,/opt/hal-core/lib   -o ../bin/halcmd objects/hal/utils/halcmd.o objects/hal/utils/halcmd_commands.o objects/hal/utils/halcmd_main.o objects/hal/utils/halcmd_completion.o ../lib/libhalcore.so.0 -lreadline
ln -sf libhalcore.so.0 ../lib/libhalcore.so
You now need to run 'sudo make setuid' in order to run in place with access to hardware.
make: Leaving directory '/opt/hal-core/src'
user@debian10:/opt/hal-core/src$ 

