//! Forward and inverted backward Scurve algo, using: gain 0-100%

//! Inputs:
//! gain=curve power.
//! am=acceleration a.
//! vo=velocity begin.
//! ve=velocity end.
//! at_time=request curve state at timestamp[t]
//! Results:
//! ct=curve time.
//! vr=velocity result.
//! sr=displacement result.
//! ar=acceleration result.
template<typename T>
struct TRS{
    //! Backward scurve.
    int scurve_bck(T gain, T am, T vo, T ve, T at_time, T &ct, T &vr, T &sr, T &ar){

        //! Dummy
        T x;

        //! Invert curve inputs.
        T temp_vo=vo;
        vo=ve;
        ve=temp_vo;

        //! Get curve time[ct];
        scurve_fwd(gain, am, vo, ve, 0, ct, x, x, x);
        T curve_time=ct;

        //! Inverted time function:
        scurve_fwd(gain, am, vo, ve, curve_time-at_time, x, vr, sr, ar);

        return 0;
    }

    //! Forward scurve.
    int scurve_fwd(T gain, T am, T vo, T ve, T at_time, T &ct, T &vr, T &sr, T &ar){

        //! formula's lineair acceleration:
        //! v*v=(vo*vo) + 2*a*s, s=vo*t + 0.5*a*(t*t), t=sqrt(s*2/a) ,v=vo+a*t
        //! formula's lineair steady:
        //! s=vo+ v*t, t=s/v
        //! formula's scurve:
        //! a(t)=jm*t
        //! v(t)=vo+jm*(t*t)/2
        //! s(t)=vo*t+jm*(t*t*t)/6

        //! Calculated acc at inflection point.
        T as=0;
        //! Jerk.
        T jm=0;
        //! Velocity.
        T v1=0,v2=0,v3=0;
        //! Displacement.
        T s1=0,s2=0,s3=0;
        //! Periods. t1=concave, t2=lineair transition, t3=convex.
        T t1=0,t2=0,t3=0;

        //! Gain 0-100% to make it more user friendly.
        //!
        //! 100% gain is full scurve without lineair transition period.
        //! 0.001% gain is a traditional lineair curve.
        gain=gain*((ve-vo)/100);

        as=2*am;
        t1=gain/as;
        jm=as/t1;
        t3=t1;

        //! Calculate period t2. Here we use no vo values.
        v1=0 +jm*(t1*t1)/2;                 //! Velocity end has priority.
        v3=v1;
        v2=(ve-vo)-(v1+v3);
        t2=v2/as;

        //! Totals, used by [t<..] requests below.
        if(t2<0){t2=0;}
        if(t2>0){                           //! Gain value = < ve-vo.
            v1=vo +jm*(t1*t1)/2;
            v2=v1+as*t2;
            v3=v2 + as*t3 - jm*(t3*t3)/2;

            s1=vo*t1 +jm*(t1*t1*t1)/6;
            s2=s1+ v1*t2+0.5*as*(t2*t2);
            s3=s2+ v2*t3 + as*(t3*t3)/2 - jm*(t3*t3*t3)/6;
        }
        if(t2==0){                          //! Gain value = ~(ve-vo).
            v1=vo +jm*(t1*t1)/2;
            v3=v1 + as*t3 - jm*(t3*t3)/2;

            s1=vo*t1 +jm*(t1*t1*t1)/6;
            s3=s1+ v1*t3 + as*(t3*t3)/2 - jm*(t3*t3*t3)/6;
        }

        ct=t1+t2+t3;                        //! Total curve time.

        T t=at_time;

        if(t<t1){                           //! Period concave t1
            v1=vo +jm*(t*t)/2;
            s1=vo*t +jm*(t*t*t)/6;

            vr=v1;
            sr=s1;
            ar=jm*t;
            return 0;
        }
        if(t>=t1 && t<=t1+t2){              //! Period lineair transition t2
            t-=t1;
            v2=v1+as*t;
            s2=s1+ v1*t+0.5*as*(t*t);

            vr=v2;
            sr=s2;
            ar=as;
            return 0;
        }
        if(t>t1+t2 && t<t1+t2+t3){          //! Period convex t3
            t-=t1;
            t-=t2;

            if(t2==0){
                v2=v1;
                s2=s1;
            }

            v3=v2 + as*t - jm*(t*t)/2;
            s3=s2+ v2*t + as*(t*t)/2 - jm*(t*t*t)/6;

            vr=v3;
            sr=s3;
            ar=as-jm*t;
            return 0;
        }
        //! Normally don't get here.
        return 0;
    }
};

void MainWindow::on_pushButton_pressed()
{
    TRS <double> sc;
    double s=0, vo=0, ve=0, vs=0, am=0, at_time=0, ct=0, cs=0, vr=0, sr=0, ar=0, gain=0;

    am=ui->doubleSpinBox_acceleration->value();
    vo=ui->doubleSpinBox_velocity_begin->value();
    ve=ui->doubleSpinBox_velocity_end->value();
    vs=ui->doubleSpinBox_velocity_max->value();
    s=ui->doubleSpinBox_displacement->value();
    gain=ui->doubleSpinBox_gain->value();

    double x=0;
    sc.scurve_bck(gain,am,vo,ve,at_time,ct,x,x,x);

    for(double t=0; t<ct; t+=0.01){
        sc.scurve_bck(gain,am,vo,ve,t,x,vr,sr,ar);
        pvec_a.push_back({t,ar,0});
        pvec_v.push_back({t,vr,0});
        pvec_s.push_back({t,sr*0.1,0});
    }

    draw_result();
}
