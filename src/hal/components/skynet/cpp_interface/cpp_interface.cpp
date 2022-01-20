#include "cpp_interface.h"
#include "iostream"
#include <QApplication>
#include "thread"

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

//! Forward kinematics.
ruckig::Ruckig<1> otg_fk {0.001};
ruckig::InputParameter<1> in_fk;
ruckig::OutputParameter<1> out_fk;
std::array<double, 1> vel_fk, acc_fk, pos_fk;

//! Inverse kinematics, synchronizes a 3d motion cartxyz and eulerxyz.
ruckig::Ruckig<6> otg_ik {0.001};
ruckig::InputParameter<6> in_ik;
ruckig::OutputParameter<6> out_ik;
std::array<double, 6> vel_ik, acc_ik, pos_ik;

Cpp_interface::Cpp_interface()
{
}

void Cpp_interface::gui_thread(){
    int argc=0;
    char *argv[]={};
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();
}

void Cpp_interface::start_gui(){
    Thread = new std::thread(&Cpp_interface::gui_thread,this);
    Thread->detach(); // Execute the thread independent from other stuff.
}

extern "C" void start_gui(){
    Cpp_interface().start_gui();
}

//! Perform a ruckig scurve motion for each joint.
//! If gcode is running, update the tarpositions.
MACHINE Cpp_interface::rtruckig(MACHINE *ptr){

    unsigned int machines=ptr->data.machines;
    int *joints=ptr->data.joints;

    for(unsigned int i=0; i<machines; i++){

        if(ptr[i].kinmode==KINMODE::ik_rapid && ptr[i].rt_motion){
            //! In this ik_ruckig function, the cart xyz or euler xyz is active.
            //! We are able to perform a 3d synchronized motion.

            //! Stop if in kinematic error situation. User can always drive the robot in fk mode.
            if(!ptr[i].error){

                //! 6 axis, cartxyz, eulerxyz
                for(int j=0; j<6; j++){
                    int id=j;
                    in_ik.enabled[id]=1;
                    if(j<3){
                        in_ik.max_velocity[id]=ptr[i].maxvel_cart;
                        in_ik.max_acceleration[id]=ptr[i].maxacc_cart;
                        in_ik.max_jerk[id]=ptr[i].jerk_cart;
                    } else {
                        in_ik.max_velocity[id]=ptr[i].maxvel_euler;
                        in_ik.max_acceleration[id]=ptr[i].maxacc_euler;
                        in_ik.max_jerk[id]=ptr[i].jerk_euler;
                    }
                    in_ik.current_velocity[id]=ptr[i].curvel_cart_euler[j];
                    in_ik.current_acceleration[id]=ptr[i].curacc_cart_euler[j];
                    in_ik.current_position[id]=ptr[i].curpos_cart_euler[j];
                    in_ik.target_position[id]=ptr[i].tarpos_cart_euler[j];

                    //! This values can be used when the gcode intepreter can predict this values by a look-ahead function.
                    in_ik.target_velocity[id]=0;
                    in_ik.target_acceleration[id]=0;
                }

                //! enum: position, velocity
                in_ik.control_interface=ruckig::ControlInterface::Position;
                //! Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
                //! Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
                //! TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
                //! None, ///< Calculate every DoF independently
                in_ik.synchronization=ruckig::Synchronization::Time;
                //! Continuous, ///< Every trajectory duration is allowed (Default)
                //! Discrete, ///< The trajectory duration must be a multiple of the control cycle
                in_ik.duration_discretization=ruckig::DurationDiscretization::Continuous;

                auto result_x = otg_ik.update(in_ik,out_ik);
                result_x = ruckig::Result::Working;

                //! One ms forward.
                out_ik.trajectory.at_time(0.001,pos_ik, vel_ik, acc_ik);

                for(int j=0; j<6; j++){
                    int id=j;
                    //! Update hal.
                    ptr[i].curpos_cart_euler[j]=pos_ik[id];
                    ptr[i].curvel_cart_euler[j]=vel_ik[id];
                    ptr[i].curacc_cart_euler[j]=acc_ik[id];
                }
            }
        }
    }

    for(unsigned int i=0; i<machines; i++){
        if(ptr[i].kinmode==KINMODE::fk_rapid && ptr[i].rt_motion){
            for(int j=0; j<joints[i]; j++){
                int id=0;
                in_fk.enabled[id]=1;
                in_fk.max_velocity[id]=ptr[i].joint[j].maxvel;
                in_fk.max_acceleration[id]=ptr[i].joint[j].maxacc;
                in_fk.max_jerk[id]=ptr[i].joint[j].jerk;
                //! enum: position, velocity
                in_fk.control_interface=ruckig::ControlInterface::Position;
                //! Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
                //! Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
                //! TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
                //! None, ///< Calculate every DoF independently
                in_fk.synchronization=ruckig::Synchronization::None;
                //! Continuous, ///< Every trajectory duration is allowed (Default)
                //! Discrete, ///< The trajectory duration must be a multiple of the control cycle
                in_fk.duration_discretization=ruckig::DurationDiscretization::Continuous;

                in_fk.current_position[id]=ptr[i].joint[j].curpos;
                in_fk.current_velocity[id]=ptr[i].joint[j].curvel;
                in_fk.current_acceleration[id]=ptr[i].joint[j].curacc;

                in_fk.target_position[id]=ptr[i].joint[j].tarpos;
                //! This values can be used when the gcode intepreter can predict this values by a look-ahead function.
                in_fk.target_velocity[id]=0;
                in_fk.target_acceleration[id]=0;

                auto result_x = otg_fk.update(in_fk,out_fk);
                if(result_x == ruckig::Result::Finished){
                    e_ptr[i].ruckig_finished=true;
                } else {
                    e_ptr[i].ruckig_finished=false;
                }

                //! One ms forward.
                out_fk.trajectory.at_time(0.001,pos_fk, vel_fk, acc_fk);

                //! Update hal.
                ptr[i].joint[j].curpos=pos_fk[id];
                ptr[i].joint[j].curvel=vel_fk[id];
                ptr[i].joint[j].curacc=acc_fk[id];

            }
        }
    }
    return *ptr;
}

