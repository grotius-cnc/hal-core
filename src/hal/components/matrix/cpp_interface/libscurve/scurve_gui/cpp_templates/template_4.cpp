#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "iostream"


#define print(x); std::cout<<std::fixed<<#x<<": "<<x<<" ";
#define end; std::cout<<std::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpencascadeWidget = new Opencascade(this);
    ui->gridLayout_opencascade->addWidget(OpencascadeWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


std::vector<gp_Pnt> pvec_v, pvec_s, pvec_a;
std::vector<gp_Pnt> pvec_sc_v, pvec_sc_s, pvec_sc_a;

#define print(x) std::cout<<std::fixed<<#x<<": "<<x<<" ";
#define end std::cout<<std::endl;

template<typename T>
struct Result{
    //! Traditional lineair curve.
    T ct=0, cs=0, vr=0, sr=0, ar=0;
    //! Scurve.
    T sc_ct=0, sc_cs=0, sc_vr=0, sc_sr=0, sc_ar=0;
};

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
Result<T> scurve_fwd(T gain, T am, T vo, T ve, T at_time){
    Result<T> r;

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
        v2=0;
        v3=v1 + as*t3 - jm*(t3*t3)/2;

        s1=vo*t1 +jm*(t1*t1*t1)/6;
        s2=0;
        s3=s1+ v1*t3 + as*(t3*t3)/2 - jm*(t3*t3*t3)/6;
    }

    r.sc_ct=t1+t2+t3;                        //! Total curve time.
    r.sc_cs=s3;

    T t=at_time;

    if(t<t1){                           //! Period concave t1
        v1=vo +jm*(t*t)/2;
        s1=vo*t +jm*(t*t*t)/6;

        r.sc_vr=v1;
        r.sc_sr=s1;
        r.sc_ar=jm*t;
        return r;
    }
    if(t>=t1 && t<=t1+t2){              //! Period lineair transition t2
        t-=t1;
        v2=v1+as*t;
        s2=s1+ v1*t+0.5*as*(t*t);

        r.sc_vr=v2;
        r.sc_sr=s2;
        r.sc_ar=as;
        return r;
    }
    if(t>t1+t2 && t<=t1+t2+t3){          //! Period convex t3
        t-=t1;
        t-=t2;

        if(t2==0){
            v2=v1;
            s2=s1;
        }

        v3=v2 + as*t - jm*(t*t)/2;
        s3=s2+ v2*t + as*(t*t)/2 - jm*(t*t*t)/6;

        r.sc_vr=v3;
        r.sc_sr=s3;
        r.sc_ar=as-jm*t;
        return r;
    }
    //! Normally don't get here.
    return r;
}

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
Result<T> scurve_bck(T gain, T am, T vo, T ve, T at_time){
    Result<T> r;

    //! Invert vo,ve
    T vo_temp=vo;
    vo=ve;
    ve=vo_temp;

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
        v2=0;
        v3=v1 + as*t3 - jm*(t3*t3)/2;

        s1=vo*t1 +jm*(t1*t1*t1)/6;
        s2=0;
        s3=s1+ v1*t3 + as*(t3*t3)/2 - jm*(t3*t3*t3)/6;
    }

    r.sc_ct=t1+t2+t3;                        //! Total curve time.
    r.sc_cs=s3;

    //! Invert time.
    double t=r.sc_ct-at_time;

    if(t<t1){                           //! Period concave t1
        v1=vo +jm*(t*t)/2;
        s1=vo*t +jm*(t*t*t)/6;

        r.sc_vr=v1;
        r.sc_sr=r.sc_cs-s1;             //! Invert displacement.
        r.sc_ar=-std::abs(jm*t);        //! Invert acceleration.
        return r;
    }
    if(t>=t1 && t<=t1+t2){              //! Period lineair transition t2
        t-=t1;
        v2=v1+as*t;
        s2=s1+ v1*t+0.5*as*(t*t);

        r.sc_vr=v2;
        r.sc_sr=r.sc_cs-s2;             //! Invert displacement.
        r.sc_ar=-std::abs(as);          //! Invert acceleration.
        return r;
    }
    if(t>t1+t2 && t<=t1+t2+t3){          //! Period convex t3
        t-=t1;
        t-=t2;

        if(t2==0){
            v2=v1;
            s2=s1;
        }

        v3=v2 + as*t - jm*(t*t)/2;
        s3=s2+ v2*t + as*(t*t)/2 - jm*(t*t*t)/6;

        r.sc_vr=v3;
        r.sc_sr=r.sc_cs-s3;             //! Invert displacement.
        r.sc_ar=-std::abs(as-jm*t);     //! Invert acceleration.
        return r;
    }
    //! Normally don't get here.
    return r;
}

