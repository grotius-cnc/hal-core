#pragma once

#include <array>
#include <optional>
#include "profile.hpp"
#include "block.hpp"

namespace ruckig {

using Limits = Profile::Limits;
using JerkSigns = Profile::JerkSigns;

//! Mathematical equations for Step 1 in position interface: Extremal profiles
class PositionStep1 {
    double p0, v0, a0;
    double pf, vf, af;
    double _vMax, _vMin, _aMax, _aMin, _jMax;

    // Pre-calculated expressions
    double pd;
    double v0_v0, vf_vf;
    double a0_a0, a0_p3, a0_p4, a0_p5, a0_p6;
    double af_af, af_p3, af_p4, af_p5, af_p6;
    double jMax_jMax;

    // Only a single velocity-limited profile can be valid
    bool has_up_vel {false}, has_down_vel {false};

    // Max 5 valid profiles + 1 spare for numerical issues
    std::array<Profile, 6> valid_profiles;
    size_t valid_profile_counter;

    void time_acc0_acc1_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_acc1_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_acc0_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_acc0_acc1(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_acc1(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_acc0(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_none(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);

    // Only for numerical issues
    void time_acc0_two_step(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_vel_two_step(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    void time_none_two_step(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);


    inline void add_profile(Profile profile, double jMax) {
        profile.direction = (jMax > 0) ? Profile::Direction::UP : Profile::Direction::DOWN;

        if (profile.limits == Limits::ACC0_ACC1_VEL || profile.limits == Limits::ACC0_VEL || profile.limits == Limits::ACC1_VEL || profile.limits == Limits::VEL) {
            switch (profile.direction) {
                case Profile::Direction::UP: has_up_vel = true; break;
                case Profile::Direction::DOWN: has_down_vel = true; break;
            }
        }

        valid_profiles[valid_profile_counter] = profile;
        ++valid_profile_counter;
    }

public:
    explicit PositionStep1(double p0, double v0, double a0, double pf, double vf, double af, double vMax, double vMin, double aMax, double aMin, double jMax);

    bool get_profile(const Profile& input, Block& block);
};


//! Mathematical equations for Step 2 in position interface: Time synchronization
class PositionStep2 {
    double p0, v0, a0;
    double tf, pf, vf, af;
    double _vMax, _vMin, _aMax, _aMin, _jMax;

    // Pre-calculated expressions
    double pd;
    double tf_tf, tf_p3, tf_p4;
    double vd, vd_vd;
    double ad, ad_ad;
    double v0_v0, vf_vf;
    double a0_a0, a0_p3, a0_p4, a0_p5, a0_p6;
    double af_af, af_p3, af_p4, af_p5, af_p6;
    double jMax_jMax;
    double g1, g2;

    bool time_acc0_acc1_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    bool time_acc1_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    bool time_acc0_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    bool time_vel(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    bool time_acc0_acc1(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    bool time_acc1(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    bool time_acc0(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);
    bool time_none(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax);

    inline bool check_all(Profile& profile, double vMax, double vMin, double aMax, double aMin, double jMax) {
        return time_acc0_acc1_vel(profile, vMax, vMin, aMax, aMin, jMax)
            || time_acc0_vel(profile, vMax, vMin, aMax, aMin, jMax)
            || time_acc1_vel(profile, vMax, vMin, aMax, aMin, jMax)
            || time_vel(profile, vMax, vMin, aMax, aMin, jMax)
            || time_acc0(profile, vMax, vMin, aMax, aMin, jMax)
            || time_acc1(profile, vMax, vMin, aMax, aMin, jMax)
            || time_acc0_acc1(profile, vMax, vMin, aMax, aMin, jMax)
            || time_none(profile, vMax, vMin, aMax, aMin, jMax);
    }

public:
    explicit PositionStep2(double tf, double p0, double v0, double a0, double pf, double vf, double af, double vMax, double vMin, double aMax, double aMin, double jMax);

    bool get_profile(Profile& profile);
};

} // namespace ruckig
