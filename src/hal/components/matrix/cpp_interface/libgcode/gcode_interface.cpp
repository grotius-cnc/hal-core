#include "gcode_interface.h"
#include "scurve_pro_orig.h"

double at_time=0;
double dtg_fwd=0, dtg_bck=0;
double old_vs=0;
double old_acc=0;
double old_gain=0;

bool pause_init=0, stop_init=0, run_init=0, gcode_init=0;
bool pause_request=0, stop_request=0, run_request=0;
bool pos=0, neg=0, no_motion=0;

double feedrate=0;
//! Feed overide factor 1-...
double fo=1;

unsigned int gl=0;
unsigned int gp=0;
double vs=0, vo=0, ve=0, s=0, a=0, gain=0;
scurve_pro_orig::SCURVE res;


bool finished=0;
double at_jog_time=0;
scurve_pro_orig::SCURVE jog;

gcode_interface::gcode_interface()
{
}

////! Manual runner used by halmodule.c every 1ms.
DATA gcode_interface::manual_runner(DATA *ptr){
    for(unsigned int i=0; i<ptr->machines; i++){

        //! Manual macro's M3,M5 etc are processed by the mainwindow.

        //! Jog joints with scurve.
        for(unsigned int j=0; j<ptr->machine[i].joints; j++){

            bool jog_joint_init=ptr->machine[i].joint[j].jog_joint_init;

            if(jog_joint_init){

                //! Init abs curpos when btn was pressed.
                double x0=ptr->machine[i].joint[j].scurve_abs_joint_init;

                //! Target pos togo to.
                double x1=ptr->machine[i].joint[j].scurve_abs_joint_request;

                //! Is it a trans or rot axis?
                double vs=ptr->machine[i].maxvel_cart;
                double a=ptr->machine[i].maxacc_cart;
                if(ptr->machine[i].joint[j].rot){
                    vs=ptr->machine[i].maxvel_euler;
                    a=ptr->machine[i].maxacc_euler;
                }
                double vo=0;
                double ve=0;
                double s=x1-x0;
                double at_time=ptr->machine[i].joint[j].at_jog_time;
                double r=0;

                //! 2.Calculate motion at a certain time stamp.
                jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
                if(at_time>jog.ct){
                    at_time=jog.ct;
                    jog_joint_init=0;
                }
                //! std::cout<<"at_time:"<<at_time<<std::endl;

                //! Positive move
                if(x1>=x0){
                    r=x0+jog.sc_sr;
                }

                //! Negative move
                if(x1<x0){
                    r=x0-jog.sc_sr;
                }

                //! 3.Write values.
                ptr->machine[i].joint[j].curpos_abs=r;
                at_time+=0.001;
                ptr->machine[i].joint[j].at_jog_time=at_time;
                //! Motion finished, reset flag.
                ptr->machine[i].joint[j].jog_joint_init=jog_joint_init;
            }
        }

        if(ptr->machine[i].jog_cartx_init){
            //! Init abs curpos when btn was pressed.
            double x0=ptr->machine[i].scurve_jog_abs_cartx_init;
            //! Target pos togo to.
            double x1=ptr->machine[i].scurve_jog_abs_cartx_request;

            double vs=ptr->machine[i].maxvel_cart;
            double a=ptr->machine[i].maxacc_cart;
            double vo=0;
            double ve=0;
            double s=x1-x0;
            double at_time=ptr->machine[i].at_cartx_time;
            double r=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_cartx_init=0;
            }

            //! Positive move
            if(x1>=x0){
                r=x0+jog.sc_sr;
            }

            //! Negative move
            if(x1<x0){
                r=x0-jog.sc_sr;
            }

            //! 3.Write values.
            ptr->machine[i].abs_cart.x=r;
            at_time+=0.001;
            ptr->machine[i].at_cartx_time=at_time;
        }

        if(ptr->machine[i].jog_carty_init){
            //! Init abs curpos when btn was pressed.
            double x0=ptr->machine[i].scurve_jog_abs_carty_init;
            //! Target pos togo to.
            double x1=ptr->machine[i].scurve_jog_abs_carty_request;

            double vs=ptr->machine[i].maxvel_cart;
            double a=ptr->machine[i].maxacc_cart;
            double vo=0;
            double ve=0;
            double s=x1-x0;
            double at_time=ptr->machine[i].at_carty_time;
            double r=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_carty_init=0;
            }

            //! Positive move
            if(x1>=x0){
                r=x0+jog.sc_sr;
            }

            //! Negative move
            if(x1<x0){
                r=x0-jog.sc_sr;
            }

            //! 3.Write values.
            ptr->machine[i].abs_cart.y=r;
            at_time+=0.001;
            ptr->machine[i].at_carty_time=at_time;
        }

        if(ptr->machine[i].jog_cartz_init){
            //! Init abs curpos when btn was pressed.
            double x0=ptr->machine[i].scurve_jog_abs_cartz_init;
            //! Target pos togo to.
            double x1=ptr->machine[i].scurve_jog_abs_cartz_request;

            double vs=ptr->machine[i].maxvel_cart;
            double a=ptr->machine[i].maxacc_cart;
            double vo=0;
            double ve=0;
            double s=x1-x0;
            double at_time=ptr->machine[i].at_cartz_time;
            double r=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_cartz_init=0;
            }

            //! Positive move
            if(x1>=x0){
                r=x0+jog.sc_sr;
            }

            //! Negative move
            if(x1<x0){
                r=x0-jog.sc_sr;
            }

            //! 3.Write values.
            ptr->machine[i].abs_cart.z=r;
            at_time+=0.001;
            ptr->machine[i].at_cartz_time=at_time;
        }

        if(ptr->machine[i].jog_eulerx_init){
            //! Init abs curpos when btn was pressed.
            double x0=ptr->machine[i].scurve_jog_abs_eulerx_init;
            //! Target pos togo to.
            double x1=ptr->machine[i].scurve_jog_abs_eulerx_request;

            double vs=ptr->machine[i].maxvel_euler;
            double a=ptr->machine[i].maxacc_euler;
            double vo=0;
            double ve=0;
            double s=x1-x0;
            double at_time=ptr->machine[i].at_eulerx_time;
            double r=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_eulerx_init=0;
                //! Write target value.
            }

            //! Positive move
            if(x1>=x0){
                r=x0+jog.sc_sr;
            }

            //! Negative move
            if(x1<x0){
                r=x0-jog.sc_sr;
            }

            //! 3.Write values.
            ptr->machine[i].abs_euler.x=r;
            at_time+=0.001;
            ptr->machine[i].at_eulerx_time=at_time;
        }

        if(ptr->machine[i].jog_eulery_init){
            //! Init abs curpos when btn was pressed.
            double x0=ptr->machine[i].scurve_jog_abs_eulery_init;
            //! Target pos togo to.
            double x1=ptr->machine[i].scurve_jog_abs_eulery_request;

            double vs=ptr->machine[i].maxvel_euler;
            double a=ptr->machine[i].maxacc_euler;
            double vo=0;
            double ve=0;
            double s=x1-x0;
            double at_time=ptr->machine[i].at_eulery_time;
            double r=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_eulery_init=0;
                //! Write target value.
            }

            //! Positive move
            if(x1>=x0){
                r=x0+jog.sc_sr;
            }

            //! Negative move
            if(x1<x0){
                r=x0-jog.sc_sr;
            }

            //! 3.Write values.
            ptr->machine[i].abs_euler.y=r;
            at_time+=0.001;
            ptr->machine[i].at_eulery_time=at_time;
        }

        if(ptr->machine[i].jog_eulerz_init){
            //! Init abs curpos when btn was pressed.
            double x0=ptr->machine[i].scurve_jog_abs_eulerz_init;
            //! Target pos togo to.
            double x1=ptr->machine[i].scurve_jog_abs_eulerz_request;

            double vs=ptr->machine[i].maxvel_euler;
            double a=ptr->machine[i].maxacc_euler;
            double vo=0;
            double ve=0;
            double s=x1-x0;
            double at_time=ptr->machine[i].at_eulerz_time;
            double r=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_eulerz_init=0;
                //! Write target value.
            }

            //! Positive move
            if(x1>=x0){
                r=x0+jog.sc_sr;
            }

            //! Negative move
            if(x1<x0){
                r=x0-jog.sc_sr;
            }

            //! 3.Write values.
            ptr->machine[i].abs_euler.z=r;
            at_time+=0.001;
            ptr->machine[i].at_eulerz_time=at_time;
        }

        if(ptr->machine[i].jog_toolx_init){
            //! Init abs curpos when btn was pressed.
            POINT p0=ptr->machine[i].scurve_jog_abs_toolx_init;
            //! Target pos togo to.
            POINT p1=ptr->machine[i].scurve_jog_abs_toolx_request;
            POINT p;

            double vs=ptr->machine[i].maxvel_cart;
            double a=ptr->machine[i].maxacc_cart;
            double vo=0;
            double ve=0;
            double s=sqrt(pow(p1.x-p0.x,2)+pow(p1.y-p0.y,2)+pow(p1.z-p0.z,2));
            double at_time=ptr->machine[i].at_toolx_time;
            double distancetogo=0;
            double ratio=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_toolx_init=0;
            }

            //! Interpolate
            distancetogo=s-jog.sr;
            ratio=1-(distancetogo/s);
            if(std::isnan(ratio)){
                ratio=0;
            }
            p.x= p0.x + ((p1.x-p0.x)*ratio);
            p.y= p0.y + ((p1.y-p0.y)*ratio);
            p.z= p0.z + ((p1.z-p0.z)*ratio);

            //! 3.Write values.
            ptr->machine[i].abs_cart=p;
            at_time+=0.001;
            ptr->machine[i].at_toolx_time=at_time;
        }

        if(ptr->machine[i].jog_tooly_init){
            //! Init abs curpos when btn was pressed.
            POINT p0=ptr->machine[i].scurve_jog_abs_tooly_init;
            //! Target pos togo to.
            POINT p1=ptr->machine[i].scurve_jog_abs_tooly_request;
            POINT p;

            double vs=ptr->machine[i].maxvel_cart;
            double a=ptr->machine[i].maxacc_cart;
            double vo=0;
            double ve=0;
            double s=sqrt(pow(p1.x-p0.x,2)+pow(p1.y-p0.y,2)+pow(p1.z-p0.z,2));
            double at_time=ptr->machine[i].at_tooly_time;
            double distancetogo=0;
            double ratio=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_tooly_init=0;
            }

            //! Interpolate
            distancetogo=s-jog.sr;
            ratio=1-(distancetogo/s);
            if(std::isnan(ratio)){
                ratio=0;
            }
            p.x= p0.x + ((p1.x-p0.x)*ratio);
            p.y= p0.y + ((p1.y-p0.y)*ratio);
            p.z= p0.z + ((p1.z-p0.z)*ratio);

            //! 3.Write values.
            ptr->machine[i].abs_cart=p;
            at_time+=0.001;
            ptr->machine[i].at_tooly_time=at_time;
        }

        if(ptr->machine[i].jog_toolz_init){
            //! Init abs curpos when btn was pressed.
            POINT p0=ptr->machine[i].scurve_jog_abs_toolz_init;
            //! Target pos togo to.
            POINT p1=ptr->machine[i].scurve_jog_abs_toolz_request;
            POINT p;

            double vs=ptr->machine[i].maxvel_cart;
            double a=ptr->machine[i].maxacc_cart;
            double vo=0;
            double ve=0;
            double s=sqrt(pow(p1.x-p0.x,2)+pow(p1.y-p0.y,2)+pow(p1.z-p0.z,2));
            double at_time=ptr->machine[i].at_toolz_time;
            double distancetogo=0;
            double ratio=0;

            //! 2.Calculate motion at a certain time stamp.
            jog=scurve_pro_orig().motion(abs(s), vo, ve, abs(vs), a, at_time, 100);
            if(at_time>=jog.ct){
                at_time=jog.ct;
                ptr->machine[i].jog_toolz_init=0;
            }

            //! Interpolate
            distancetogo=s-jog.sr;
            ratio=1-(distancetogo/s);
            if(std::isnan(ratio)){
                ratio=0;
            }
            p.x= p0.x + ((p1.x-p0.x)*ratio);
            p.y= p0.y + ((p1.y-p0.y)*ratio);
            p.z= p0.z + ((p1.z-p0.z)*ratio);

            //! 3.Write values.
            ptr->machine[i].abs_cart=p;
            at_time+=0.001;
            ptr->machine[i].at_toolz_time=at_time;
        }
    }
    return *ptr;
}

