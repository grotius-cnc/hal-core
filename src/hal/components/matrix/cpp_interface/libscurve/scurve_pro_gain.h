#ifndef SCURVE_PRO_GAIN_H
#define SCURVE_PRO_GAIN_H

//!
//! An improved scurve algoritme using gain 1-100% to
//! reduce machine's acceleration, deceleration periods.
//!
//! Made by Skynet Cyberdyne 03-01-2022.
//! Licence Gpl2.
//!
//! 04-01-2022 Added motion_stop function.
//!

#include <iostream>
#include <math.h>
#include <stdio.h>

template<typename T>
class scurve_pro_gain
{
public:
    scurve_pro_gain();
    //! Structure containing results.
    struct Sc_Result{
        //! Scurve with gian 1-100%
        //! sc_ct=scurve curve time.
        //! sc_cs=scurve curve displacement.
        //! sc_vr=scurve velocity result.
        //! sc_sr=scurve displacment result.
        //! sc_ar=scurve acceleration result.
        T sc_ct=0, sc_cs=0, sc_vr=0, sc_sr=0, sc_ar=0;
        //! Motion.
        //! tr_ct=total curve time.
        //! tr_cs=total curve displacement.
        T tr_ct=0, tr_cs=0;

        //! Result of wich period[t1-t2-t3] the curve is.
        //! Period 1=concave, 2=steady, 3=convex.
        unsigned int period;
    };

    Sc_Result Sc_fwd(T gain, T am, T vo, T ve, T at_time);
    Sc_Result Sc_bck(T gain, T am, T vo, T ve, T at_time);
    Sc_Result motion(T s, T vo, T ve, T vs, T a, T at_time, T gain);
};
template class scurve_pro_gain<double>;

#endif // SCURVE_PRO_GAIN_H
