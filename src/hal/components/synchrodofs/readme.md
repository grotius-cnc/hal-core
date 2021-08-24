This is the ruckig DoFs s-curve motion library 
implementing a DoFs[x] synchronized motion in time.

All axis will synchronize the s-curve motion 
and will end together at the same time.

To garanty a 1ms tread use a maximum DoFs[3] configuration.

When using more then 3 DoFs, increase time in source at 2 places:

    1.ruckig::at_time 0.001 (1ms) 
    2.hal servo-thread time 1000000 