extern "C" struct DATA mr(DATA *ptr){
    struct DATA result_ptr=gcode_interface().manual_runner(ptr);
    return result_ptr;
};

////! Gcode runner used by halmodule.c every 1ms.
DATA gcode_interface::gcode_runner(DATA *ptr){
    for(unsigned int i=0; i<ptr->machines; i++){

        //! Run mdi.
        if(ptr->machine[i].state_machine==MDI){
            if(ptr->machine[i].sc.mdi_ready_to_run){
                if(ptr->machine[i].sc.mdi_reset){
                    ptr->machine[i].sc.cur_gcodeline=0;
                    ptr->machine[i].state_program=STOP;
                    ptr->machine[i].sc.mdi_reset=0;
                }
                //! Reset gcode program.
                ptr->machine[i].sc.gcode_reset=1;
                ptr->machine[i].sc.cur_gcodevec=i;
                if(gcodevec.at(i).blockvec.size()>0){
                    motion_read(ptr,i);
                    motion_run(ptr, i);
                    motion_pause(ptr, i);
                    motion_stop(ptr, i);
                    motion_fwd(ptr, i);
                    motion_rev(ptr, i);
                    motion_macro(ptr, i); //! process M3,M5 etc.
                    motion_write(ptr,i);
                    motion_interpolate_results(ptr,i);
                }
                //! Motion finished?
                if(ptr->machine[i].sc.end_of_traject==1){
                    ptr->machine[i].sc.mdi_ready_to_run=0;
                    ptr->machine[i].sc.end_of_traject=0;
                    //! Remove graphics by mainwindow->opencascade function.
                    ptr->machine[i].sc.mdi_remove_graphics=1;
                    ptr->machine[i].sc.mdi_reset=1;
                }
            }
        }

        //! Run gcode.
        if(ptr->machine[i].state_machine==AUTO){
label:
            //! At program startup.
            if(ptr->machine[i].sc.gcode_ready_to_run){
                //! Check if in position.
                if(ptr->machine[i].sc.gcode_reset){
                    if(!in_startpos(ptr,i)){
                        ptr->machine[i].sc.gcode_ready_to_run=0;
                        ptr->machine[i].sc.message_not_in_startpos=1;
                        goto label;
                    }
                    ptr->machine[i].sc.cur_gcodeline=ptr->machine[i].sc.run_from_line;
                    ptr->machine[i].sc.stop_init=0;
                    ptr->machine[i].state_program=STOP;
                    ptr->machine[i].sc.gcode_init=0;
                    ptr->machine[i].sc.at_time=0;
                    ptr->machine[i].sc.gcode_reset=0;
                }
                //! Reset mdi program.
                ptr->machine[i].sc.mdi_reset=1;
                ptr->machine[i].sc.cur_gcodevec=i+1;
                if(gcodevec.at(i+1).blockvec.size()>0){
                    motion_read(ptr,i);
                    motion_run(ptr, i);
                    motion_pause(ptr, i);
                    motion_stop(ptr, i);
                    motion_fwd(ptr, i);
                    motion_rev(ptr, i);
                    motion_macro(ptr, i); //! process M3,M5 etc.
                    motion_write(ptr,i);
                    motion_interpolate_results(ptr,i);
                }
                //! Motion finished?
                if(ptr->machine[i].sc.end_of_traject==1){
                    ptr->machine[i].sc.gcode_ready_to_run=0;
                    ptr->machine[i].sc.end_of_traject=0;
                    ptr->machine[i].sc.gcode_reset=1;
                }
            }
        }
    }
    return *ptr;
}