template<typename T>
//! s=displacement.
//! vo=velocity begin.
//! ve=velocity end.
//! vs=velocity max.
//! a=acceleration.
//! at_time=request at timestamp.
//! ct=curve time.
//! cs=curve displacement.
//! vr=velocity result.
//! sr=displacement result.
//! ar=acceleration result.
Result<T> motion(T s, T vo, T ve, T vs, T a, T at_time, T gain){

    //! Scurve structure.
    Result<double> r;

    //! Traditional lineair curve
    T t1=0, t2=0, t3=0, s1=0, s2=0, s3=0, v1=0, ct=0, cs=0, vr=0, sr=0, ar=0;
    T t=at_time;

    //! formula's lineair acceleration:
    //! v*v=(vo*vo) + 2*a*s, s=vo*t + 0.5*a*(t*t), t=sqrt(s*2/a) ,v=vo+a*t
    //! formula's lineair steady:
    //! s=vo+ v*t, t=s/v
    //! formula's scurve:
    //! a(t)=jm*t
    //! v(t)=vo+jm*(t*t)/2
    //! s(t)=vo*t+jm*(t*t*t)/6

    //! Global limits.
    if(ve<0){ve=0;}
    if(vo<0){vo=0;}

    //! Curve down algo.
    if(ve<vo && vs>0 && s>0){

        //! Limits
        if(vs>vo){vs=vo;}
        if(ve>vs){ve=vs;}

        t1=(vo-ve)/a;
        s1=vo*t1 +0.5*a*(t1*t1);

        //! Dcc period to vs.
        t1=abs((vo-vs)/-a);
        s1=abs(vo*t1 +0.5*-a*(t1*t1));

        //! Dcc vs to ve period.
        t3=abs((vs-ve)/-a);
        s3=abs(vs*t3 +0.5*-a*(t3*t3));

        //! Vs steady period.
        s2=s-(s1+s3);
        t2=s2/vs;

        bool prompt_new_ve=0;
        if(t2<0){ //! Ve can not be reached. Set new ve based on s.

            ve=sqrt((vo*vo) + 2*-a*s);
            t1=(vo-ve)/a;

            s1=abs(vo*t1 +0.5*-a*(t1*t1));

            t2=0; //! Only t1 is used in this case.
            t3=0;
            s2=0;
            s3=0;

            prompt_new_ve=1;
        }

        v1=vo - a*t1;

        ct=t1+t2+t3;
        cs=s1+s2+s3;

        if(t<t1){ //! Period 1.
            vr=vo - a*t;
            sr=abs(vo*t + 0.5*-a*(t*t));
            ar=-a;

            if(prompt_new_ve){
                 r=scurve_bck(gain,a,vo,ve,t);
                 r.vr=vr;
                 r.sr=sr;
                 r.ar=ar;
                 r.ct=ct;
                 r.cs=cs;
                 return r;
            }
            r=scurve_bck(gain,a,vo,vs,t);
            r.vr=vr;
            r.sr=sr;
            r.ar=ar;
            r.ct=ct;
            r.cs=cs;
            return r;
        }
        if(t>=t1 &&t<=t1+t2){ //! Period 2.
            t-=t1;
            vr=v1;
            sr=s1 + (v1*t);
            ar=0;

            r.sc_vr=vr;
            r.sc_sr=sr;
            r.sc_ar=ar;

            r.vr=vr;
            r.sr=sr;
            r.ar=ar;
            r.ct=ct;
            r.cs=cs;
            return r;
        }
        if(t>t1+t2){ //! Period 3.
            t-=t1;
            t-=t2;
            vr=v1 + -a*t;
            sr=abs(s1+s2 + v1*t + 0.5*-a*(t*t));
            ar=-a;

            r=scurve_bck(gain,a,vs,ve,t);
            r.sc_sr+=s1;
            r.sc_sr+=s2;
            r.vr=vr;
            r.sr=sr;
            r.ar=ar;
            r.ct=ct;
            r.cs=cs;
            return r;
        }
    }

    //! Curve up algo.
    if(ve>=vo && vs>0 && s>0){

        //! Limits.
        if(ve>vs){ve=vs;}

        t1=(ve-vo)/a;
        s1=vo*t1 +0.5*a*(t1*t1);

        bool prompt_new_ve=0;
        if(s1>s){ //! Ve can not be reached! Set new ve based on s.
            t1=sqrt(s/(0.5*a));
            ve=vo+a*t1;

            ct=t1;
            cs=s;

            vr=vo+a*t;
            sr=0.5*a*(t*t);
            ar=a;

            prompt_new_ve=1;
        }

        //! Sample to fit.
        while(1){

            //! Acc period.
            t1=(vs-vo)/a;
            if(t1<0){t1=0;}
            s1=vo*t1 +0.5*a*(t1*t1);

            //! Dcc period.
            t3=abs((vs-ve)/-a);
            if(t3<0){t3=0;}
            s3=abs(vs*t3 +0.5*-a*(t3*t3));

            //! Steady period.
            s2=s-(s1+s3);
            t2=s2/vs;

            if(s1+s3<=s){
                break;
            } else {
                vs-=0.01;
            }
        }

        //! v1 is given by sc.scurve_fwd function.
        v1=vo+ a*t1;

        ct=t1+t2+t3;
        cs=s1+s2+s3;

        if(t<t1){ //! Period 1.
            vr=vo + a*t;
            sr=vo*t + 0.5*a*(t*t);
            ar=a;

            if(prompt_new_ve){
                r=scurve_fwd(gain,a,vo,ve,t);
                r.vr=vr;
                r.sr=sr;
                r.ar=ar;
                r.ct=ct;
                r.cs=cs;
                return r;
            }

            r=scurve_fwd(gain,a,vo,vs,t);
            r.vr=vr;
            r.sr=sr;
            r.ar=ar;
            r.ct=ct;
            r.cs=cs;
            return r;
        }
        if(t>=t1 &&t<=t1+t2){ //! Period 2.
            t-=t1;

            vr=v1;
            sr=s1 + (v1*t);
            ar=0;

            r.sc_vr=vr;
            r.sc_sr=sr;
            r.sc_ar=ar;

            r.vr=vr;
            r.sr=sr;
            r.ar=ar;
            r.ct=ct;
            r.cs=cs;
            return r;
        }
        if(t>t1+t2){ //! Period 3.
            t-=t1;
            t-=t2;

            vr=v1 + -a*t;
            sr=abs(s1+s2 + v1*t + 0.5*-a*(t*t));
            ar=-a;

            r=scurve_bck(gain,a,vs,ve,t);
            r.sc_sr+=s1;
            r.sc_sr+=s2;
            r.vr=vr;
            r.sr=sr;
            r.ar=ar;
            r.ct=ct;
            r.cs=cs;
            return r;
        }
    }
    return r;
}


