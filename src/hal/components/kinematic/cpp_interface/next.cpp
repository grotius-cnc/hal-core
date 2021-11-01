#include "next.h"
#include "iostream"

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

Next::Next()
{

}

data Next::init(data d){

    //! Kdl data storage:
    KDL::Chain KDLChain;
    KDL::JntArray KDLJointInit;
    KDL::JntArray KDLJointCur;
    KDL::JntArray KDLJointMin;
    KDL::JntArray KDLJointMax;
    KDL::Frame cart;

    //! Setup Kdl chain following the attached document : Kuka Kr 6 10 angilus.pdf
    KDLChain.addSegment(KDL::Segment("J0",KDL::Joint(KDL::Joint::RotZ), KDL::Frame(KDL::Vector(d.J0_x,d.J0_y,d.J0_z))));        //0.0 to J2
    KDLChain.addSegment(KDL::Segment("J1",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J1_x,d.J1_y,d.J1_z))));        //J2 to J3
    KDLChain.addSegment(KDL::Segment("J2",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J2_x,d.J2_y,d.J2_z))));        //J3 to J5
    KDLChain.addSegment(KDL::Segment("J3",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(d.J3_x,d.J3_y,d.J3_z))));        //J4
    KDLChain.addSegment(KDL::Segment("J4",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J4_x,d.J4_y,d.J4_z))));        //J5 to end-effector (robot flange axis 6)
    KDLChain.addSegment(KDL::Segment("J5",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(d.J5_x,d.J5_y,d.J5_z))));        //Tool cone lenght.

    // The floortrack is loaded outside the KDLChainVec size.

    KDLJointMin.resize(KDLChain.getNrOfSegments());
    KDLJointMax.resize(KDLChain.getNrOfSegments());
    KDLJointCur.resize(KDLChain.getNrOfSegments());
    KDLJointInit.resize(KDLChain.getNrOfSegments());

    KDLJointInit(0)= d.J0_init; //joint to radians
    KDLJointCur(0)= d.J0;
    KDLJointMin(0)= d.J0_min;
    KDLJointMax(0)= d.J0_max;

    KDLJointInit(1)= d.J1_init; //joint to radians
    KDLJointCur(1)= d.J1;
    KDLJointMin(1)= d.J1_min;
    KDLJointMax(1)= d.J1_max;

    KDLJointInit(2)= d.J2_init; //joint to radians
    KDLJointCur(2)= d.J2;
    KDLJointMin(2)= d.J2_min;
    KDLJointMax(2)= d.J2_max;

    KDLJointInit(3)= d.J3_init; //joint to radians
    KDLJointCur(3)= d.J3;
    KDLJointMin(3)= d.J3_min;
    KDLJointMax(3)= d.J3_max;

    KDLJointInit(4)= d.J4_init; //joint to radians
    KDLJointCur(4)= d.J4;
    KDLJointMin(4)= d.J4_min;
    KDLJointMax(4)= d.J4_max;

    KDLJointInit(5)= d.J5_init; //joint to radians
    KDLJointCur(5)= d.J5;
    KDLJointMin(5)= d.J5_min;
    KDLJointMax(5)= d.J5_max;

    // Perform a forward kinematic calculation (fk).
    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);
    int status = fksolver.JntToCart(KDLJointCur,cart,-1);

    if(status==0 || status>0){
        //std::cout<<"fk init ok"<<std::endl;
        //std::cout<<"cartx:"<<cart.p.x()<<" y:"<<cart.p.y()<<" z:"<<cart.p.z()<<std::endl;

        // Add the xyz values to the data bucket.
        d.Cartx=cart.p.x();
        d.Carty=cart.p.y();
        d.Cartz=cart.p.z();

        // Add the euler values to the data bucket.
        cart.M.GetEulerZYX(d.Eulerz,d.Eulery,d.Eulerx);

        // Add the joint values to the data bucket.
        d.J0=KDLJointCur(0);
        d.J1=KDLJointCur(1);
        d.J2=KDLJointCur(2);
        d.J3=KDLJointCur(3);
        d.J4=KDLJointCur(4);
        d.J5=KDLJointCur(5);
        d.error=0;

    } else {
        //std::cout<<fksolver.getError()<<std::endl;
        //std::cout<<"fk error"<<std::endl;
        d.error=1;
    }

    return d;
}