extern "C" struct DATA gr(DATA *ptr){
    struct DATA result_ptr=gcode_interface().gcode_runner(ptr);
    return result_ptr;
};

void gcode_interface::motion_macro(DATA *ptr, int i){

    //! Gcode program.
    unsigned int gp=ptr->machine[i].sc.cur_gcodevec;
    //! Gcode line.
    unsigned int gl=ptr->machine[i].sc.cur_gcodeline;

    //! Macro nr. example:M3
    int mn=gcodevec.at(gp).blockvec.at(gl).Macro_nr;
    //! Macro value. example: M3 P10.00
    double mv=gcodevec.at(gp).blockvec.at(gl).P;

    //! Standard macro's M3,M5,M7,M8,M9, Excusive Macro's M300 P0.01,M500 P12345 etc
    //! Each macro may contain a value of type double.
    //! std::cout<<"macro M"<<mn<<" value P"<<mv<<std::endl;
    ptr->machine[i].gcode_macro_nr=mn;
    ptr->machine[i].gcode_macro_value=mv;
}

int gcode_interface::in_startpos(DATA *ptr, int i){

    double x0=ptr->machine[i].rel_cart.x;
    double y0=ptr->machine[i].rel_cart.y;
    double z0=ptr->machine[i].rel_cart.z;

    double x1=gcodevec.at(i+1).blockvec.at(ptr->machine[i].sc.run_from_line).start.x;
    double y1=gcodevec.at(i+1).blockvec.at(ptr->machine[i].sc.run_from_line).start.y;
    double z1=gcodevec.at(i+1).blockvec.at(ptr->machine[i].sc.run_from_line).start.z;

    //! Everything within 1mm is ok.
    if(x0<=x1+1 && x0>=x1-1 && y0<=y1+1 && y0>=y1-1 && z0<=z1+1 && z0>=z1-1){
        std::cout<<"ok in position"<<std::endl;
        return 1;
    }

    std::setprecision(3);
    //! std::cout<<std::fixed<<"curpos x:"<<x0<<" y:"<<y0<<" z:"<<z0<<std::endl;
    //! std::cout<<"perform a mdi to pos x:"<<x1<<" y:"<<y1<<" z:"<<z1<<std::endl;
    std::string str="perform a mdi to pos x:";
    str+=std::to_string(x1);
    str+=" y:";
    str+=std::to_string(y1);
    str+=" z:";
    str+=std::to_string(z1);
    strcpy(ptr->machine[i].sc.message,str.c_str());
    return 0;
}

