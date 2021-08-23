#include "cpp_interface.h"

Cpp_interface::Cpp_interface()
{
    // This is performed every ms
}

result Cpp_interface::dofs(result input){

    // Stop sequence.
    if(input.maxvel==0){
        input.maxvel=0.0001;
        if(input.curvel<0.001 && input.curvel>0.000){
            input.curvel=0;
        }
    }

    // Ruckig input values.
    in.max_velocity[0]=abs(input.maxvel);
    in.max_acceleration[0]=input.maxacc;
    in.max_jerk[0]=input.maxjerk;
    in.current_position[0]=input.curpos;
    in.current_velocity[0]=input.curvel;
    in.current_acceleration[0]=input.curacc;
    in.target_velocity[0]=0;
    in.target_acceleration[0]=0;
    in.target_position[0]=input.tarpos; //pos.at(i)+dx;

    if(input.interfacetype==interface::position){
        in.interface=ruckig::Interface::Position;
    }
    if(input.interfacetype==interface::velocity){
        in.interface=ruckig::Interface::Velocity;
    }

    // Optional inputs
    if(input.synchronizationtype==synchronization::None){
        in.synchronization=ruckig::Synchronization::None;
    }
    if(input.synchronizationtype==synchronization::Phase){
        in.synchronization=ruckig::Synchronization::Phase;
    }
    if(input.synchronizationtype==synchronization::Time){
        in.synchronization=ruckig::Synchronization::Time;
    }
    if(input.synchronizationtype==synchronization::TimeIfNecessary){
        in.synchronization=ruckig::Synchronization::TimeIfNecessary;
    }

    if(input.durationdiscretizationtype==durationdiscretization::Continuous){
        in.duration_discretization=ruckig::DurationDiscretization::Continuous;
    }
    if(input.durationdiscretizationtype==durationdiscretization::Discrete){
        in.duration_discretization=ruckig::DurationDiscretization::Discrete;
    }

    // Enable - Disable
    in.enabled[0]=input.enable;

    auto result_x = otg.update(in, out);
    result_x = ruckig::Result::Working;

    // One ms forward.
    out.trajectory.at_time(0.001, pos, vel, acc);

    input.curpos=pos[0];
    input.curvel=vel[0];
    input.curacc=acc[0];

    // std::cout<<"pos:"<<input.curpos<<std::endl;

    return input;
}

extern "C" result wrapper_get_pos_array(result input){
    result r=Cpp_interface().dofs(input);
    return r;
}





















