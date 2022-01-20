#include "iostream"

template<typename T>
struct scurve{

    #define print(x) std::cout<<std::fixed<<#x<<": "<<x<<" ";

    //! Used formula's:
    //! s=v*t, t=s/v, v*v=2*a*s, s=0.5*a*(t*t), v=vo+a*t
    //! a(t)=jm*t
    //! v(t)=vo+jm*(t*t)/2
    //! s(t)=vo*t+jm*(t*t*t)/6

    //! Acceleration max.
    T am=0;
    //! Calculated acc at inflection point.
    T as=0;
    //! Jerk.
    T jm=0;
    //! Velocity begin.
    T vo=0;
    //! Acceleration.
    T a=0;
    //! Velocity.
    T v1=0,v2=0,v3=0;
    //! Displacement.
    T s1=0,s2=0,s3=0;
    //! Periods
    T t1=0,t2=0,t3=0;
    //! Curve results.
    T vr=0, sr=0, ar=0;
    //! Total displacement.
    T stot=0;
    //! Velocity end.
    T vtot=0;
    //! Curve power.
    T gain=0;
    //! Curve time.
    T ct=0;
    //! Conventional lineair curve results.
    T sl=0, vl=0;

    //! Calculate the curve periods. [concave t1][lineair_transition t2][convex t3]
    void init(){
        as=2*am;
        t1=gain/as;
        jm=as/t1;
        t3=t1;

        v1=vo +jm*(t1*t1)/2;                //! Velocity end has priority.
        v3=v2 + as*t3 - jm*(t3*t3)/2;
        v2=vtot-(v1+v3);
        if(v2<=0){                          //! No lineair transition period is possible.
            t2=0;
        }
        if(v2>0){
            t2=v2/as;
        }
        ct=t1+t2+t3;                        //! Total curve time.


    }
    void lineair(double t){                 //! Traditional lineair acc-dcc curve.
        sl=vo*t +0.5*am*(t*t);
        vl=vo+ am*t;
    }
    void concave(double t){                 //! Period t1
        a=jm*t;
        v1=vo +jm*(t*t)/2;
        s1=vo*t +jm*(t*t*t)/6;

        vr=v1;
        sr=s1;
    }
    void lineair_transition(double t){      //! Period t2
        a=as;
        v2=v1+as*t;
        s2=v1*t+0.5*as*(t*t);
        s2+=s1;

        vr=v2;
        sr=s2;
    }
    void convex(double t){                  //! Period t3
        if(t2<=0){                          //! If lineair period is zero. Use front curve as vo value!
            v2=v1;
            s2=s1;
        }

        a=as-jm*t;
        v3=v2 + as*t - jm*(t*t)/2;
        s3=v2*t + as*(t*t)/2 - jm*(t*t*t)/6;
        s3+=s2;

        vr=v3;
        sr=s3;
    }
    int calculate(double at_time){
        double t=at_time;

        lineair(t);                     //! Conventional lineair curve.
        if(t<=t1){
            concave(t);
            return 0;
        }
        t-=t1;
        if(t<=t2){
            concave(t1);
            lineair_transition(t);
            return 0;
        }
        t-=t2;
        if(t<=t3){
            concave(t1);
            lineair_transition(t2);
            convex(t);
            if(t==t3){
                stot=sr;
            }
            return 0;
        }
        return 1;
    }
    void print_results(){
        //! Scurve
        print(as); print(am); print(jm)     std::cout<<std::endl;
        print(t1); print(t2); print(t3);    std::cout<<std::endl;
        print(ct);                          std::cout<<std::endl;
        print(v1); print(v2); print(v3);    std::cout<<std::endl;
        print(v3);                          std::cout<<std::endl;
        print(s1); print(s2); print(s3);    std::cout<<std::endl;
        print(s3);                          std::cout<<std::endl;

        //! Lineair traditional curve:
        print(sl); print(vl);               std::cout<<std::endl;

        //! Scurve-pro time savers:
        std::cout<<"Efficienty factor [scurve with lineair transition] versus [normal scurve]"<<std::endl;
        std::cout<<"Displacement factor improvment [stot/sl]: "<<s3/sl<<std::endl;
        std::cout<<"Velocity factor improvment [vtot/vl]    : "<<v3/vl<<std::endl;
    }
};

void run()
{
    scurve <double>s;
    s.am=2;
    s.gain=4;
    s.vtot=8;
    s.init();
    for(double t=0; t<s.ct; t+=0.01){

        auto start = std::chrono::high_resolution_clock::now();
        s.calculate(t);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        std::cout << "Time taken by function nanoseconds : ~" << duration.count() << " milliseconds: ~" << duration.count()*0.000001 <<std::endl;

        // pvec_a.push_back({t,s.a,0});
        // pvec_v.push_back({t,s.vr,0});
        // pvec_s.push_back({t,s.sr,0});
        // pvec_vl.push_back({t,s.vl,0});
        // pvec_sl.push_back({t,s.sl,0});
    }
    s.calculate(s.ct);
    s.print_results();
}