void MainWindow::on_pushButton_run_pressed()
{
    Result<double> r;
    double s=0, vo=0, ve=0, vs=0, a=0, gain=0;

    a=ui->doubleSpinBox_acceleration->value();
    vo=ui->doubleSpinBox_velocity_begin->value();
    ve=ui->doubleSpinBox_velocity_end->value();
    vs=ui->doubleSpinBox_velocity_max->value();
    s=ui->doubleSpinBox_displacement->value();
    gain=ui->doubleSpinBox_gain->value();

    r=motion<double>(s, vo, ve, vs, a, 0.0, gain);

    for(double t=0; t<r.ct; t+=0.01){
        r=motion<double>(s, vo, ve, vs, a, t, gain);
        pvec_a.push_back({t,r.ar,0});
        pvec_v.push_back({t,r.vr,0});
        pvec_s.push_back({t,r.sr*0.1,0});

        pvec_sc_a.push_back({t,r.sc_ar,0});
        pvec_sc_v.push_back({t,r.sc_vr,0});
        pvec_sc_s.push_back({t,r.sc_sr*0.1,0});
    }

    draw_result();

    // //! Test a scurve.
    //    Result<double> r;

    //    double s=0, vo=0, ve=0, vs=0, am=0, at_time=0.0, gain=0;

    //    am=ui->doubleSpinBox_acceleration->value();
    //    vo=ui->doubleSpinBox_velocity_begin->value();
    //    ve=ui->doubleSpinBox_velocity_end->value();
    //    vs=ui->doubleSpinBox_velocity_max->value();
    //    s=ui->doubleSpinBox_displacement->value();
    //    gain=ui->doubleSpinBox_gain->value();

    //    r=scurve_bck(gain,am,vo,ve,at_time);
    //    double ct=r.sc_ct;
    //    for(double t=0; t<=ct; t+=0.1){
    //        r=scurve_bck(gain,am,vo,ve,t);
    //        pvec_a.push_back({t,r.sc_ar,0});
    //        pvec_v.push_back({t,r.sc_vr,0});
    //        pvec_s.push_back({t,r.sc_sr*0.1,0});

    //        //std::cout<<std::fixed<<"sc_vr:"<<r.sc_vr<<std::endl;
    //        //std::cout<<std::fixed<<"sc_sr:"<<r.sc_sr<<std::endl;
    //    }

    //    draw_result();
}