void gcode_interface::motion_read(DATA *ptr, int i){
    //! Initialize values for each machine instance.
    at_time=ptr->machine[i].sc.at_time;
    dtg_fwd=ptr->machine[i].sc.dtg_fwd,
            dtg_bck=ptr->machine[i].sc.dtg_bck;
    old_vs=ptr->machine[i].sc.old_vs;
    old_acc=ptr->machine[i].sc.old_acc;
    old_gain=ptr->machine[i].sc.old_gain;
    pause_init=ptr->machine[i].sc.pause_init;
    stop_init=ptr->machine[i].sc.stop_init;
    run_init=ptr->machine[i].sc.run_init;
    gcode_init=ptr->machine[i].sc.gcode_init;
    pause_request=ptr->machine[i].sc.pause_request;
    stop_request=ptr->machine[i].sc.stop_request;
    run_request=ptr->machine[i].sc.run_request;
    pos=ptr->machine[i].sc.pos;
    neg=ptr->machine[i].sc.neg;
    no_motion=ptr->machine[i].sc.no_motion;
    feedrate=ptr->machine[i].sc.feedrate;
    fo=ptr->machine[i].sc.feed_overide;
    vs=ptr->machine[i].sc.vs;
    vo=ptr->machine[i].sc.vo;
    ve=ptr->machine[i].sc.ve;
    s=ptr->machine[i].sc.s;
    a=ptr->machine[i].sc.a;
    gain=ptr->machine[i].sc.gain;
    gp=ptr->machine[i].sc.cur_gcodevec;
    gl=ptr->machine[i].sc.cur_gcodeline;
}

void gcode_interface::motion_write(DATA *ptr, int i){
    //! Write values for each machine instance.
    ptr->machine[i].sc.at_time=at_time;
    ptr->machine[i].sc.dtg_fwd=dtg_fwd;
    ptr->machine[i].sc.dtg_bck=dtg_bck;
    ptr->machine[i].sc.old_vs=old_vs;
    ptr->machine[i].sc.old_acc=old_acc;
    ptr->machine[i].sc.old_gain=old_gain;
    ptr->machine[i].sc.pause_init=pause_init;
    ptr->machine[i].sc.stop_init=stop_init;
    ptr->machine[i].sc.run_init=run_init;
    ptr->machine[i].sc.gcode_init=gcode_init;
    ptr->machine[i].sc.pause_request=pause_request;
    ptr->machine[i].sc.stop_request=stop_request;
    ptr->machine[i].sc.run_request=run_request;
    ptr->machine[i].sc.pos=pos;
    ptr->machine[i].sc.neg=neg;
    ptr->machine[i].sc.no_motion=no_motion;
    ptr->machine[i].sc.feedrate=feedrate;
    ptr->machine[i].sc.feed_overide=fo;
    ptr->machine[i].sc.vs=vs;
    ptr->machine[i].sc.vo=vo;
    ptr->machine[i].sc.ve=ve;
    ptr->machine[i].sc.s=s;
    ptr->machine[i].sc.a=a;
    ptr->machine[i].sc.gain=gain;
    ptr->machine[i].sc.cur_gcodevec=gp;
    ptr->machine[i].sc.cur_gcodeline=gl;
}

void gcode_interface::motion_run(DATA *ptr, int i){
    if(ptr->machine[i].state_program==RUN){
        if(!run_init){
            run_request=1;
            run_init=1;
        }
        pause_init=0;
        stop_init=0;
    }
}

void gcode_interface::motion_pause(DATA *ptr, int i){
    if(ptr->machine[i].state_program==PAUSE_RESUME){
        if(!pause_init){
            pause_request=1;
            pause_init=1;
        }
        run_init=0;
        stop_init=0;
    }
}

void gcode_interface::motion_stop(DATA *ptr, int i){
    if(ptr->machine[i].state_program==STOP){
        if(!stop_init){
            stop_request=1;
            stop_init=1;
        }
        run_init=0;
        pause_init=0;
    }
}

