#include "cpp_interface.h"

Cpp_interface::Cpp_interface()
{
    // This is performed every ms
}

result *Cpp_interface::dofs(result input[]){

    // Add existing (dummy) values to unused DoFs.
    for(unsigned int i=0; i<7; i++){
         in.enabled[i]=1;
         in.max_velocity[i]=input[0].maxvel;
         in.max_acceleration[i]=input[0].maxacc;
         in.max_jerk[i]=input[0].maxjerk;
         in.interface=ruckig::Interface::Position;
         in.synchronization=ruckig::Synchronization::Time;
    }

    for(unsigned int i=0; i<input[0].size; i++){

        // Stop sequence.
        if(input[i].maxvel==0){
            input[i].maxvel=0.0001;
            if(input[i].curvel<0.001 && input[i].curvel>0.000){
                input[i].curvel=0;
            }
        }

        // Ruckig input values.
        in.max_velocity[i]=abs(input[i].maxvel);
        in.max_acceleration[i]=input[i].maxacc;
        in.max_jerk[i]=input[i].maxjerk;
        in.current_position[i]=input[i].curpos;
        in.current_velocity[i]=input[i].curvel;
        in.current_acceleration[i]=input[i].curacc;
        in.target_velocity[i]=0;
        in.target_acceleration[i]=0;
        in.target_position[i]=input[i].tarpos; //pos.at(i)+dx;

        in.interface=ruckig::Interface::Position;
        in.synchronization=ruckig::Synchronization::Time;

        // Enable - Disable
        in.enabled[i]=input[i].enable;
    }

    auto result_x = otg.update(in,out);
    result_x = ruckig::Result::Working;

    // One ms forward.
    out.trajectory.at_time(0.001,pos, vel, acc);

    for(unsigned int i=0; i<input[0].size; i++){
        input[i].curpos=pos[i];
        input[i].curvel=vel[i];
        input[i].curacc=acc[i];
    }

    return input;
}

extern "C" result wrapper_get_pos_array(result input_array[]){
    result* r=Cpp_interface().dofs(input_array);
    return *r;
}





















