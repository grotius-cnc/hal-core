This is the ruckig DoFs s-curve motion library 
implementing a DoFs[x] synchronized motion in time.

All axis will synchronize the s-curve motion 
and will end together at the same time.

To garanty a 1ms tread use a maximum DoFs[3] configuration.

When using > DoFs[3], edit source code in cpp interface and recompile.

 1.ruckig::at_time 0.001 (1ms) to 0.002 (2ms) in cpp project.
 2.hal servo-thread time 1000.000 to 2000.000