void MainWindow::on_doubleSpinBox_acceleration_valueChanged(double arg1)
{
    on_pushButton_run_pressed();
}

void MainWindow::on_doubleSpinBox_velocity_end_valueChanged(double arg1)
{
    on_pushButton_run_pressed();
}

void MainWindow::on_doubleSpinBox_displacement_valueChanged(double arg1)
{
    on_pushButton_run_pressed();
}

void MainWindow::on_doubleSpinBox_velocity_max_valueChanged(double arg1)
{
    on_pushButton_run_pressed();
}

void MainWindow::on_doubleSpinBox_velocity_begin_valueChanged(double arg1)
{
    on_pushButton_run_pressed();
}

void MainWindow::on_doubleSpinBox_gain_valueChanged(double arg1)
{
    on_pushButton_run_pressed();
}

void MainWindow::draw_result(){

    OpencascadeWidget->Remove_all();

    Handle(AIS_Shape) awire;
    //! Draw results:
    if(pvec_v.size()>0){
        awire=draw_primitives().draw_3d_line_wire(pvec_v);
        awire=draw_primitives().colorize(awire,Quantity_NOC_BLACK,0);
        OpencascadeWidget->show_shape(awire);
    }
    if(pvec_s.size()>0){
        awire=draw_primitives().draw_3d_line_wire(pvec_s);
        awire=draw_primitives().colorize(awire,Quantity_NOC_BLUE,0);
        OpencascadeWidget->show_shape(awire);
    }
    if(pvec_a.size()>0){
        awire=draw_primitives().draw_3d_line_wire(pvec_a);
        awire=draw_primitives().colorize(awire,Quantity_NOC_RED,0);
        OpencascadeWidget->show_shape(awire);
    }
    if(pvec_sc_v.size()>0){
        awire=draw_primitives().draw_3d_line_wire(pvec_sc_v);
        awire=draw_primitives().colorize(awire,Quantity_NOC_GREEN,0);
        OpencascadeWidget->show_shape(awire);
    }
    if(pvec_sc_s.size()>0){
        awire=draw_primitives().draw_3d_line_wire(pvec_sc_s);
        awire=draw_primitives().colorize(awire,Quantity_NOC_YELLOW,0);
        OpencascadeWidget->show_shape(awire);
    }
    if(pvec_sc_a.size()>0){
        awire=draw_primitives().draw_3d_line_wire(pvec_sc_a);
        awire=draw_primitives().colorize(awire,Quantity_NOC_PURPLE,0);
        OpencascadeWidget->show_shape(awire);
    }


    OpencascadeWidget->Redraw();

    pvec_a.clear();
    pvec_v.clear();
    pvec_s.clear();
    pvec_sc_v.clear();
    pvec_sc_s.clear();
    pvec_sc_a.clear();
}



