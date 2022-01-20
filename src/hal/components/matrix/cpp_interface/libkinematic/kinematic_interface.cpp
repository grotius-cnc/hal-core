#include "kinematic_interface.h"
#include "stdio.h"

kinematic_interface::kinematic_interface()
{

}

DATA kinematic_interface::fk(DATA *ptr){ 
    for(unsigned int i=0; i<ptr->machines; i++){
        KDL::Chain KDLChain;
        KDL::JntArray KDLJointCur, KDLJointMin, KDLJointMax;
        KDL::Frame KDLCart;

        KDLJointCur.resize(ptr->machine[i].joints);
        KDLJointMin.resize(ptr->machine[i].joints);
        KDLJointMax.resize(ptr->machine[i].joints);

        for(unsigned int j=0; j<ptr->machine[i].joints; j++){

            KDL::Joint::JointType type;
            if(ptr->machine[i].joint[j].rotationtype==0){ //! Rotx
                type=KDL::Joint::JointType::RotX;
            }
            if(ptr->machine[i].joint[j].rotationtype==1){ //! Roty
                type=KDL::Joint::JointType::RotY;
            }
            if(ptr->machine[i].joint[j].rotationtype==2){ //! Rotz
                type=KDL::Joint::JointType::RotZ;
            }
            if(ptr->machine[i].joint[j].rotationtype==3){ //! Transx
                type=KDL::Joint::JointType::TransX;
            }
            if(ptr->machine[i].joint[j].rotationtype==4){ //! Transy
                type=KDL::Joint::JointType::TransY;
            }
            if(ptr->machine[i].joint[j].rotationtype==5){ //! Transz
                type=KDL::Joint::JointType::TransZ;
            }
            if(ptr->machine[i].joint[j].rotationtype==6){ //! Rotaxis
                type=KDL::Joint::JointType::RotAxis;
            }
            if(ptr->machine[i].joint[j].rotationtype==7){ //! Transaxis
                type=KDL::Joint::JointType::TransAxis;
            }
            if(ptr->machine[i].joint[j].rotationtype==8){ //! None
                type=KDL::Joint::JointType::None;
            }

            std::string str="J";
            str+=std::to_string(j);
            KDLChain.addSegment(KDL::Segment(str,KDL::Joint(type), KDL::Frame(KDL::Vector(ptr->machine[i].joint[j].axis_endpoint.x,
                                                                                          ptr->machine[i].joint[j].axis_endpoint.y,
                                                                                          ptr->machine[i].joint[j].axis_endpoint.z))));
            KDLJointCur(j)=ptr->machine[i].joint[j].curpos_abs;
        }

        KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);

        //! Forward kinematics -1, result is machine tcp.
        fksolver.JntToCart(KDLJointCur,KDLCart,-1);
        ptr->machine[i].abs_cart.x=KDLCart.p.x();
        ptr->machine[i].abs_cart.y=KDLCart.p.y();
        ptr->machine[i].abs_cart.z=KDLCart.p.z();
        KDLCart.M.GetEulerZYX(ptr->machine[i].abs_euler.z,ptr->machine[i].abs_euler.y,ptr->machine[i].abs_euler.x);

        //! Fill in the joint 4x4 matrix for every machine joint.
        for(unsigned int s=0; s<ptr->machine[i].joints; s++){

            //! Mention s+1.
            fksolver.JntToCart(KDLJointCur,KDLCart,s+1);
            //! Copy 4x4 matrix.
            for (int l=0; l<4; l++){
                for(int k=0; k<4; k++){
                    ptr->machine[i].joint[s].matrix[l][k]=KDLCart.operator()(l,k);

                }
            }
        }
    }
    return *ptr;
}

extern "C" DATA fk(DATA *ptr){
    DATA ptr_result=kinematic_interface().fk(ptr);
    return ptr_result;
}