data Next::fk(data d){

    //! Kdl data storage:
    KDL::Chain KDLChain;
    KDL::JntArray KDLJointInit;
    KDL::JntArray KDLJointCur;
    KDL::JntArray KDLJointMin;
    KDL::JntArray KDLJointMax;
    KDL::Frame cart;

    //! Setup Kdl chain following the attached document : Kuka Kr 6 10 angilus.pdf
    KDLChain.addSegment(KDL::Segment("J0",KDL::Joint(KDL::Joint::RotZ), KDL::Frame(KDL::Vector(d.J0_x,d.J0_y,d.J0_z))));        //0.0 to J2
    KDLChain.addSegment(KDL::Segment("J1",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J1_x,d.J1_y,d.J1_z))));        //J2 to J3
    KDLChain.addSegment(KDL::Segment("J2",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J2_x,d.J2_y,d.J2_z))));        //J3 to J5
    KDLChain.addSegment(KDL::Segment("J3",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(d.J3_x,d.J3_y,d.J3_z))));        //J4
    KDLChain.addSegment(KDL::Segment("J4",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J4_x,d.J4_y,d.J4_z))));        //J5 to end-effector (robot flange axis 6)
    KDLChain.addSegment(KDL::Segment("J5",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(d.J5_x,d.J5_y,d.J5_z))));        //Tool cone lenght.

    // The floortrack is loaded outside the KDLChainVec size.
    KDLJointMin.resize(KDLChain.getNrOfSegments());
    KDLJointMax.resize(KDLChain.getNrOfSegments());
    KDLJointCur.resize(KDLChain.getNrOfSegments());
    KDLJointInit.resize(KDLChain.getNrOfSegments());

    KDLJointInit(0)= d.J0_init; //joint to radians
    KDLJointCur(0)= d.J0;
    KDLJointMin(0)= d.J0_min;
    KDLJointMax(0)= d.J0_max;

    KDLJointInit(1)= d.J1_init; //joint to radians
    KDLJointCur(1)= d.J1;
    KDLJointMin(1)= d.J1_min;
    KDLJointMax(1)= d.J1_max;

    KDLJointInit(2)= d.J2_init; //joint to radians
    KDLJointCur(2)= d.J2;
    KDLJointMin(2)= d.J2_min;
    KDLJointMax(2)= d.J2_max;

    KDLJointInit(3)= d.J3_init; //joint to radians
    KDLJointCur(3)= d.J3;
    KDLJointMin(3)= d.J3_min;
    KDLJointMax(3)= d.J3_max;

    KDLJointInit(4)= d.J4_init; //joint to radians
    KDLJointCur(4)= d.J4;
    KDLJointMin(4)= d.J4_min;
    KDLJointMax(4)= d.J4_max;

    KDLJointInit(5)= d.J5_init; //joint to radians
    KDLJointCur(5)= d.J5;
    KDLJointMin(5)= d.J5_min;
    KDLJointMax(5)= d.J5_max;

    // Perform a forward kinematic calculation (fk).
    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);
    int status = fksolver.JntToCart(KDLJointCur,cart,-1);

    if(status==0 || status>0){
        //std::cout<<"fk ok"<<std::endl;
        //std::cout<<"cartx:"<<cart.p.x()<<" y:"<<cart.p.y()<<" z:"<<cart.p.z()<<std::endl;

        // Add the xyz values to the data bucket.
        d.Cartx=cart.p.x();
        d.Carty=cart.p.y();
        d.Cartz=cart.p.z();

        // Add the euler values to the data bucket.
        cart.M.GetEulerZYX(d.Eulerz,d.Eulery,d.Eulerx);
        d.error=0;
    } else {
        //std::cout<<fksolver.getError()<<std::endl;
        //std::cout<<"fk error"<<std::endl;
        d.error=1;
    }

    return d;
}