void gcode_interface::motion_interpolate_results(DATA *ptr, int i){
    //! Interpolate dtg result to cart xyz and euler xyz coordinates.
    //! Interpolate toolcenterpoint "tcp" as a line?
    if(ptr->machine[i].maxvel_cart>=0){
        if(gcodevec.at(gp).blockvec.at(gl).blocktype==G0 || gcodevec.at(gp).blockvec.at(gl).blocktype==G1){
            ptr->machine[i].tcp=interpolate_line(gcodevec.at(gp).blockvec.at(gl),ptr->machine[i].sc.gc_dtg_curblock);
        }
    }
    //! Reverse motion.
    if(ptr->machine[i].maxvel_cart<0){
        if(gcodevec.at(gp).blockvec.at(gl).blocktype==G0 || gcodevec.at(gp).blockvec.at(gl).blocktype==G1){
            ptr->machine[i].tcp=interpolate_line_reversed(gcodevec.at(gp).blockvec.at(gl),ptr->machine[i].sc.gc_dtg_curblock);
        }
    }

    //! Interpolate as a arc?
    if(ptr->machine[i].maxvel_cart>=0){
        if(gcodevec.at(gp).blockvec.at(gl).blocktype==G2 || gcodevec.at(gp).blockvec.at(gl).blocktype==G3){
            ptr->machine[i].tcp=interpolate_arc(gcodevec.at(gp).blockvec.at(gl),ptr->machine[i].sc.gc_dtg_curblock);
        }
    }
    //! Reverse motion.
    if(ptr->machine[i].maxvel_cart<0){
        if(gcodevec.at(gp).blockvec.at(gl).blocktype==G2 || gcodevec.at(gp).blockvec.at(gl).blocktype==G3){
            ptr->machine[i].tcp=interpolate_arc_reversed(gcodevec.at(gp).blockvec.at(gl),ptr->machine[i].sc.gc_dtg_curblock);
        }
    }
    //! Interpolate euler angles for each gcodeblock.
    //! The gcodeblock already knows the blocklenght at this stage.
    EULER eul={0,0,0};
    if(ptr->machine[i].maxvel_cart>=0){
        //! Do this when program is running?
        eul=interpolate_euler_fwd(gcodevec.at(gp).blockvec.at(gl),ptr->machine[i].sc.gc_dtg_curblock);
    }
    //! Euler reverse motion.
    //! TODO:
    if(ptr->machine[i].maxvel_cart<0){
        eul=interpolate_euler_bck(gcodevec.at(gp).blockvec.at(gl),ptr->machine[i].sc.gc_dtg_curblock);
    }

    //! Tcp offset:
    gp_Trsf trsf2xxx;
    gp_Quaternion aQuat1xxx;
    aQuat1xxx.SetEulerAngles (gp_YawPitchRoll, ptr->machine[i].rel_offset_euler.z,ptr->machine[i].rel_offset_euler.y,ptr->machine[i].rel_offset_euler.x);
    trsf2xxx.SetRotation(aQuat1xxx);
    gp_Trsf trsf3xxx;
    trsf3xxx.SetTranslation({0,0,0},{ptr->machine[i].rel_offset_cart.x,ptr->machine[i].rel_offset_cart.y,ptr->machine[i].rel_offset_cart.z});
    trsf3xxx.Multiply(trsf2xxx);

    //! Put everything together in a final matrix.
    gp_Trsf resultxxx;
    resultxxx.Multiply(trsf3xxx);

    gp_Pnt Pnt;
    Pnt.SetX(ptr->machine[i].tcp.x);
    Pnt.SetY(ptr->machine[i].tcp.y);
    Pnt.SetZ(ptr->machine[i].tcp.z);
    Pnt.Transform(resultxxx);

    ptr->machine[i].abs_cart.x=Pnt.X();
    ptr->machine[i].abs_cart.y=Pnt.Y();
    ptr->machine[i].abs_cart.z=Pnt.Z();
    ptr->machine[i].abs_euler.x=ptr->machine[i].rel_offset_euler.x+(eul.x*toRadians);
    ptr->machine[i].abs_euler.y=ptr->machine[i].rel_offset_euler.y+(eul.y*toRadians);
    ptr->machine[i].abs_euler.z=ptr->machine[i].rel_offset_euler.z+(eul.z*toRadians);
}

//! Forward traject.
void gcode_interface::motion_fwd(DATA *ptr, int i){

    if(ptr->machine[i].maxvel_cart>=0 && !pos){
        dtg_fwd=s-dtg_bck;
        pos=1;
        neg=0;
        //std::cout<<"switched to forward feed"<<std::endl;
    }

    if(ptr->machine[i].maxvel_cart>=0){
        //! Velocity max interupt. Reduce input frequency.
        if(old_vs!=ptr->machine[i].maxvel_cart || old_acc!=ptr->machine[i].maxacc_cart || pause_request || stop_request || run_request){
            old_vs=ptr->machine[i].maxvel_cart;
            old_acc=ptr->machine[i].maxacc_cart;

            if(run_request || ptr->machine[i].state_program==RUN){
                vs=ptr->machine[i].maxvel_cart;
                if(feedrate*fo<vs){
                    vs=feedrate*fo;
                }
            }
            if(pause_request || stop_request){
                vs=0;
            }
            a=ptr->machine[i].maxacc_cart;
            if(vs==0){
                vs=0.1;
            }
            vo=res.sc_vr;
            dtg_fwd-=res.sc_sr;
            at_time=0;
            pause_request=0;
            stop_request=0;
            run_request=0;
        }
        res=scurve_pro_orig().motion(dtg_fwd, vo, ve, abs(vs), a, at_time, 100);
        at_time+=0.001;
        if((ptr->machine[i].maxvel_cart==0 || ptr->machine[i].state_program==PAUSE_RESUME || ptr->machine[i].state_program==STOP) && res.sc_vr<=0.2){
            at_time-=0.001;
            res.sc_vr=0;
            res.sc_ar=0;
            no_motion=true;
        } else {no_motion=false;}
        if(at_time>res.ct){ //removed >=
            if(gl<gcodevec.at(gp).blockvec.size()-1){
                ptr->machine[i].sc.cur_gcodeline++;
                gl=ptr->machine[i].sc.cur_gcodeline;
                gcode_init=0;
                ptr->machine[i].sc.end_of_traject=0;
            } else {
                at_time=res.ct;
                //!
                //! End of traject.
                ptr->machine[i].sc.end_of_traject=1;
                ptr->machine[i].state_program=STOP;
            }
        }
        if(!gcode_init){
            s=gcodevec.at(gp).blockvec.at(gl).blocklenght;
            //! If a G1,G2,G3.
            feedrate=gcodevec.at(gp).blockvec.at(gl).F;
            //! If a G0 rapid.
            if(gcodevec.at(gp).blockvec.at(gl).blocktype==G0){
                feedrate=ptr->machine[i].maxvel_cart;
            }
            ptr->machine[i].sc.flag_highlight_new_gcodeline=1;
            dtg_fwd=s;
            at_time=0;
            vo=res.sc_vr;
            gcode_init=1;
        }
        ptr->machine[i].sc.gc_dtg_curblock=res.cs-res.sc_sr;
    }
}

