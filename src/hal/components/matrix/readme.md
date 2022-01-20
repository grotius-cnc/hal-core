To compile the kernel module : 

    ~/hal/./make_halmodule

To compile the gui app : 

    ~/cpp_interface/make

To clean the gui app : 

    ~/cpp_interface/make clean

To run the app : 

    ~/hal/./runtest
    
Specs :

    1. Run a gcode forward or reversed during the full gcode file.
    2. Run a gcode file containing a 3d motion. cartesian xyz=XYZ euler xyz=ABC
    3. Run a multiline mdi command.
    4. Run the machine in manual mode, Offline will jump to target pos. Online will perform the scurve motion.
    5. Run multiple machines at the same time, each machine may load a gcode file.
    6. Run a gcode from a certain line nr. It will ask to get in position with a Mdi safe height move.
    7. Hal outputs for joint positions and basic Mcode's like m3,m5,m7,m8,m9.
    8. Load a different machine model, like a robot on a track or a gantry.
    9. Load a different kinematic model.
    10. Code your own user tab.
    11. It can rotate and translate the gcode workframe in any position.
    12. This is a realtime program, driven by a kernel module.
    
 Todo :
 
     1. Look ahead function.
    
![screen0](https://user-images.githubusercontent.com/44880102/150279993-4963d765-6a89-4d5e-8ff0-951183f059ef.jpg)
    
![halcore_picture](https://user-images.githubusercontent.com/44880102/150311471-a83901c7-d88b-457f-b202-64741f897e92.jpg)
