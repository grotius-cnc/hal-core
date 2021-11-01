#include "kinematic.h"

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

//! Fk_tooldir is a temponairy frame on top of the machine base configuration. On top of end effector, tool adapter plate.
//! This frame is called when user want's to move, jog in tool direction.
//! It is used to get xyz coordinates when user want's to move in tool direction.
//!
int kinematic::Fk_tooldir(double x_in, double y_in, double z_in, double &x_out, double &y_out, double &z_out){

    KDL::JntArray Tooljoints;
    KDL::Frame Toolcart;
    KDL::Chain ToolChain;
    ToolChain.addChain(KDLChain);
    ToolChain.addSegment(KDL::Segment("Tooldir",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(x_in,y_in,z_in))));

    Tooljoints.resize(ToolChain.getNrOfSegments());
    Tooljoints(0)=KDLJointCur(0);
    Tooljoints(1)=KDLJointCur(1);
    Tooljoints(2)=KDLJointCur(2);
    Tooljoints(3)=KDLJointCur(3);
    Tooljoints(4)=KDLJointCur(4);
    Tooljoints(5)=KDLJointCur(5);
    Tooljoints(6)=0;

    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(ToolChain);
    int status = fksolver.JntToCart(Tooljoints,Toolcart,-1);

    if(status==0 || status>0){
        x_out=Toolcart.p.x();
        y_out=Toolcart.p.y();
        z_out=Toolcart.p.z();
        return 1;
    } else {
        std::cout<<fksolver.getError()<<std::endl;
        return 0;
    }
}

int kinematic::Init(){

    //! Setup Kdl chain following the attached document : Kuka Kr 6 10 angilus.pdf
    KDLChain.addSegment(KDL::Segment("J0",KDL::Joint(KDL::Joint::RotZ), KDL::Frame(KDL::Vector(25.0,0.0,400.0))));      //0.0 to J2
    KDLChain.addSegment(KDL::Segment("J1",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,455.0))));       //J2 to J3
    KDLChain.addSegment(KDL::Segment("J2",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,35.0))));        //J3 to J5
    KDLChain.addSegment(KDL::Segment("J3",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(420.0,0.0,0.0))));       //J4
    KDLChain.addSegment(KDL::Segment("J4",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(80.0,0.0,0.0))));        //J5 to end-effector (robot flange axis 6)
    KDLChain.addSegment(KDL::Segment("J5",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(105.0,0.0,0.0))));         //Tool cone lenght.

    // The floortrack is loaded outside the KDLChainVec size.

    KDLJointMin.resize(KDLChain.getNrOfSegments());
    KDLJointMax.resize(KDLChain.getNrOfSegments());
    KDLJointCur.resize(KDLChain.getNrOfSegments());
    KDLJointInit.resize(KDLChain.getNrOfSegments());

    KDLJointInit(0)= 0 * toRadians; //joint to radians
    KDLJointCur(0)= 0 * toRadians;
    KDLJointMin(0)= -170 * toRadians;
    KDLJointMax(0)= 170 * toRadians;

    KDLJointInit(1)= 0 * toRadians; //joint to radians
    KDLJointCur(1)= 0 * toRadians;
    KDLJointMin(1)= -100 * toRadians;
    KDLJointMax(1)= 135 * toRadians;

    KDLJointInit(2)= 0 * toRadians; //joint to radians
    KDLJointCur(2)= 0 * toRadians;
    KDLJointMin(2)= -210 * toRadians;
    KDLJointMax(2)= 66 * toRadians;

    KDLJointInit(3)= 0 * toRadians; //joint to radians
    KDLJointCur(3)= 0 * toRadians;
    KDLJointMin(3)= -185 * toRadians;
    KDLJointMax(3)= 185 * toRadians;

    KDLJointInit(4)= 0 * toRadians; //joint to radians
    KDLJointCur(4)= 0 * toRadians;
    KDLJointMin(4)= -120 * toRadians;
    KDLJointMax(4)= 120 * toRadians;

    KDLJointInit(5)= 0 * toRadians; //joint to radians
    KDLJointCur(5)= 0 * toRadians;
    KDLJointMin(5)= -350 * toRadians;
    KDLJointMax(5)= 350 * toRadians;

    //! Test a forward kinematic calculation:
    if(Fk()){
        std::cout << "Forward kinematics ok" << std::endl;
    } else {
        std::cout << "Forward kinematics error" << std::endl;
        return 0;
    }

    return 1;
}

int kinematic::Fk(){

    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);
    int status = fksolver.JntToCart(KDLJointCur,cart,-1);

    if(status==0 || status>0){
        return 1;
    } else {
        //std::cout<<fksolver.getError()<<std::endl;
        return 0;
    }
}

int kinematic::Fk_zero(){

    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);
    int status = fksolver.JntToCart(KDLJointInit,cartzero,-1);

    if(status==0 || status>0){
        return 1;
    } else {
        //std::cout<<fksolver.getError()<<std::endl;
        return 0;
    }
}

int kinematic::Ik(bool mode_ikfrominit){

    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);

    KDL::ChainIkSolverVel_pinv iksolverv(KDLChain); //Inverse velocity solver needed for IK
    KDL::ChainIkSolverPos_NR_JL iksolver(KDLChain, KDLJointMin, KDLJointMax, fksolver, iksolverv, 100, 1e-6); //max 100 iterations, stop at accuracy 1e-6

    KDL::JntArray JntResult(KDLChain.getNrOfJoints());

    int status=0;
    if(mode_ikfrominit){ //Perform Ik from Init.
        status = iksolver.CartToJnt(KDLJointInit, cart, JntResult);
    } else {
        status = iksolver.CartToJnt(KDLJointCur, cart, JntResult);
    }

    if(status>=0){
        KDLJointCur=JntResult;
        return 1;
    } else {
        //std::cout<<iksolver.getError()<<std::endl;
        return 0;
    }
}

int kinematic::temp_Ik(bool ikfrominit, KDL::Chain chain, KDL::JntArray jointinit, KDL::JntArray jointmin, KDL::JntArray jointmax, KDL::Frame cart, KDL::JntArray &jointcur){

    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(chain);

    KDL::ChainIkSolverVel_pinv iksolverv(chain); //Inverse velocity solver needed for IK
    KDL::ChainIkSolverPos_NR_JL iksolver(chain, jointmin, jointmax, fksolver, iksolverv, 100, 1e-6); //max 100 iterations, stop at accuracy 1e-6

    KDL::JntArray JntResult(chain.getNrOfJoints());

    int status=0;
    if(ikfrominit){ //Perform Ik from Init.
        status = iksolver.CartToJnt(jointinit, cart, JntResult);
    } else {
        status = iksolver.CartToJnt(jointcur, cart, JntResult);
    }

    if(status>=0){
        jointcur=JntResult;
        return 1;
    } else {
        //std::cout<<iksolver.getError()<<std::endl;
        return 0;
    }
}



