//! Reverse traject.
void gcode_interface::motion_rev(DATA *ptr, int i){

    if(ptr->machine[i].maxvel_cart<0 && !neg){
        dtg_bck=s-dtg_fwd;
        neg=1;
        pos=0;
        //std::cout<<"switched to reverse feed"<<std::endl;
    }

    if(ptr->machine[i].maxvel_cart<0){
        //! Velocity max interupt. Reduce input frequency.
        if(old_vs!=ptr->machine[i].maxvel_cart || old_acc!=ptr->machine[i].maxacc_cart || pause_request || stop_request || run_request){
            old_vs=ptr->machine[i].maxvel_cart;
            old_acc=ptr->machine[i].maxacc_cart;

            if(run_request || ptr->machine[i].state_program==RUN){
                vs=ptr->machine[i].maxvel_cart;
                if(feedrate*fo<vs){
                    vs=feedrate*fo;
                }
            }

            if(pause_request || stop_request){
                vs=0;
            }
            a=ptr->machine[i].maxacc_cart;
            if(vs==0){
                vs=0.1;
            }
            vo=res.sc_vr;
            dtg_bck-=res.sc_sr;
            at_time=0;
            pause_request=0;
            stop_request=0;
            run_request=0;
        }
        res=scurve_pro_orig().motion(dtg_bck, vo, ve, abs(vs), a, at_time, 100);
        at_time+=0.001;
        if((ptr->machine[i].maxvel_cart==0 || ptr->machine[i].state_program==PAUSE_RESUME || ptr->machine[i].state_program==STOP) && res.sc_vr<=0.2){
            at_time-=0.001;
            res.sc_vr=0;
            res.sc_ar=0;
            no_motion=true;
        } else {no_motion=false;}
        if(at_time>res.ct){ // Removed >=
            if(gl>0){
                ptr->machine[i].sc.cur_gcodeline--;
                gl=ptr->machine[i].sc.cur_gcodeline;
                gcode_init=0;
                ptr->machine[i].sc.end_of_traject=0;
            } else {
                at_time=res.ct;
                //!
                //! End of traject.
                //ptr->machine[i].sc.end_of_traject=1;
                ptr->machine[i].state_program=STOP;
            }
        }
        if(!gcode_init){
            s=gcodevec.at(gp).blockvec.at(gl).blocklenght;
            //! If a G1,G2,G3
            feedrate=gcodevec.at(gp).blockvec.at(gl).F;
            //! If a G0 rapid.
            if(gcodevec.at(gp).blockvec.at(gl).blocktype==G0){
                feedrate=ptr->machine[i].maxvel_cart;
            }
            ptr->machine[i].sc.flag_highlight_new_gcodeline=1;
            dtg_bck=s;
            at_time=0;
            vo=res.sc_vr;
            gcode_init=1;
        }
        ptr->machine[i].sc.gc_dtg_curblock=res.cs-res.sc_sr;
    }
}

