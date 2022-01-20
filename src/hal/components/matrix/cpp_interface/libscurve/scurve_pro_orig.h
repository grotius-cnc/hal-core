#ifndef SCURVE_PRO_ORIG_H
#define SCURVE_PRO_ORIG_H

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
#include <external_var.h>



class scurve_pro_orig
{
public:
    scurve_pro_orig();

    struct SCURVE{
        //! Traditional lineair curve.
        //! ct=curve time.
        //! cs=curve displacement.
        //! vr=velocity result.
        //! sr=displacment result.
        //! ar=acceleration result.
        double ct=0, cs=0, vr=0, sr=0, ar=0;
        //! sc_ct=scurve curve time.
        //! sc_cs=scurve curve displacement.
        //! sc_vr=scurve velocity result.
        //! sc_sr=scurve displacment result.
        //! sc_ar=scurve acceleration result.
        double sc_ct=0, sc_cs=0, sc_vr=0, sc_sr=0, sc_ar=0;

        //! Various:
        bool debug=0, no_motion, finished;
    };

    SCURVE scurve_fwd(double gain, double am, double vo, double ve, double at_time);
    SCURVE scurve_bck(double gain, double am, double vo, double ve, double at_time);
    SCURVE motion(double s, double vo, double ve, double vs, double a, double at_time, double gain);

private:
    SCURVE res;
};


#endif // SCURVE_PRO_ORIG_H