extern "C" MACHINE rtruckig(struct MACHINE *ptr){
    MACHINE result_ptr=Cpp_interface().rtruckig(ptr);
    return result_ptr;
}

//! All angles input and output unit: radians.
//! *ptr is a c style dynamic structure.
//! machines is the nr of machines results in nr of kinematic chains.
//! joints[x] is a joint array for 20 machines, each machine has a x nr of joints.
MACHINE Cpp_interface::forward_kinematic(MACHINE *ptr){

    unsigned int machines=ptr->data.machines;
    int *joints=ptr->data.joints;

    for(unsigned int i=0; i<machines; i++){

        if(ptr[i].kinmode==KINMODE::fk_rapid || ptr[i].kinmode==KINMODE::fk_rapid_tooldir){

            //! For every machine we need :
            KDL::Chain KDLChain;
            KDL::JntArray KDLJointInit;
            KDL::JntArray KDLJointCur;
            KDL::JntArray KDLJointMin;
            KDL::JntArray KDLJointMax;
            KDL::Frame KDLCart;

            for(int j=0; j<joints[i]; j++){
                //! Setup the chain :
                std::string name="M"+std::to_string(i)+"-J"+std::to_string(j);

                //! Enum joint type.
                //! # rotationtype : rot_x = 0, rot_y = 1, rot_z = 2, trans_x = 3, trans_y = 4, trans_z = 5, rot_axis = 6, trans_axis =7, none = 8
                KDL::Joint::JointType type;

                if(ptr[i].joint[j].rotationtype==0){
                    type=KDL::Joint::RotX;
                }
                if(ptr[i].joint[j].rotationtype==1){
                    type=KDL::Joint::RotY;
                }
                if(ptr[i].joint[j].rotationtype==2){
                    type=KDL::Joint::RotZ;
                }
                if(ptr[i].joint[j].rotationtype==3){
                    type=KDL::Joint::TransX;
                }
                if(ptr[i].joint[j].rotationtype==4){
                    type=KDL::Joint::TransY;
                }
                if(ptr[i].joint[j].rotationtype==5){
                    type=KDL::Joint::TransZ;
                }
                if(ptr[i].joint[j].rotationtype==6){
                    type=KDL::Joint::RotAxis;
                }
                if(ptr[i].joint[j].rotationtype==7){
                    type=KDL::Joint::TransAxis;
                }
                if(ptr[i].joint[j].rotationtype==8){
                    type=KDL::Joint::None;
                }
                //! Importand. This is a workaround to give kdl the correct hal values.
                //! For the opencascade we use the .hal file. For kdl we read in the same file, but for the axis endpoints, we need to shift one joint up.
                if(j<joints[i]-1){
                    KDLChain.addSegment(KDL::Segment(name,KDL::Joint(type), KDL::Frame(KDL::Vector(ptr[i].joint[j+1].axis_endpoint.x, ptr[i].joint[j+1].axis_endpoint.y, ptr[i].joint[j+1].axis_endpoint.z))));
                }
                if(j==joints[i]-1){
                    if(ptr[i].kinmode==KINMODE::fk_rapid_tooldir){
                        //! Here we add the tooldir values to extend the tcp.
                        KDLChain.addSegment(KDL::Segment(name,KDL::Joint(type), KDL::Frame(KDL::Vector(ptr[i].tooldir_cart.x,ptr[i].tooldir_cart.y,ptr[i].tooldir_cart.z))));
                    }
                    if(ptr[i].kinmode==KINMODE::fk_rapid){
                        KDLChain.addSegment(KDL::Segment(name,KDL::Joint(type), KDL::Frame(KDL::Vector(0,0,0))));
                    }
                }
            }

            KDLJointMin.resize(KDLChain.getNrOfSegments());
            KDLJointMax.resize(KDLChain.getNrOfSegments());
            KDLJointCur.resize(KDLChain.getNrOfSegments());
            KDLJointInit.resize(KDLChain.getNrOfSegments());

            //! Current joint values
            for(int j=0; j<joints[i]; j++){
                KDLJointInit(j)= ptr[i].joint[j].init; //! Unit: radians
                KDLJointCur(j)= ptr[i].joint[j].curpos;
                KDLJointMin(j)= ptr[i].joint[j].min;
                KDLJointMax(j)= ptr[i].joint[j].max;
            }

            //! Perform a forward kinematic calculation (fk).
            KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);
            int status = fksolver.JntToCart(KDLJointCur,KDLCart,-1);

            if(status==0 || status>0){


                if(ptr[i].rt_motion && ptr[i].kinmode==KINMODE::fk_rapid_tooldir){
                    ptr[i].tarpos_cart_euler[0]=KDLCart.p.x();
                    ptr[i].tarpos_cart_euler[1]=KDLCart.p.y();
                    ptr[i].tarpos_cart_euler[2]=KDLCart.p.z();
                } else {
                    ptr[i].curpos_cart_euler[0]=KDLCart.p.x();
                    ptr[i].curpos_cart_euler[1]=KDLCart.p.y();
                    ptr[i].curpos_cart_euler[2]=KDLCart.p.z();
                }

                if(ptr[i].kinmode==KINMODE::fk_rapid){
                    KDLCart.M.GetEulerZYX(ptr[i].curpos_cart_euler[5],ptr[i].curpos_cart_euler[4],ptr[i].curpos_cart_euler[3]);
                }

                ptr[i].error=0;
            } else {
                ptr[i].error=1;
            }

            if(ptr[i].kinmode==KINMODE::fk_rapid_tooldir){
                // Switch to ik_rapid mode.
                ptr[i].kinmode=KINMODE::ik_rapid;
            }
        }
    }

    return *ptr;
}