//! This function looks for sharp corners in the upfollwing gcodeblocks.
//! If there are sharp corners we reduce feedrate.
//! Additional rules related to velocity control can be applied in this functions
//! The function output is in % velocity.
double gcode_interface::gcode_rules(BLOCK cur, BLOCK next){

    //! Calculate the angle to next gcodeblock in sequence.
    //! Rapid
    if(cur.blocktype==BLOCKTYPE::G0){
        return 0;
    }
    if(cur.blocktype==BLOCKTYPE::G1 && next.blocktype==BLOCKTYPE::G1){
        double ax=cur.start.x;
        double ay=cur.start.y;
        double az=cur.start.z;
        double a[] = {ax, ay, az};

        double bx=cur.end.x;
        double by=cur.end.y;
        double bz=cur.end.z;
        double b[] = {bx, by, bz};

        double cx=next.end.x;
        double cy=next.end.y;
        double cz=next.end.z;
        double c[] = {cx, cy, cz};

        //! If angle < 1 degrees, vel_end=100%
        if(GetAngleABC(a,b,c)<5){
            //! 100% velocity.
            return 100;
        } else {
            //! 0% velocity.
            return 0;
        }
    }
    //! Line to arc
    if(cur.blocktype==BLOCKTYPE::G1 && (next.blocktype==BLOCKTYPE::G2 || next.blocktype==BLOCKTYPE::G3)){
        //! Line startpoint.
        double ax=cur.start.x;
        double ay=cur.start.y;
        double az=cur.start.z;
        double a[] = {ax, ay, az};

        //! Line endpoint.
        double bx=cur.end.x;
        double by=cur.end.y;
        double bz=cur.end.z;
        double b[] = {bx, by, bz};

        //! We look at the position of the first mm on the arc bow.
        POINT p_arc=interpolate_arc(next,(next.blocklenght-1));
        double cx=p_arc.x;
        double cy=p_arc.y;
        double cz=p_arc.z;
        double c[] = {cx, cy, cz};

        //! If angle < 1 degrees, vel_end=100%
        if(GetAngleABC(a,b,c)<5){
            //! 100% velocity.
            return 100;
        } else {
            //! 0% velocity.
            return 0;
        }
    }
    //! Arc to line
    if((cur.blocktype==BLOCKTYPE::G2 || cur.blocktype==BLOCKTYPE::G3 ) && next.blocktype==BLOCKTYPE::G1){
        //! 1mm to be at arc end point.
        POINT p_arc=interpolate_arc(cur,1);

        //! 1mm before arc endpoint.
        double ax=p_arc.x;
        double ay=p_arc.y;
        double az=p_arc.z;
        double a[] = {ax, ay, az};

        //! Arc endpoint.
        double bx=cur.end.x;
        double by=cur.end.y;
        double bz=cur.end.z;
        double b[] = {bx, by, bz};

        //! Line endpoint.
        double cx=next.end.x;
        double cy=next.end.y;
        double cz=next.end.z;
        double c[] = {cx, cy, cz};

        //! If angle < 1 degrees, vel_end=100%
        if(GetAngleABC(a,b,c)<5){
            //! 100% velocity.
            return 100;
        } else {
            //! 0% velocity.
            return 0;
        }
    }
    //! Arc to arc
    if((cur.blocktype==BLOCKTYPE::G2 || cur.blocktype==BLOCKTYPE::G3 ) &&
            (next.blocktype==BLOCKTYPE::G2 || next.blocktype==BLOCKTYPE::G3)){

        //! 1mm to be at arc end point.
        POINT p0_arc=interpolate_arc(cur,1);
        POINT p1_arc=interpolate_arc(next,(next.blocklenght-1));
        //! 1mm before arc endpoint.
        double ax=p0_arc.x;
        double ay=p0_arc.y;
        double az=p0_arc.z;
        double a[] = {ax, ay, az};

        //! Arc endpoint.
        double bx=cur.end.x;
        double by=cur.end.y;
        double bz=cur.end.z;
        double b[] = {bx, by, bz};

        //! 1mm on second Arc.
        double cx=p1_arc.x;
        double cy=p1_arc.y;
        double cz=p1_arc.z;
        double c[] = {cx, cy, cz};

        //! If angle < 1 degrees, vel_end=100%
        if(GetAngleABC(a,b,c)<5){
            //! 100% velocity.
            return 100;
        } else {
            //! 0% velocity.
            return 0;
        }
    }
    return 0;
}

//! Calculate's the block angle between 2 blocks in 3d space.
//! If block angle = 90 degrees, the machine has to stop.
//! If block angle < 90 degrees, machine may have end velocity.
//!
//! Function to find the angle between
double gcode_interface::GetAngleABC( double* a, double* b, double* c )
{
    /* Implemenation example. https://stackoverflow.com/questions/19729831/angle-between-3-points-in-3d-space
    double a[] = {1, 0, 0};
    double b[] = {0, 0, 0};
    double c[] = {0, 1, 0};
    std::cout<< "The angle of ABC is " << GetAngleABC(a,b,c)<< "º " << std::endl;*/

    double ab[3] = { b[0] - a[0], b[1] - a[1], b[2] - a[2] };
    double bc[3] = { c[0] - b[0], c[1] - b[1], c[2] - b[2]  };

    double abVec = sqrt(ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2]);
    double bcVec = sqrt(bc[0] * bc[0] + bc[1] * bc[1] + bc[2] * bc[2]);

    double abNorm[3] = {ab[0] / abVec, ab[1] / abVec, ab[2] / abVec};
    double bcNorm[3] = {bc[0] / bcVec, bc[1] / bcVec, bc[2] / bcVec};

    double res = abNorm[0] * bcNorm[0] + abNorm[1] * bcNorm[1] + abNorm[2] * bcNorm[2];

    return acos(res)*180.0/ M_PI;
}

//! Helper functions
BLOCK gcode_interface::line_lenght(BLOCK b){
    if(b.blocktype==BLOCKTYPE::G0 || b.blocktype==BLOCKTYPE::G1){
        b.blocklenght=sqrt(pow(b.end.x-b.start.x,2)+pow(b.end.y-b.start.y,2)+pow(b.end.z-b.start.z,2));
    }
    return b;
}

BLOCK gcode_interface::arc_lenght(BLOCK b){

    if(b.blocktype==BLOCKTYPE::G2 || b.blocktype==BLOCKTYPE::G3){
        double radius=sqrt(pow(b.end.x-b.center.x,2)+pow(b.end.y-b.center.y,2)+pow(b.end.z-b.center.z,2));
        double diameter=radius*2;
        double circumfence=diameter*M_PI;
        double start_angle, end_angle, arc_angle, arc_lenght;

        if(b.blocktype==BLOCKTYPE::G2){
            start_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
            end_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
        }

        if(b.blocktype==BLOCKTYPE::G3){
            start_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
            end_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
        }

        if(end_angle < start_angle){  //this to avoid the start angle is bigger then the end angle value
            end_angle += 2*M_PI;
        }

        arc_angle=end_angle-start_angle;
        arc_lenght=(arc_angle/(2*M_PI))*circumfence;
        b.blocklenght=arc_lenght;
    }
    return b;
}

