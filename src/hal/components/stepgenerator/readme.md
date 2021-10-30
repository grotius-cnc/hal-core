# stepgenerator component

# To compile
	$ ./make

# To run a test
	$ ./runtest

This is a 1 axis stepgenerator code example.

The stepgenerator is designed to follow a 1ms servo-thread "motion control" position, for example a "Ruckig dofs 1ms s-curve position output".



To perform outputs every base-thread cycle, or delay a few cycle's to increase steplen and stepspace.
Wait 1 cycle, then set stepper pulse output high or low.

        $ halcmd setp stepgenerator.cycle_delay 1


Example travel 5mm in 1ms (servo-thread).

The motor teeth gear travels 40mm at 1 revolution.

The stepdriver setting: 5000 pulse/rev.

Internal driver calculation:
5000 pulse/(lineair travel 40mm a revolution) = 125 pulses/mm

Set the motor gear settings by :

        $ halcmd setp stepgenerator.pulses_mm 125
	
Perform a new motor position by :	
 
        $ /opt/hal-core/bin/./halcmd setp stepgenerator.pos_cmd 150
 	
To check the motor position, we have a pin that counts a seperate stepgenerator position.	

        stepgenerator.pos_fb
	
To open the project in Qt :

Start Qt, open file : qt-hal.creator

To compile with Qt, run the compile script from within qt.



