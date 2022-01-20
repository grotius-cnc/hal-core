//! Traditional lineair curve.
//! Fully funtional, curve up, curve down.

#define print(x) std::cout<<std::fixed<<#x<<": "<<x<<" ";
#define end std::cout<<std::endl;

template<typename T>
struct TR{

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
    int motion(T s, T vo, T ve, T vs, T a, T at_time, T &ct, T &cs, T &vr, T &sr, T &ar){

        T t1=0, t2=0, t3=0, s1=0, s2=0, s3=0, v1=0;
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

            if(t2<0){ //! Ve can not be reached. Set new ve based on s.

                ve=sqrt((vo*vo) + 2*-a*s);
                t1=(vo-ve)/a;

                s1=abs(vo*t1 +0.5*-a*(t1*t1));

                t2=0; //! Only t1 is used in this case.
                t3=0;
                s2=0;
                s3=0;
            }

            v1=vo - a*t1;

            ct=t1+t2+t3;
            cs=s1+s2+s3;

            if(t<t1){ //! Period 1.
                vr=vo - a*t;
                sr=abs(vo*t + 0.5*-a*(t*t));
                ar=-a;
                return 0;
            }
            if(t>=t1 &&t<=t1+t2){ //! Period 2.
                t-=t1;
                vr=v1;
                sr=s1 + (v1*t);
                ar=0;
                return 0;
            }
            if(t>t1+t2){ //! Period 3.
                t-=t1;
                t-=t2;
                vr=v1 + -a*t;
                sr=abs(s1+s2 + v1*t + 0.5*-a*(t*t));
                ar=-a;
                return 0;
            }
        }


        //! Curve up algo.
        if(ve>=vo && vs>0 && s>0){

            //! Limits.
            if(ve>vs){ve=vs;}

            t1=(ve-vo)/a;
            s1=vo*t1 +0.5*a*(t1*t1);

            if(s1>s){ //! Ve can not be reached! Set new ve based on s.
                t1=sqrt(s/(0.5*a));
                ve=vo+a*t1;

                ct=t1;
                cs=s;

                vr=vo+a*t;
                sr=0.5*a*(t*t);
                ar=a;
                return 0;
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

            v1=vo+ a*t1;

            ct=t1+t2+t3;
            cs=s1+s2+s3;

            if(t<t1){ //! Period 1.
                vr=vo + a*t;
                sr=vo*t + 0.5*a*(t*t);
                ar=a;
                return 0;
            }
            if(t>=t1 &&t<=t1+t2){ //! Period 2.
                t-=t1;
                vr=v1;
                sr=s1 + (v1*t);
                ar=0;
                return 0;
            }
            if(t>t1+t2){ //! Period 3.
                t-=t1;
                t-=t2;
                vr=v1 + -a*t;
                sr=abs(s1+s2 + v1*t + 0.5*-a*(t*t));
                ar=-a;
                return 0;
            }
        }
        return 0;
    }
};

void MainWindow::on_pushButton_run_pressed()
{
    TR<double> tr;
    double s=0, vo=0, ve=0, vs=0, a=0, at_time=0, ct=0, cs=0, vr=0, sr=0, ar=0;

    a=ui->doubleSpinBox_acceleration->value();
    vo=ui->doubleSpinBox_velocity_begin->value();
    ve=ui->doubleSpinBox_velocity_end->value();
    vs=ui->doubleSpinBox_velocity_max->value();
    s=ui->doubleSpinBox_displacement->value();

    double x=0;
    tr.motion(s, vo, ve, vs, a, at_time,ct,x,x,x,x);

    for(double t=0; t<ct; t+=0.01){
        tr.motion(s, vo, ve, vs, a, t,ct,cs,vr,sr,ar);
        pvec_a.push_back({t,ar,0});
        pvec_v.push_back({t,vr,0});
        pvec_s.push_back({t,sr*0.1,0});
    }

    draw_result();
}