//! Calculate's the euler xyz.
EULER gcode_interface::interpolate_euler_fwd(BLOCK b, double distancetogo)
{
    double ratio=0;
    ratio=1-(distancetogo/b.blocklenght);

    if(std::isnan(ratio)){
        ratio=0;
    }
    // std::cout<<"radio:"<<ratio<<std::endl;

    EULER e;
    e.x= b.euler_start.x + ((b.euler_end.x-b.euler_start.x)*ratio);
    e.y= b.euler_start.y + ((b.euler_end.y-b.euler_start.y)*ratio);
    e.z= b.euler_start.z + ((b.euler_end.z-b.euler_start.z)*ratio);

    return e;
}

//! Calculate's the euler xyz.
EULER gcode_interface::interpolate_euler_bck(BLOCK b, double distancetogo)
{
    double ratio=0;
    ratio=1-(distancetogo/b.blocklenght);

    if(std::isnan(ratio)){
        ratio=0;
    }
    // std::cout<<"radio:"<<ratio<<std::endl;

    EULER e;
    e.x= b.euler_end.x - ((b.euler_end.x-b.euler_start.x)*ratio);
    e.y= b.euler_end.y - ((b.euler_end.y-b.euler_start.y)*ratio);
    e.z= b.euler_end.z - ((b.euler_end.z-b.euler_start.z)*ratio);

    return e;
}

//! Calculate's the tcp on a line.
POINT gcode_interface::interpolate_line(BLOCK b, double distancetogo)
{
    double lenght=sqrt(pow(b.end.x-b.start.x,2)+pow(b.end.y-b.start.y,2)+pow(b.end.z-b.start.z,2));
    double ratio=0;

    ratio=1-(distancetogo/lenght);

    if(std::isnan(ratio)){
        ratio=0;
    }

    POINT p;
    p.x= b.start.x + ((b.end.x-b.start.x)*ratio);
    p.y= b.start.y + ((b.end.y-b.start.y)*ratio);
    p.z= b.start.z + ((b.end.z-b.start.z)*ratio);

    return p;
}

//! Calculate's the tcp on a line. Function checked ok.
POINT gcode_interface::interpolate_line_reversed(BLOCK b, double distancetogo)
{
    double lenght=sqrt(pow(b.end.x-b.start.x,2)+pow(b.end.y-b.start.y,2)+pow(b.end.z-b.start.z,2));
    double ratio=0;

    ratio=1-(distancetogo/lenght);

    if(std::isnan(ratio)){
        ratio=0;
    }

    POINT p;
    p.x= b.end.x - ((b.end.x-b.start.x)*ratio);
    p.y= b.end.y - ((b.end.y-b.start.y)*ratio);
    p.z= b.end.z - ((b.end.z-b.start.z)*ratio);

    return p;
}

//! Calculate's the tcp on a arc.
//! In future we will do this for a 3d arc with opencascade.
//! Therefore we have to update the gcode writing style first.
POINT gcode_interface::interpolate_arc(BLOCK b, double distancetogo){

    double radius=sqrt(pow(b.end.x-b.center.x,2)+pow(b.end.y-b.center.y,2)+pow(b.end.z-b.center.z,2));
    double diameter=radius*2;
    double circumfence=diameter*M_PI;
    double start_angle, end_angle, arc_angle, arc_lenght;

    if(b.blocktype==BLOCKTYPE::G2){
        start_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
        end_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
    }

    if(b.blocktype==BLOCKTYPE::G3){
        start_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
        end_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
    }

    if(end_angle < start_angle){  //!this to avoid the start angle is bigger then the end angle value
        end_angle += 2*M_PI;
    }

    arc_angle=end_angle-start_angle;
    arc_lenght=(arc_angle/(2*M_PI))*circumfence;

    double ratio=distancetogo/arc_lenght;

    POINT p;
    if(b.blocktype==BLOCKTYPE::G2){
        p.x = radius * cos(start_angle+(ratio*arc_angle));
        p.y = radius * sin(start_angle+(ratio*arc_angle));
    }
    if(b.blocktype==BLOCKTYPE::G3){
        p.x = radius * cos(start_angle+((1-ratio)*arc_angle));
        p.y = radius * sin(start_angle+((1-ratio)*arc_angle));
    }

    p.x+=b.center.x;
    p.y+=b.center.y;
    p.z=b.center.z;

    return p;
}

POINT gcode_interface::interpolate_arc_reversed(BLOCK b, double distancetogo){


    double radius=sqrt(pow(b.end.x-b.center.x,2)+pow(b.end.y-b.center.y,2)+pow(b.end.z-b.center.z,2));
    double diameter=radius*2;
    double circumfence=diameter*M_PI;
    double start_angle, end_angle, arc_angle, arc_lenght;

    if(b.blocktype==BLOCKTYPE::G2){
        start_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
        end_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
    }

    if(b.blocktype==BLOCKTYPE::G3){
        start_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
        end_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
    }

    if(end_angle < start_angle){
        end_angle += 2*M_PI;
    }

    arc_angle=end_angle-start_angle;
    arc_lenght=(arc_angle/(2*M_PI))*circumfence;

    double ratio=1-(distancetogo/arc_lenght);

    POINT p;
    if(b.blocktype==BLOCKTYPE::G2){
        p.x = radius * cos(start_angle+(ratio*arc_angle));
        p.y = radius * sin(start_angle+(ratio*arc_angle));
    }
    if(b.blocktype==BLOCKTYPE::G3){
        p.x = radius * cos(start_angle+((1-ratio)*arc_angle));
        p.y = radius * sin(start_angle+((1-ratio)*arc_angle));
    }

    p.x+=b.center.x;
    p.y+=b.center.y;
    p.z=b.center.z;

    return p;
}





