data Next::ik(data d){

    //! Kdl data storage:
    KDL::Chain KDLChain;
    KDL::JntArray KDLJointInit;
    KDL::JntArray KDLJointCur;
    KDL::JntArray KDLJointMin;
    KDL::JntArray KDLJointMax;
    KDL::Frame cart;

    //! Setup Kdl chain following the attached document : Kuka Kr 6 10 angilus.pdf
    KDLChain.addSegment(KDL::Segment("J0",KDL::Joint(KDL::Joint::RotZ), KDL::Frame(KDL::Vector(d.J0_x,d.J0_y,d.J0_z))));        //0.0 to J2
    KDLChain.addSegment(KDL::Segment("J1",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J1_x,d.J1_y,d.J1_z))));        //J2 to J3
    KDLChain.addSegment(KDL::Segment("J2",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J2_x,d.J2_y,d.J2_z))));        //J3 to J5
    KDLChain.addSegment(KDL::Segment("J3",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(d.J3_x,d.J3_y,d.J3_z))));        //J4
    KDLChain.addSegment(KDL::Segment("J4",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(d.J4_x,d.J4_y,d.J4_z))));        //J5 to end-effector (robot flange axis 6)
    KDLChain.addSegment(KDL::Segment("J5",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(d.J5_x,d.J5_y,d.J5_z))));        //Tool cone lenght.

    // The floortrack is loaded outside the KDLChainVec size.
    KDLJointMin.resize(KDLChain.getNrOfSegments());
    KDLJointMax.resize(KDLChain.getNrOfSegments());
    KDLJointCur.resize(KDLChain.getNrOfSegments());
    KDLJointInit.resize(KDLChain.getNrOfSegments());

    KDLJointInit(0)= d.J0_init; //joint to radians
    KDLJointCur(0)= d.J0;
    KDLJointMin(0)= d.J0_min;
    KDLJointMax(0)= d.J0_max;

    KDLJointInit(1)= d.J1_init; //joint to radians
    KDLJointCur(1)= d.J1;
    KDLJointMin(1)= d.J1_min;
    KDLJointMax(1)= d.J1_max;

    KDLJointInit(2)= d.J2_init; //joint to radians
    KDLJointCur(2)= d.J2;
    KDLJointMin(2)= d.J2_min;
    KDLJointMax(2)= d.J2_max;

    KDLJointInit(3)= d.J3_init; //joint to radians
    KDLJointCur(3)= d.J3;
    KDLJointMin(3)= d.J3_min;
    KDLJointMax(3)= d.J3_max;

    KDLJointInit(4)= d.J4_init; //joint to radians
    KDLJointCur(4)= d.J4;
    KDLJointMin(4)= d.J4_min;
    KDLJointMax(4)= d.J4_max;

    KDLJointInit(5)= d.J5_init; //joint to radians
    KDLJointCur(5)= d.J5;
    KDLJointMin(5)= d.J5_min;
    KDLJointMax(5)= d.J5_max;

    // Add the xyz values to the data bucket.
    cart.p.x(d.Cartx);
    cart.p.y(d.Carty);
    cart.p.z(d.Cartz);

    // Testparams for kuka robot.
    // cart.p.x(500);
    // cart.p.y(0);
    // cart.p.z(500);

    //cart.M.EulerZYX(d.Eulerz,d.Eulery,d.Eulerx);

    cart.M.DoRotZ(d.Eulerz-cart.M.GetRot().z());
    cart.M.DoRotY(d.Eulery-cart.M.GetRot().y());
    cart.M.DoRotX(d.Eulerx-cart.M.GetRot().x());

    // Perform a inverse kinematic calculation (fk).

    KDL::ChainFkSolverPos_recursive fksolver = KDL::ChainFkSolverPos_recursive(KDLChain);

    KDL::ChainIkSolverVel_pinv iksolverv(KDLChain); //Inverse velocity solver needed for IK
    if(d.Iterations==0){d.Iterations=100;}
    KDL::ChainIkSolverPos_NR_JL iksolver(KDLChain, KDLJointMin, KDLJointMax, fksolver, iksolverv, d.Iterations, 1e-6); //max 100 iterations, stop at accuracy 1e-6
    // We can add a hal pin in the future to set max iteration values.

    KDL::JntArray JntResult(KDLChain.getNrOfJoints());

    int status=0;
    if(d.Ik_from_init){ // Perform Ik from Init, we can add a hal input pin for this.
        status = iksolver.CartToJnt(KDLJointInit, cart, JntResult);
    } else {
        status = iksolver.CartToJnt(KDLJointCur, cart, JntResult);
    }

    if(status>=0){

        // Write values to the bucket data.
        d.J0=JntResult(0);
        d.J1=JntResult(1);
        d.J2=JntResult(2);
        d.J3=JntResult(3);
        d.J4=JntResult(4);
        d.J5=JntResult(5);
        d.error=0;
    } else {
        //std::cout<<iksolver.getError()<<std::endl;
        //std::cout<<"ik error"<<std::endl;
        d.error=1;
    }
    return d;
}

extern "C" data init_wrapper(data d){
    struct data r;
    r=Next().init(d);
    return r;
}

extern "C" data fk_wrapper(data d){
    struct data r;
    r=Next().fk(d);
    return r;
}

extern "C" data ik_wrapper(data d){
    struct data r;
    r=Next().ik(d);
    return r;
}














