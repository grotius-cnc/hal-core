//! Traditional lineair curve. No curve down period in this example.

#define print(x) std::cout<<std::fixed<<#x<<": "<<x<<" ";
#define end std::cout<<std::endl;

template<typename T>
struct TRL{

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
    int motion(T stot, T vo, T ve, T vs, T a, T at_time, T &ct, T &cs, T &vr, T &sr, T &ar){

        T t1=0, t2=0, t3=0, s1=0, s2=0, s3=0, v1=0, v2=0, v3=0;
        T t=at_time;

        //! Used formula's:
        //! s=vo+ v*t, t=s/v, v*v=2*a*s, s=0.5*a*(t*t), t=sqrt(s/0.5*a) ,v=vo+a*t
        //! a(t)=jm*t
        //! v(t)=vo+jm*(t*t)/2
        //! s(t)=vo*t+jm*(t*t*t)/6

        //! Limits curve up.
        if(ve>vs){ve=vs;}

        if(vo>ve){
            std::cout<<"mention curve is down !!"<<std::endl;
        }

        //! Curve up algo.
        if(ve>=vo && vs>0 && stot>0){


            t1=(ve-vo)/a;
            s1=vo*t1 +0.5*a*(t1*t1);
            //std::cout<<"t1:"<<t1<<" s1:"<<s1<<std::endl;

            if(s1>stot){ //! Ve can not be reached! Set new ve based on s.
                t1=sqrt(stot/(0.5*a));
                ve=vo+a*t1;

                ct=t1;
                cs=stot;

                vr=vo+a*t;
                sr=0.5*a*(t*t);
                ar=a;
                std::cout<<"curve don't fit displacment reqeust, velocity end:"<<ve<<" curve time:"<<t1<<std::endl;
                std::cout<<""<<stot<<std::endl;
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
                s2=stot-(s1+s3);
                t2=s2/vs;

                if(s1+s3<=stot){
                    break;
                } else {
                    vs-=0.01;
                }
            }

            std::cout<<"stot:"<<stot<<std::endl;
            std::cout<<"s1:"<<s1<<std::endl;
            std::cout<<"s2:"<<s2<<std::endl;
            std::cout<<"s3:"<<s3<<std::endl;
            std::cout<<""<<stot<<std::endl;

            v1=vo+ a*t1;

            ct=t1+t2+t3;
            cs=s1+s2+s3;

            if(t<=t1){ //! Period 1.
                vr=vo + a*t;
                sr=vo*t + 0.5*a*(t*t);
                ar=a;
                return 0;
            }
            if(t>t1 &&t<t1+t2){ //! Period 2.
                t-=t1;
                vr=v1;
                sr=s1 + (v1*t);
                ar=0;
                return 0;
            }
            if(t>=t1+t2){ //! Period 3.
                t-=t1;
                t-=t2;
                vr=v1 + -a*t;
                sr=s1+s2 + v1*t + 0.5*-a*(t*t);
                ar=-a;
                return 0;
            }
        }
        return 0;
    }
};

void MainWindow::on_pushButton_run_pressed()
{
    TRL<double> tr;
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