extern "C" MACHINE forward_kinematic(MACHINE *ptr){
    MACHINE result_ptr=Cpp_interface().forward_kinematic(ptr);
    return result_ptr;
}

//! *ptr is a c style dynamic structure.
//! machines is the nr of machines results in nr of kinematic chains.
//! joints[x] is a joint array for 20 machines, each machine has a x nr of joints.
MACHINE Cpp_interface::inverse_kinematic(MACHINE *ptr){

    /* For info, original example.
    //! Setup Kdl chain following the attached document : Kuka Kr 6 10 angilus.pdf
    KDLChain.addSegment(KDL::Segment("J0",KDL::Joint(KDL::Joint::RotZ), KDL::Frame(KDL::Vector(25.0,0.0,400.0))));      //0.0 to J2
    KDLChain.addSegment(KDL::Segment("J1",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,455.0))));       //J2 to J3
    KDLChain.addSegment(KDL::Segment("J2",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,35.0))));        //J3 to J5
    KDLChain.addSegment(KDL::Segment("J3",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(420.0,0.0,0.0))));       //J4
    KDLChain.addSegment(KDL::Segment("J4",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(80.0,0.0,0.0))));        //J5 to end-effector (robot flange axis 6)
    KDLChain.addSegment(KDL::Segment("J5",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(105.0,0.0,0.0))));         //Tool cone lenght.
    */

    unsigned int machines=ptr->data.machines;
    int *joints=ptr->data.joints;

    for(unsigned int i=0; i<machines; i++){

        if(ptr[i].kinmode==KINMODE::ik_rapid || ptr[i].kinmode==KINMODE::ik_rapid_tooldir){

            //! For every machine we need :
            KDL::Chain KDLChain;
            KDL::JntArray KDLJointInit;
            KDL::JntArray KDLJointCur;
            KDL::JntArray KDLJointMin;
            KDL::JntArray KDLJointMax;
            KDL::JntArray KDLJointRestore;
            KDL::Frame KDLCart;

            for(int j=0; j<joints[i]; j++){

                //! Setup the chain :
                std::string name="M"+std::to_string(i)+"-J"+std::to_string(j);

                //! Enum joint type.
                //! # rotationtype : rot_x = 0, rot_y = 1, rot_z = 2, trans_x = 3, trans_y = 4, trans_z = 5, rot_axis = 6, trans_axis =7, none = 8
                KDL::Joint::JointType type;
                if(ptr[i].joint[j].rotationtype==0){
                    type=KDL::Joint::RotX;
                }
                if(ptr[i].joint[j].rotationtype==1){
                    type=KDL::Joint::RotY;
                }
                if(ptr[i].joint[j].rotationtype==2){
                    type=KDL::Joint::RotZ;
                }
                if(ptr[i].joint[j].rotationtype==3){
                    type=KDL::Joint::TransX;
                }
                if(ptr[i].joint[j].rotationtype==4){
                    type=KDL::Joint::TransY;
                }
                if(ptr[i].joint[j].rotationtype==5){
                    type=KDL::Joint::TransZ;
                }
                if(ptr[i].joint[j].rotationtype==6){
                    type=KDL::Joint::RotAxis;
                }
                if(ptr[i].joint[j].rotationtype==7){
                    type=KDL::Joint::TransAxis;
                }
                if(ptr[i].joint[j].rotationtype==8){
                    type=KDL::Joint::None;
                }

                //! Importand. This is a workaround to give kdl the correct hal values.
                //! For the opencascade we use the .hal file. For kdl we read in the same file, but for the axis endpoints, we need to shift one joint up.
                if(j<joints[i]-1){
                    KDLChain.addSegment(KDL::Segment(name,KDL::Joint(type), KDL::Frame(KDL::Vector(ptr[i].joint[j+1].axis_endpoint.x, ptr[i].joint[j+1].axis_endpoint.y, ptr[i].joint[j+1].axis_endpoint.z))));
                }
                if(j==joints[i]-1){
                    KDLChain.addSegment(KDL::Segment(name,KDL::Joint(type), KDL::Frame(KDL::Vector(0,0,0))));
                }
            }

            KDLJointMin.resize(KDLChain.getNrOfSegments());
            KDLJointMax.resize(KDLChain.getNrOfSegments());
            KDLJointCur.resize(KDLChain.getNrOfSegments());
            KDLJointInit.resize(KDLChain.getNrOfSegments());
            KDLJointRestore.resize(KDLChain.getNrOfSegments());

            for(int j=0; j<joints[i]; j++){
                KDLJointInit(j)= ptr[i].joint[j].init;
                KDLJointCur(j)=ptr[i].joint[j].curpos;
                KDLJointRestore(j)=ptr[i].joint[j].curpos;
                KDLJointMin(j)= ptr[i].joint[j].min;
                KDLJointMax(j)= ptr[i].joint[j].max;
            }

            KDLCart.p.x(ptr[i].curpos_cart_euler[0]);
            KDLCart.p.y(ptr[i].curpos_cart_euler[1]);
            KDLCart.p.z(ptr[i].curpos_cart_euler[2]);
            KDLCart.M.DoRotZ(ptr[i].curpos_cart_euler[5]-KDLCart.M.GetRot().z());
            KDLCart.M.DoRotY(ptr[i].curpos_cart_euler[4]-KDLCart.M.GetRot().y());
            KDLCart.M.DoRotX(ptr[i].curpos_cart_euler[3]-KDLCart.M.GetRot().x());

            KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);

            KDL::ChainIkSolverVel_pinv iksolverv(KDLChain); //Inverse velocity solver needed for IK
            if(ptr[i].kinematic_iterations==0){ptr[i].kinematic_iterations=100;}
            KDL::ChainIkSolverPos_NR_JL iksolver(KDLChain, KDLJointMin, KDLJointMax, fksolver, iksolverv, ptr[i].kinematic_iterations, 1e-6); //max 100 iterations, stop at accuracy 1e-6
            KDL::JntArray JntResult(KDLChain.getNrOfJoints());

            int status=0;
            status = iksolver.CartToJnt(KDLJointCur, KDLCart, JntResult);

            if(status>=0){
                for(int j=0; j<joints[i]; j++){
                    ptr[i].joint[j].curpos=JntResult(j);
                }
                ptr[i].error=0;
            } else {
                //                for(int j=0; j<joints[i]; j++){
                //                    ptr[i].joint[j].curpos=KDLJointRestore(j);
                //                }
                //                for(int j=0; j<6; j++){
                //                    e_ptr[i].curpos_cart_euler[j]=e_ptr[i].curpos_cart_euler_restore[j];
                //                }
                std::cout<<"ik error"<<std::endl;
                ptr[i].error=1;
            }
        }
    }
    return *ptr;
}

extern "C" MACHINE inverse_kinematic(MACHINE *ptr){
    MACHINE result_ptr=Cpp_interface().inverse_kinematic(ptr);
    return result_ptr;
}














