This module is based on the "Ruckig s-curve library" and used to create s-curve motion for a cnc or robot axis.

The module can perform a motion for multiple independent DoFs, up to 16 axis are allowed including pulse stepgeneration up to 16 axis.

Dofs[x] s-curve motion parameters :

    dofs nr=[x]
      
    dofs.enable.[x]           1       # bool 0-1
    dofs.maxvel.[x]           250     # max velocity
    dofs.maxacc.[x]           150     # max acceleration
    dofs.maxjerk.[x]          10      # max jerk
    dofs.tarpos.[x]           1000    # targetposition

Dofs[x] stepgeneration parameters :

    dofs.stepgen.pulses_mm.[x] 125    # stepper motor pulses a 1 mm travel.
    dofs.stepgen.cycle_delay.[x] 0    # wait a [x] number of base-thread cycle's between a step pulse
    
    
Runtested with a Ethercat Beckhoff EL2124 5us pulse :

    dofs.stepgen.step.[x] lcec.0.output_stepper_1.dout-0
    dofs.stepgen.dir.[x] lcec.0.output_stepper_1.dout-1    
    dofs.stepgen.pulses_mm.[x] 125
    
Have Fun, and don't expect too much of onboard pc stepping. External pid controllers can increase performance a lot. 

My test result where done by loading a 4 axis Dofs with above parameters.
If your Dofs speed is to high for the stepper to follow, your stepper feedback position will differ from the actual Dofs position.
You could make a pin for this to monitor follow errors.

  
