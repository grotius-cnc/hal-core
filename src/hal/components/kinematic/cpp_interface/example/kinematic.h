#ifndef KINEMATIC_H
#define KINEMATIC_H

#ifndef ULAPI
#define ULAPI
#endif

#define SUCCESS 1
#undef Success //https://eigen.tuxfamily.org/bz/show_bug.cgi?id=253

#include <chainiksolverpos_lma.hpp>
#include <chainfksolverpos_recursive.hpp>
#include <chainiksolvervel_pinv.hpp>
#include <chainiksolverpos_nr_jl.hpp>

#include <iostream>
#include <chrono>
#include <thread>

class kinematic
{
public:
    //! Kdl data storage:
    KDL::Chain KDLChain;
    KDL::JntArray KDLJointInit;
    KDL::JntArray KDLJointCur;
    KDL::JntArray KDLJointMin;
    KDL::JntArray KDLJointMax;
    KDL::Frame cart,cartzero;

    int Init();
    int Fk();
    int Fk_zero();
    int Ik(bool mode_ikfrominit);
    int temp_Ik(bool ikfrominit,KDL::Chain chain, KDL::JntArray jointinit, KDL::JntArray jointmin, KDL::JntArray jointmax, KDL::Frame cart, KDL::JntArray &jointcur);
    int Fk_tooldir(double x_in, double y_in, double z_in, double &x_out, double &y_out, double &z_out);
};

#endif // KINEMATIC_H