DATA kinematic_interface::ik(DATA *ptr){ 
    for(unsigned int i=0; i<ptr->machines; i++){
        KDL::Chain KDLChain;
        KDL::JntArray KDLJointCur, KDLJointMin, KDLJointMax;
        KDL::Frame KDLCart;

        KDLJointCur.resize(ptr->machine[i].joints);
        KDLJointMin.resize(ptr->machine[i].joints);
        KDLJointMax.resize(ptr->machine[i].joints);

        for(unsigned int j=0; j<ptr->machine[i].joints; j++){

            KDL::Joint::JointType type;
            if(ptr->machine[i].joint[j].rotationtype==0){ //! Rotx
                type=KDL::Joint::JointType::RotX;
            }
            if(ptr->machine[i].joint[j].rotationtype==1){ //! Roty
                type=KDL::Joint::JointType::RotY;
            }
            if(ptr->machine[i].joint[j].rotationtype==2){ //! Rotz
                type=KDL::Joint::JointType::RotZ;
            }
            if(ptr->machine[i].joint[j].rotationtype==3){ //! Transx
                type=KDL::Joint::JointType::TransX;
            }
            if(ptr->machine[i].joint[j].rotationtype==4){ //! Transy
                type=KDL::Joint::JointType::TransY;
            }
            if(ptr->machine[i].joint[j].rotationtype==5){ //! Transz
                type=KDL::Joint::JointType::TransZ;
            }
            if(ptr->machine[i].joint[j].rotationtype==6){ //! Rotaxis
                type=KDL::Joint::JointType::RotAxis;
            }
            if(ptr->machine[i].joint[j].rotationtype==7){ //! Transaxis
                type=KDL::Joint::JointType::TransAxis;
            }
            if(ptr->machine[i].joint[j].rotationtype==8){ //! None
                type=KDL::Joint::JointType::None;
            }

            std::string str="J";
            str+=std::to_string(j);
            KDLChain.addSegment(KDL::Segment(str,KDL::Joint(type), KDL::Frame(KDL::Vector(ptr->machine[i].joint[j].axis_endpoint.x,
                                                                                          ptr->machine[i].joint[j].axis_endpoint.y,
                                                                                          ptr->machine[i].joint[j].axis_endpoint.z))));
        }

        //! Current joint values
        for(unsigned int j=0; j<ptr->machine[i].joints; j++){
            KDLJointCur(j)=ptr->machine[i].joint[j].curpos_abs;
            KDLJointMin(j)=ptr->machine[i].joint[j].min;
            KDLJointMax(j)=ptr->machine[i].joint[j].max;
        }

        KDLCart.p.x(ptr->machine[i].abs_cart.x);
        KDLCart.p.y(ptr->machine[i].abs_cart.y);
        KDLCart.p.z(ptr->machine[i].abs_cart.z);
        KDLCart.M.DoRotZ(ptr->machine[i].abs_euler.z);
        KDLCart.M.DoRotY(ptr->machine[i].abs_euler.y);
        KDLCart.M.DoRotX(ptr->machine[i].abs_euler.x);

        KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);

        KDL::ChainIkSolverVel_pinv iksolverv(KDLChain); //Inverse velocity solver needed for IK
        KDL::ChainIkSolverPos_NR_JL iksolver(KDLChain, KDLJointMin, KDLJointMax, fksolver, iksolverv, ptr->iterations, 1e-6); //max 100 iterations, stop at accuracy 1e-6
        KDL::JntArray JntResult(KDLChain.getNrOfJoints());

        int status=0;
        status = iksolver.CartToJnt(KDLJointCur, KDLCart, JntResult);

        if(status>=0){
            for(unsigned int j=0; j<ptr->machine[i].joints; j++){

                ptr->machine[i].joint[j].curpos_abs=JntResult(j);

                //! Mention s+1.
                fksolver.JntToCart(KDLJointCur,KDLCart,j+1);
                //! Copy 4x4 matrix.
                for (int l=0; l<4; l++){
                    for(int k=0; k<4; k++){
                        ptr->machine[i].joint[j].matrix[l][k]=KDLCart.operator()(l,k);
                    }
                }
            }
        } else {
            //                for(int j=0; j<joints[i]; j++){
            //                    ptr[i].joint[j].curpos=KDLJointRestore(j);
            //                }
            //                for(int j=0; j<6; j++){
            //                    e_ptr[i].curpos_cart_euler[j]=e_ptr[i].curpos_cart_euler_restore[j];
            //                }
            //std::cout<<"ik error"<<std::endl;
        }
    }
    return *ptr;
}

extern "C" DATA ik(DATA *ptr){
    DATA ptr_result=kinematic_interface().ik(ptr);
    return ptr_result;
}
















