//! This halmodule.c uses a shared c++ header file "external_var.h
//! This header file is written c,c++ compatible. Mention coding style standards like "struct" are used.
//!
//! The halmodule.h is a advanched coding technique for using c style kernel modules in combination with high end c++ libraries.
//! In the c++ section you can often see "extern "C"" in the code, this is the bridge between c and c++.
//! In the c++ section std::vectors are used. In c we use malloc to create a vector. So slightly different. But u get used to this.
//! Ok have fun, and take your time.
//!
//! Creaded by Skynet cyberdyne december 2021

#include "rtapi.h"
#ifdef RTAPI
#include "rtapi_app.h"
#endif
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "hal.h"
#include "rtapi_math64.h"
#include "rtapi_math.h"
#include "halmodule.h"

//! Ammount of machines, ammount of kinematic chains.
unsigned int machines;
RTAPI_MP_INT(machines,"");

////! Ammount of joints for a machines.
unsigned int joints[20];
RTAPI_MP_ARRAY_INT(joints,20,"");

////! Ammount of calculations to find the best kinematic solution.
int iterations;
RTAPI_MP_INT(iterations,"");

int baseperiod;
RTAPI_MP_INT(baseperiod,"");

int servoperiod;
RTAPI_MP_INT(servoperiod,"");

////! Print info to terminal.
//int screen_update_ms;
//RTAPI_MP_INT(screen_update_ms,"");

char *transparancy;
RTAPI_MP_STRING(transparancy,"");

//! stringvec=first, second, thirdh
//! rtapi_print_msg(RTAPI_MSG_ERR,"stepfiles c: %s \n",stringvec[0].string);
struct STRINGVEC{
    char *string;
};
struct STRINGVEC stringvec[100];
RTAPI_MP_ARRAY_STRING(stringvec,100,"");

struct JOINTVEC{
    char *string;
};
struct JOINTVEC jointposvec[150];
RTAPI_MP_ARRAY_STRING(jointposvec,150,"");

struct JOINTVAL{
    char *string;
};
struct JOINTVAL jointvalvec[100];
RTAPI_MP_ARRAY_STRING(jointvalvec,100,"");

struct MACHINEVAL{
    char *string;
};
struct MACHINEVAL machinevalvec[100];
RTAPI_MP_ARRAY_STRING(machinevalvec,100,"");

struct MACHINEOFFSET{
    char *string;
};
struct MACHINEOFFSET machineoffsetvec[100];
RTAPI_MP_ARRAY_STRING(machineoffsetvec,100,"");

////! Print info to terminal.
//int debug;
//RTAPI_MP_INT(debug,"");

//! Hal component int id.
static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

//! Hal float pin in-out
typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *pos_abs;

//! Hal bool pin in-out
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t *m3,*m5,*m7,*m8,*m9;

//! Int pin in-out
typedef struct {
    hal_s32_t *Pin;
} s32_data_t;

//! Unsigned int pin in-out
typedef struct {
    hal_u32_t *Pin;
} u32_data_t; 

//! Char array port in-out
typedef struct {
    hal_port_t *Pin;
} port_data_t;

//! Parameter pin rw float
typedef struct {
    hal_float_t Pin;
} param_float_data_t;

//! Parameter pin rw bool
typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;


static int init();
static void mainfunction();
static void follow();
static int setup_pins();
static void write_pins();
static void read_pins();

bool gui,queue_btn_event;
struct DATA *ptr, *queue_btn_ptr;

//! Functional data manipulation:
//! P3=P1-P2.
struct POINT point_min_point(struct POINT p1, struct POINT p2){
    struct POINT p3;
    p3.x=p1.x-p2.x;
    p3.y=p1.y-p2.y;
    p3.z=p1.z-p2.z;
    return p3;
}

//! P3=P1+P2.
struct POINT point_plus_point(struct POINT p1, struct POINT p2){
    struct POINT p3;
    p3.x=p1.x+p2.x;
    p3.y=p1.y+p2.y;
    p3.z=p1.z+p2.z;
    return p3;
}

//! EUL3=EUL1-EUL2.
struct EULER euler_min_euler(struct EULER eul1, struct EULER eul2){
    struct EULER eul3;
    eul3.x=eul1.x-eul2.x;
    eul3.y=eul1.y-eul2.y;
    eul3.z=eul1.z-eul2.z;
    return eul3;
}


//! EUL3=EUL1+EUL2.
struct EULER euler_plus_euler(struct EULER eul1, struct EULER eul2){
    struct EULER eul3;
    eul3.x=eul1.x+eul2.x;
    eul3.y=eul1.y+eul2.y;
    eul3.z=eul1.z+eul2.z;
    return eul3;
}

void copy_matrix(double m_in[4][4],double *m_out[4][4]){
    for(unsigned int i=0; i<4; i++){
        for(unsigned int j=0; j<4; j++){
            *m_out[i][j]=m_in[i][j];
        }
    }
}


struct POINT translate_matrix(double matrix[4][4], double x_translate, double y_translate, double z_translate){

    /*
    double matrix[4][4] = {
        {1, 0, 0, 0}, //[1 0 0 x]
        {0, 1, 0, 0}, //[0 1 0 y]
        {0, 0, 1, 0}, //[0 0 1 z]
        {0, 0, 0, 1}  //[0 0 0 1]
    };
    matrix[0][3]=p.x;
    matrix[1][3]=p.y;
    matrix[2][3]=p.z;*/

    double vector[4] = {0, 0, 0, 1}; //x,y,z,w //w=1 for translate, w=0 for direction
    vector[0]=x_translate;
    vector[1]=y_translate;
    vector[2]=z_translate;

    double x=(matrix[0][0]*vector[0])+
            (matrix[0][1]*vector[1])+
            (matrix[0][2]*vector[2])+
            (matrix[0][3]*vector[3]);

    double y=(matrix[1][0]*vector[0])+
            (matrix[1][1]*vector[1])+
            (matrix[1][2]*vector[2])+
            (matrix[1][3]*vector[3]);

    double z=(matrix[2][0]*vector[0])+
            (matrix[2][1]*vector[1])+
            (matrix[2][2]*vector[2])+
            (matrix[2][3]*vector[3]);
    /*
    double w=(matrix[3][0]*vector[0])+
            (matrix[3][1]*vector[1])+
            (matrix[3][2]*vector[2])+
            (matrix[3][3]*vector[3]);*/

    struct POINT result;
    result.x=x;
    result.y=y;
    result.z=z;
    return result;
}


int rtapi_app_main(void) {

    comp_idx = hal_init("matrix");
    if(comp_idx < 0) return comp_idx;

    int r = 0;
    //! Servo thread 1ms
    r = hal_export_funct("matrix.mainfunction", mainfunction, &skynet,0,0,comp_idx);
    //! Base thread 1000*servo_thread
    r = hal_export_funct("matrix.follow", follow, &skynet,0,0,comp_idx);
    r+=setup_pins();
    r+=init();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }

    return r;
}

void rtapi_app_exit(void) {
    hal_exit(comp_idx);
    free(ptr);
}

static int init(){
    int r=0;

    ptr = (struct DATA *)malloc(sizeof(struct DATA));
    ptr->machines=machines;

    //! Allocate machines
    for(unsigned int i=0; i<machines; i++){
        ptr->machine = (struct MACHINE *)malloc(machines * sizeof(struct MACHINE));
    }

    //! Allocate joints for every machine.
    for(unsigned int i=0; i<machines; i++){
        ptr->machine[i].joint = (struct JOINT *)malloc(joints[i] * sizeof(struct JOINT));
        ptr->machine[i].joints=joints[i];
    }

    //! Load the stepfiles and joint value's into the *ptr.
    int s=0;
    int h=0;
    int k=0;
    int o=0;
    int b=0;
    for(unsigned int i=0; i<machines; i++){
        //! initialize in fk mode.
        ptr->machine[i].kinmode=forward_kinematic;

        strcpy(ptr->machine[i].stepfile,stringvec[s].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"static stepfile: %s \n",ptr->machine[i].stepfile);
        s++;

        if(atof(transparancy)==0){strcpy(transparancy,"1");}
        ptr->machine[i].transparancy=atof(transparancy);
        ptr->iterations=iterations;
        ptr->baseperiod=baseperiod;
        ptr->servoperiod=servoperiod;

        //! Machineoffsetvec
        ptr->machine[i].offset_cart.x=atof(machineoffsetvec[o].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine offset cartx: %f \n",atof(machineoffsetvec[o].string));
        o++;
        ptr->machine[i].offset_cart.y=atof(machineoffsetvec[o].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine offset carty: %f \n",atof(machineoffsetvec[o].string));
        o++;
        ptr->machine[i].offset_cart.z=atof(machineoffsetvec[o].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine offset cartz: %f \n",atof(machineoffsetvec[o].string));
        o++;
        ptr->machine[i].offset_euler.x=atof(machineoffsetvec[o].string);
        ptr->machine[i].offset_euler.x=ptr->machine[i].offset_euler.x*toRadians;
        rtapi_print_msg(RTAPI_MSG_ERR,"machine offset eulerx: %f \n",atof(machineoffsetvec[o].string));
        o++;
        ptr->machine[i].offset_euler.y=atof(machineoffsetvec[o].string);
        ptr->machine[i].offset_euler.y=ptr->machine[i].offset_euler.y*toRadians;
        rtapi_print_msg(RTAPI_MSG_ERR,"machine offset eulery: %f \n",atof(machineoffsetvec[o].string));
        o++;
        ptr->machine[i].offset_euler.z=atof(machineoffsetvec[o].string);
        ptr->machine[i].offset_euler.z=ptr->machine[i].offset_euler.z*toRadians;
        rtapi_print_msg(RTAPI_MSG_ERR,"machine offset eulerz: %f \n",atof(machineoffsetvec[o].string));
        o++;

        //! Machinevalvec
        ptr->machine[i].maxvel_cart=atof(machinevalvec[b].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine maxvel_cart: %f \n",atof(machinevalvec[b].string));
        b++;

        ptr->machine[i].maxacc_cart=atof(machinevalvec[b].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine maxacc_cart: %f \n",atof(machinevalvec[b].string));
        b++;

        ptr->machine[i].gain_cart=atof(machinevalvec[b].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine jerk_cart: %f \n",atof(machinevalvec[b].string));
        b++;

        ptr->machine[i].maxvel_euler=atof(machinevalvec[b].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine maxvel_cart: %f \n",atof(machinevalvec[b].string));
        b++;

        ptr->machine[i].maxacc_euler=atof(machinevalvec[b].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine maxacc_cart: %f \n",atof(machinevalvec[b].string));
        b++;

        ptr->machine[i].gain_euler=atof(machinevalvec[b].string);
        rtapi_print_msg(RTAPI_MSG_ERR,"machine jerk_cart: %f \n",atof(machinevalvec[b].string));
        b++;


        for(unsigned int j=0; j< ptr->machine[i].joints; j++){
            strcpy(ptr->machine[i].joint[j].stepfile,stringvec[s].string);
            rtapi_print_msg(RTAPI_MSG_ERR,"joint stepfile: %s \n",ptr->machine[i].joint[j].stepfile);
            s++;


            rtapi_print_msg(RTAPI_MSG_ERR,"Machine: %d \n",i);
            rtapi_print_msg(RTAPI_MSG_ERR,"Joint:%u \n",j);
            rtapi_print_msg(RTAPI_MSG_ERR,"x: %f \n",atof(jointposvec[h].string));
            ptr->machine[i].joint[j].axis_endpoint.x=atof(jointposvec[h].string);
            h++;
            rtapi_print_msg(RTAPI_MSG_ERR,"y: %f \n",atof(jointposvec[h].string));
            ptr->machine[i].joint[j].axis_endpoint.y=atof(jointposvec[h].string);
            h++;
            rtapi_print_msg(RTAPI_MSG_ERR,"z: %f \n",atof(jointposvec[h].string));
            ptr->machine[i].joint[j].axis_endpoint.z=atof(jointposvec[h].string);
            h++;
            rtapi_print_msg(RTAPI_MSG_ERR,"rot: %f \n",atof(jointposvec[h].string));
            ptr->machine[i].joint[j].rotationtype=atof(jointposvec[h].string);
            h++;


            ptr->machine[i].joint[j].min=atof(jointvalvec[k].string);
            rtapi_print_msg(RTAPI_MSG_ERR,"min: %f \n",atof(jointvalvec[k].string));
            k++;
            ptr->machine[i].joint[j].max=atof(jointvalvec[k].string);
            rtapi_print_msg(RTAPI_MSG_ERR,"max: %f \n",atof(jointvalvec[k].string));
            k++;
            ptr->machine[i].joint[j].maxvel=atof(jointvalvec[k].string);
            rtapi_print_msg(RTAPI_MSG_ERR,"maxvel: %f \n",atof(jointvalvec[k].string));
            k++;
            ptr->machine[i].joint[j].maxacc=atof(jointvalvec[k].string);
            rtapi_print_msg(RTAPI_MSG_ERR,"maxacc: %f \n",atof(jointvalvec[k].string));
            k++;
            ptr->machine[i].joint[j].jerk=atof(jointvalvec[k].string);
            rtapi_print_msg(RTAPI_MSG_ERR,"jerk: %f \n",atof(jointvalvec[k].string));
            k++;

            //! Tryout
            ptr->machine[i].joint[j].curpos_abs=0;
            ptr->machine[i].joint[j].curpos_rel=0;
        }
        //! Tryout
        ptr->machine[i].abs_euler.z=0;
        ptr->machine[i].abs_euler.y=0;
        ptr->machine[i].abs_euler.x=0;
        ptr->machine[i].rel_euler.z=0;
        ptr->machine[i].rel_euler.y=0;
        ptr->machine[i].rel_euler.x=0;
    }

    *ptr=fk(ptr);
    start_gui(ptr);

    if(ptr == NULL){
        //rtapi_print_msg(RTAPI_MSG_ERR,"error loading dynamic memory in halmodule.c, init function, exit now.\n");
        return 1;
    } else {
        //rtapi_print_msg(RTAPI_MSG_ERR,"loading dynamic memory ok\n");
    }

    return r;
}

extern void btn_event_from_cpp_to_c(struct DATA *p){

    queue_btn_ptr=p;
    queue_btn_event=1;
}


static void mainfunction(){

    read_pins();

    //! Look for btn events.
    if(queue_btn_event){
        //! Update only the c++ signal & slot values to the c *ptr.
        for(unsigned int i=0; i<machines; i++){

            ptr->machine[i].stepsize=queue_btn_ptr->machine[i].stepsize;

            //! Zero relative position for rel_cart & rel_euler.
            //! This copy new offset value for rel_cart & rel_euler.
            ptr->machine[i].btn_zero_rel_all=queue_btn_ptr->machine[i].btn_zero_rel_all;
            if(ptr->machine[i].btn_zero_rel_all){
                ptr->machine[i].rel_offset_cart=ptr->machine[i].abs_cart;
                ptr->machine[i].rel_offset_euler=ptr->machine[i].abs_euler;

                ptr->machine[i].btn_zero_rel_J0=1;
                ptr->machine[i].btn_zero_rel_J1=1;
                ptr->machine[i].btn_zero_rel_J2=1;
                ptr->machine[i].btn_zero_rel_J3=1;
                ptr->machine[i].btn_zero_rel_J4=1;
                ptr->machine[i].btn_zero_rel_J5=1;

                ptr->machine[i].btn_zero_rel_all=0;
            }

            ptr->machine[i].btn_zero_rel_cartx=queue_btn_ptr->machine[i].btn_zero_rel_cartx;
            if(ptr->machine[i].btn_zero_rel_cartx){
                ptr->machine[i].rel_offset_cart.x=ptr->machine[i].abs_cart.x;
                ptr->machine[i].btn_zero_rel_cartx=0;
            }
            ptr->machine[i].btn_zero_rel_carty=queue_btn_ptr->machine[i].btn_zero_rel_carty;
            if(ptr->machine[i].btn_zero_rel_carty){
                ptr->machine[i].rel_offset_cart.y=ptr->machine[i].abs_cart.y;
                ptr->machine[i].btn_zero_rel_carty=0;
            }
            ptr->machine[i].btn_zero_rel_cartz=queue_btn_ptr->machine[i].btn_zero_rel_cartz;
            if(ptr->machine[i].btn_zero_rel_cartz){
                ptr->machine[i].rel_offset_cart.z=ptr->machine[i].abs_cart.z;
                ptr->machine[i].btn_zero_rel_cartz=0;
            }
            ptr->machine[i].btn_zero_rel_eulerx=queue_btn_ptr->machine[i].btn_zero_rel_eulerx;
            if(ptr->machine[i].btn_zero_rel_eulerx){
                ptr->machine[i].rel_offset_euler.x=ptr->machine[i].abs_euler.x;
                ptr->machine[i].btn_zero_rel_eulerx=0;
            }
            ptr->machine[i].btn_zero_rel_eulery=queue_btn_ptr->machine[i].btn_zero_rel_eulery;
            if(ptr->machine[i].btn_zero_rel_eulery){
                ptr->machine[i].rel_offset_euler.y=ptr->machine[i].abs_euler.y;
                ptr->machine[i].btn_zero_rel_eulery=0;
            }
            ptr->machine[i].btn_zero_rel_eulerz=queue_btn_ptr->machine[i].btn_zero_rel_eulerz;
            if(ptr->machine[i].btn_zero_rel_eulerz){
                ptr->machine[i].rel_offset_euler.z=ptr->machine[i].abs_euler.z;
                ptr->machine[i].btn_zero_rel_eulerz=0;
            }
            ptr->machine[i].btn_zero_rel_J0=queue_btn_ptr->machine[i].btn_zero_rel_J0;
            if(ptr->machine[i].btn_zero_rel_J0){
                ptr->machine[i].joint[0].rel_offset=ptr->machine[i].joint[0].curpos_abs;
                ptr->machine[i].btn_zero_rel_J0=0;
            }
            ptr->machine[i].btn_zero_rel_J1=queue_btn_ptr->machine[i].btn_zero_rel_J1;
            if(ptr->machine[i].btn_zero_rel_J1){
                ptr->machine[i].joint[1].rel_offset=ptr->machine[i].joint[1].curpos_abs;
                ptr->machine[i].btn_zero_rel_J1=0;
            }
            ptr->machine[i].btn_zero_rel_J2=queue_btn_ptr->machine[i].btn_zero_rel_J2;
            if(ptr->machine[i].btn_zero_rel_J2){
                ptr->machine[i].joint[2].rel_offset=ptr->machine[i].joint[2].curpos_abs;
                ptr->machine[i].btn_zero_rel_J2=0;
            }
            ptr->machine[i].btn_zero_rel_J3=queue_btn_ptr->machine[i].btn_zero_rel_J3;
            if(ptr->machine[i].btn_zero_rel_J3){
                ptr->machine[i].joint[3].rel_offset=ptr->machine[i].joint[3].curpos_abs;
                ptr->machine[i].btn_zero_rel_J3=0;
            }
            ptr->machine[i].btn_zero_rel_J4=queue_btn_ptr->machine[i].btn_zero_rel_J4;
            if(ptr->machine[i].btn_zero_rel_J4){
                ptr->machine[i].joint[4].rel_offset=ptr->machine[i].joint[4].curpos_abs;
                ptr->machine[i].btn_zero_rel_J4=0;
            }
            ptr->machine[i].btn_zero_rel_J5=queue_btn_ptr->machine[i].btn_zero_rel_J5;
            if(ptr->machine[i].btn_zero_rel_J5){
                ptr->machine[i].joint[5].rel_offset=ptr->machine[i].joint[5].curpos_abs;
                ptr->machine[i].btn_zero_rel_J5=0;
            }

            //! Different enum states are transferred to the halmodule.c.
            //! The mode feedback's are done in the mainwindow::mainloop.

            //! Mode auto
            ptr->machine[i].btn_mode_auto=queue_btn_ptr->machine[i].btn_mode_auto;
            if(ptr->machine[i].btn_mode_auto){
                ptr->machine[i].state_machine=AUTO;
                ptr->machine[i].btn_mode_auto=0;
            }
            //! Mode manual
            ptr->machine[i].btn_mode_manual=queue_btn_ptr->machine[i].btn_mode_manual;
            if(ptr->machine[i].btn_mode_manual){
                ptr->machine[i].state_machine=MANUAL;
                ptr->machine[i].btn_mode_manual=0;
            }
            //! Mode mdi
            ptr->machine[i].btn_mode_mdi=queue_btn_ptr->machine[i].btn_mode_mdi;
            if(ptr->machine[i].btn_mode_mdi){
                ptr->machine[i].state_machine=MDI;
                ptr->machine[i].btn_mode_mdi=0;
            }
            //! Estop
            ptr->machine[i].btn_estop=queue_btn_ptr->machine[i].btn_estop;
            if(ptr->machine[i].btn_estop){
                ptr->machine[i].state_machine=ESTOP;
                ptr->machine[i].btn_estop=0;
            }
            //! Run (program start)
            ptr->machine[i].btn_run=queue_btn_ptr->machine[i].btn_run;
            if(ptr->machine[i].btn_run){
                ptr->machine[i].state_program=RUN;
                ptr->machine[i].btn_run=0;
            }
            //! Stop (program stop)
            ptr->machine[i].btn_stop=queue_btn_ptr->machine[i].btn_stop;
            if(ptr->machine[i].btn_stop){
                ptr->machine[i].state_program=STOP;
                ptr->machine[i].btn_stop=0;
            }
            //! Pause, resume (program start)
            ptr->machine[i].btn_pause_resume=queue_btn_ptr->machine[i].btn_pause_resume;
            if(ptr->machine[i].btn_pause_resume){
                ptr->machine[i].state_program=PAUSE_RESUME;
                ptr->machine[i].btn_pause_resume=0;
            }
            //! Mode offline
            ptr->machine[i].btn_offline=queue_btn_ptr->machine[i].btn_offline;
            if(ptr->machine[i].btn_offline){
                ptr->machine[i].state_realtime=OFFLINE;
                ptr->machine[i].btn_offline=0;
            }
            //! Mode online
            ptr->machine[i].btn_online=queue_btn_ptr->machine[i].btn_online;
            if(ptr->machine[i].btn_online){
                ptr->machine[i].state_realtime=ONLINE;
                ptr->machine[i].btn_online=0;
            }

            //! Jog with scurve motion.
            if(ptr->machine[i].state_machine==MANUAL && ptr->machine[i].state_realtime==ONLINE){
                //! Jog joints with scurve.
                for(unsigned int j=0; j<joints[i]; j++){
                    bool rot=0;
                    //! # rotationtype : rot_x = 0, rot_y = 1, rot_z = 2, trans_x = 3, trans_y = 4, trans_z = 5, rot_axis = 6, trans_axis =7, none = 8
                    if(queue_btn_ptr->machine[i].joint[j].rotationtype==0 || queue_btn_ptr->machine[i].joint[j].rotationtype==1 || queue_btn_ptr->machine[i].joint[j].rotationtype==2 || queue_btn_ptr->machine[i].joint[j].rotationtype==6){
                        rot=1;
                    }

                    ptr->machine[i].joint[j].joint_plus=queue_btn_ptr->machine[i].joint[j].joint_plus;
                    ptr->machine[i].joint[j].joint_min=queue_btn_ptr->machine[i].joint[j].joint_min;

                    if(ptr->machine[i].joint[j].joint_plus && !ptr->machine[i].joint[j].jog_joint_init){
                        ptr->machine[i].kinmode=forward_kinematic;
                        ptr->machine[i].joint[j].jog_joint_init=1;
                        if(rot){
                            ptr->machine[i].joint[j].scurve_abs_joint_init=ptr->machine[i].joint[j].curpos_abs;
                            ptr->machine[i].joint[j].scurve_abs_joint_request=ptr->machine[i].joint[j].curpos_abs+(ptr->machine[i].stepsize*toRadians);
                            ptr->machine[i].joint[j].rot=1;

                        } else {
                            ptr->machine[i].joint[j].scurve_abs_joint_init=ptr->machine[i].joint[j].curpos_abs;
                            ptr->machine[i].joint[j].scurve_abs_joint_request=ptr->machine[i].joint[j].curpos_abs+ptr->machine[i].stepsize;
                            ptr->machine[i].joint[j].rot=0;
                        }
                        //! Reset jog timer.
                        ptr->machine[i].joint[j].at_jog_time=0;
                        ptr->machine[i].joint[j].joint_plus=0;
                    }
                    if(ptr->machine[i].joint[j].joint_min && !ptr->machine[i].joint[j].jog_joint_init){
                        ptr->machine[i].kinmode=forward_kinematic;
                        ptr->machine[i].joint[j].jog_joint_init=1;
                        if(rot){
                            ptr->machine[i].joint[j].scurve_abs_joint_init=ptr->machine[i].joint[j].curpos_abs;
                            ptr->machine[i].joint[j].scurve_abs_joint_request=ptr->machine[i].joint[j].curpos_abs-(ptr->machine[i].stepsize*toRadians);
                            ptr->machine[i].joint[j].rot=1;
                        } else {
                            ptr->machine[i].joint[j].scurve_abs_joint_init=ptr->machine[i].joint[j].curpos_abs;
                            ptr->machine[i].joint[j].scurve_abs_joint_request=ptr->machine[i].joint[j].curpos_abs-ptr->machine[i].stepsize;
                            ptr->machine[i].joint[j].rot=0;
                        }
                        //! Reset jog timer.
                        ptr->machine[i].joint[j].at_jog_time=0;
                        ptr->machine[i].joint[j].joint_min=0;
                    }
                }

                //! Jog cartx with scurve.
                ptr->machine[i].btn_cartx_plus=queue_btn_ptr->machine[i].btn_cartx_plus;
                if(ptr->machine[i].btn_cartx_plus && !ptr->machine[i].jog_cartx_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_cartx_init=1;
                    ptr->machine[i].scurve_jog_abs_cartx_init=ptr->machine[i].abs_cart.x;
                    ptr->machine[i].scurve_jog_abs_cartx_request=ptr->machine[i].abs_cart.x+ptr->machine[i].stepsize;
                    //! Reset cart timer.
                    ptr->machine[i].at_cartx_time=0;
                    ptr->machine[i].btn_cartx_plus=0;
                }

                ptr->machine[i].btn_cartx_min=queue_btn_ptr->machine[i].btn_cartx_min;
                if(ptr->machine[i].btn_cartx_min && !ptr->machine[i].jog_cartx_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_cartx_init=1;
                    ptr->machine[i].scurve_jog_abs_cartx_init=ptr->machine[i].abs_cart.x;
                    ptr->machine[i].scurve_jog_abs_cartx_request=ptr->machine[i].abs_cart.x-ptr->machine[i].stepsize;
                    //! Reset cart timer.
                    ptr->machine[i].at_cartx_time=0;
                    ptr->machine[i].btn_cartx_min=0;
                }

                //! Jog carty with scurve.
                ptr->machine[i].btn_carty_plus=queue_btn_ptr->machine[i].btn_carty_plus;
                if(ptr->machine[i].btn_carty_plus && !ptr->machine[i].jog_carty_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_carty_init=1;
                    ptr->machine[i].scurve_jog_abs_carty_init=ptr->machine[i].abs_cart.y;
                    ptr->machine[i].scurve_jog_abs_carty_request=ptr->machine[i].abs_cart.y+ptr->machine[i].stepsize;
                    //! Reset cart timer.
                    ptr->machine[i].at_carty_time=0;
                    ptr->machine[i].btn_carty_plus=0;
                }

                ptr->machine[i].btn_carty_min=queue_btn_ptr->machine[i].btn_carty_min;
                if(ptr->machine[i].btn_carty_min && !ptr->machine[i].jog_carty_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_carty_init=1;
                    ptr->machine[i].scurve_jog_abs_carty_init=ptr->machine[i].abs_cart.y;
                    ptr->machine[i].scurve_jog_abs_carty_request=ptr->machine[i].abs_cart.y-ptr->machine[i].stepsize;
                    //! Reset cart timer.
                    ptr->machine[i].at_carty_time=0;
                    ptr->machine[i].btn_carty_min=0;
                }

                //! Jog cartz with scurve.
                ptr->machine[i].btn_cartz_plus=queue_btn_ptr->machine[i].btn_cartz_plus;
                if(ptr->machine[i].btn_cartz_plus && !ptr->machine[i].jog_cartz_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_cartz_init=1;
                    ptr->machine[i].scurve_jog_abs_cartz_init=ptr->machine[i].abs_cart.z;
                    ptr->machine[i].scurve_jog_abs_cartz_request=ptr->machine[i].abs_cart.z+ptr->machine[i].stepsize;
                    //! Reset cart timer.
                    ptr->machine[i].at_cartz_time=0;
                    ptr->machine[i].btn_cartz_plus=0;
                }

                ptr->machine[i].btn_cartz_min=queue_btn_ptr->machine[i].btn_cartz_min;
                if(ptr->machine[i].btn_cartz_min && !ptr->machine[i].jog_cartz_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_cartz_init=1;
                    ptr->machine[i].scurve_jog_abs_cartz_init=ptr->machine[i].abs_cart.z;
                    ptr->machine[i].scurve_jog_abs_cartz_request=ptr->machine[i].abs_cart.z-ptr->machine[i].stepsize;
                    //! Reset cart timer.
                    ptr->machine[i].at_cartz_time=0;
                    ptr->machine[i].btn_cartz_min=0;
                }

                ptr->machine[i].btn_eulerx_plus=queue_btn_ptr->machine[i].btn_eulerx_plus;
                if(ptr->machine[i].btn_eulerx_plus && !ptr->machine[i].jog_eulerx_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_x;
                    ptr->machine[i].jog_eulerx_init=1;
                    ptr->machine[i].scurve_jog_abs_eulerx_init=ptr->machine[i].abs_euler.x;
                    ptr->machine[i].scurve_jog_abs_eulerx_request=ptr->machine[i].abs_euler.x+ptr->machine[i].stepsize*toRadians;
                    //! Reset cart timer.
                    ptr->machine[i].at_eulerx_time=0;
                    ptr->machine[i].btn_eulerx_plus=0;
                }

                ptr->machine[i].btn_eulerx_min=queue_btn_ptr->machine[i].btn_eulerx_min;
                if(ptr->machine[i].btn_eulerx_min && !ptr->machine[i].jog_eulerx_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_x;
                    ptr->machine[i].jog_eulerx_init=1;
                    ptr->machine[i].scurve_jog_abs_eulerx_init=ptr->machine[i].abs_euler.x;
                    ptr->machine[i].scurve_jog_abs_eulerx_request=ptr->machine[i].abs_euler.x-ptr->machine[i].stepsize*toRadians;
                    //! Reset cart timer.
                    ptr->machine[i].at_eulerx_time=0;
                    ptr->machine[i].btn_eulerx_min=0;
                }

                ptr->machine[i].btn_eulery_plus=queue_btn_ptr->machine[i].btn_eulery_plus;
                if(ptr->machine[i].btn_eulery_plus && !ptr->machine[i].jog_eulery_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_y;
                    ptr->machine[i].jog_eulery_init=1;
                    ptr->machine[i].scurve_jog_abs_eulery_init=ptr->machine[i].abs_euler.y;
                    ptr->machine[i].scurve_jog_abs_eulery_request=ptr->machine[i].abs_euler.y+ptr->machine[i].stepsize*toRadians;
                    //! Reset cart timer.
                    ptr->machine[i].at_eulery_time=0;
                    ptr->machine[i].btn_eulery_plus=0;
                }

                ptr->machine[i].btn_eulery_min=queue_btn_ptr->machine[i].btn_eulery_min;
                if(ptr->machine[i].btn_eulery_min && !ptr->machine[i].jog_eulery_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_y;
                    ptr->machine[i].jog_eulery_init=1;
                    ptr->machine[i].scurve_jog_abs_eulery_init=ptr->machine[i].abs_euler.y;
                    ptr->machine[i].scurve_jog_abs_eulery_request=ptr->machine[i].abs_euler.y-ptr->machine[i].stepsize*toRadians;
                    //! Reset cart timer.
                    ptr->machine[i].at_eulery_time=0;
                    ptr->machine[i].btn_eulery_min=0;
                }

                ptr->machine[i].btn_eulerz_plus=queue_btn_ptr->machine[i].btn_eulerz_plus;
                if(ptr->machine[i].btn_eulerz_plus && !ptr->machine[i].jog_eulerz_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_z;
                    ptr->machine[i].jog_eulerz_init=1;
                    ptr->machine[i].scurve_jog_abs_eulerz_init=ptr->machine[i].abs_euler.z;
                    ptr->machine[i].scurve_jog_abs_eulerz_request=ptr->machine[i].abs_euler.z+ptr->machine[i].stepsize*toRadians;
                    //! Reset cart timer.
                    ptr->machine[i].at_eulerz_time=0;
                    ptr->machine[i].btn_eulerz_plus=0;
                }

                ptr->machine[i].btn_eulerz_min=queue_btn_ptr->machine[i].btn_eulerz_min;
                if(ptr->machine[i].btn_eulerz_min && !ptr->machine[i].jog_eulerz_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_z;
                    ptr->machine[i].jog_eulerz_init=1;
                    ptr->machine[i].scurve_jog_abs_eulerz_init=ptr->machine[i].abs_euler.z;
                    ptr->machine[i].scurve_jog_abs_eulerz_request=ptr->machine[i].abs_euler.z-ptr->machine[i].stepsize*toRadians;
                    //! Reset cart timer.
                    ptr->machine[i].at_eulerz_time=0;
                    ptr->machine[i].btn_eulerz_min=0;
                }

                ptr->machine[i].btn_toolx_plus=queue_btn_ptr->machine[i].btn_toolx_plus;
                if(ptr->machine[i].btn_toolx_plus && !ptr->machine[i].jog_toolx_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_toolx_init=1;

                    unsigned int last_joint=ptr->machine[i].joints-1;
                    //! 3d Translate the last matrix with our interconnecing 4x4 matrix.
                    //! Reference info about this used matrix functions can be found in /libskynet/matrix_3d.
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,ptr->machine[i].stepsize,0,0);
                    ptr->machine[i].scurve_jog_abs_toolx_init=ptr->machine[i].abs_cart;
                    ptr->machine[i].scurve_jog_abs_toolx_request=p;
                    //! Reset cart timer.
                    ptr->machine[i].at_toolx_time=0;
                    ptr->machine[i].btn_toolx_plus=0;
                }

                ptr->machine[i].btn_toolx_min=queue_btn_ptr->machine[i].btn_toolx_min;
                if(ptr->machine[i].btn_toolx_min && !ptr->machine[i].jog_toolx_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_toolx_init=1;

                    unsigned int last_joint=ptr->machine[i].joints-1;
                    //! 3d Translate the last matrix with our interconnecing 4x4 matrix.
                    //! Reference info about this used matrix functions can be found in /libskynet/matrix_3d.
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,-ptr->machine[i].stepsize,0,0);
                    ptr->machine[i].scurve_jog_abs_toolx_init=ptr->machine[i].abs_cart;
                    ptr->machine[i].scurve_jog_abs_toolx_request=p;
                    //! Reset cart timer.
                    ptr->machine[i].at_toolx_time=0;
                    ptr->machine[i].btn_toolx_min=0;
                }

                ptr->machine[i].btn_tooly_plus=queue_btn_ptr->machine[i].btn_tooly_plus;
                if(ptr->machine[i].btn_tooly_plus && !ptr->machine[i].jog_tooly_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_tooly_init=1;

                    unsigned int last_joint=ptr->machine[i].joints-1;
                    //! 3d Translate the last matrix with our interconnecing 4x4 matrix.
                    //! Reference info about this used matrix functions can be found in /libskynet/matrix_3d.
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,ptr->machine[i].stepsize,0);
                    ptr->machine[i].scurve_jog_abs_tooly_init=ptr->machine[i].abs_cart;
                    ptr->machine[i].scurve_jog_abs_tooly_request=p;
                    //! Reset cart timer.
                    ptr->machine[i].at_tooly_time=0;
                    ptr->machine[i].btn_tooly_plus=0;
                }

                ptr->machine[i].btn_tooly_min=queue_btn_ptr->machine[i].btn_tooly_min;
                if(ptr->machine[i].btn_tooly_min && !ptr->machine[i].jog_tooly_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_tooly_init=1;

                    unsigned int last_joint=ptr->machine[i].joints-1;
                    //! 3d Translate the last matrix with our interconnecing 4x4 matrix.
                    //! Reference info about this used matrix functions can be found in /libskynet/matrix_3d.
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,-ptr->machine[i].stepsize,0);
                    ptr->machine[i].scurve_jog_abs_tooly_init=ptr->machine[i].abs_cart;
                    ptr->machine[i].scurve_jog_abs_tooly_request=p;
                    //! Reset cart timer.
                    ptr->machine[i].at_tooly_time=0;
                    ptr->machine[i].btn_tooly_min=0;
                }

                ptr->machine[i].btn_toolz_plus=queue_btn_ptr->machine[i].btn_toolz_plus;
                if(ptr->machine[i].btn_toolz_plus && !ptr->machine[i].jog_toolz_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_toolz_init=1;

                    unsigned int last_joint=ptr->machine[i].joints-1;
                    //! 3d Translate the last matrix with our interconnecing 4x4 matrix.
                    //! Reference info about this used matrix functions can be found in /libskynet/matrix_3d.
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,0,ptr->machine[i].stepsize);
                    ptr->machine[i].scurve_jog_abs_toolz_init=ptr->machine[i].abs_cart;
                    ptr->machine[i].scurve_jog_abs_toolz_request=p;
                    //! Reset cart timer.
                    ptr->machine[i].at_toolz_time=0;
                    ptr->machine[i].btn_toolz_plus=0;
                }

                ptr->machine[i].btn_toolz_min=queue_btn_ptr->machine[i].btn_toolz_min;
                if(ptr->machine[i].btn_toolz_min && !ptr->machine[i].jog_toolz_init){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].jog_toolz_init=1;

                    unsigned int last_joint=ptr->machine[i].joints-1;
                    //! 3d Translate the last matrix with our interconnecing 4x4 matrix.
                    //! Reference info about this used matrix functions can be found in /libskynet/matrix_3d.
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,0,-ptr->machine[i].stepsize);
                    ptr->machine[i].scurve_jog_abs_toolz_init=ptr->machine[i].abs_cart;
                    ptr->machine[i].scurve_jog_abs_toolz_request=p;
                    //! Reset cart timer.
                    ptr->machine[i].at_toolz_time=0;
                    ptr->machine[i].btn_toolz_min=0;
                }

            }

            //! Jog without scurve motion, jump to endpoint when offline.
            if(ptr->machine[i].state_machine==MANUAL && ptr->machine[i].state_realtime==OFFLINE){
                for(unsigned int j=0; j<joints[i]; j++){
                    bool rot=0;
                    //! # rotationtype : rot_x = 0, rot_y = 1, rot_z = 2, trans_x = 3, trans_y = 4, trans_z = 5, rot_axis = 6, trans_axis =7, none = 8
                    if(queue_btn_ptr->machine[i].joint[j].rotationtype==0 || queue_btn_ptr->machine[i].joint[j].rotationtype==1 || queue_btn_ptr->machine[i].joint[j].rotationtype==2 || queue_btn_ptr->machine[i].joint[j].rotationtype==6){
                        rot=1;
                    }

                    ptr->machine[i].joint[j].joint_plus=queue_btn_ptr->machine[i].joint[j].joint_plus;
                    ptr->machine[i].joint[j].joint_min=queue_btn_ptr->machine[i].joint[j].joint_min;

                    if(ptr->machine[i].joint[j].joint_plus){
                        ptr->machine[i].kinmode=forward_kinematic;
                        if(rot){
                            ptr->machine[i].joint[j].curpos_abs+=ptr->machine[i].stepsize*toRadians;
                        } else {
                            ptr->machine[i].joint[j].curpos_abs+=ptr->machine[i].stepsize;
                        }
                        ptr->machine[i].joint[j].joint_plus=0;
                    }
                    if(ptr->machine[i].joint[j].joint_min){
                        ptr->machine[i].kinmode=forward_kinematic;
                        if(rot){
                            ptr->machine[i].joint[j].curpos_abs-=ptr->machine[i].stepsize*toRadians;
                        } else {
                            ptr->machine[i].joint[j].curpos_abs-=ptr->machine[i].stepsize;
                        }
                        ptr->machine[i].joint[j].joint_min=0;
                    }
                }

                ptr->machine[i].btn_cartx_plus=queue_btn_ptr->machine[i].btn_cartx_plus;
                if(ptr->machine[i].btn_cartx_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].abs_cart.x+=ptr->machine[i].stepsize;
                    ptr->machine[i].btn_cartx_plus=0;
                }

                ptr->machine[i].btn_cartx_min=queue_btn_ptr->machine[i].btn_cartx_min;
                if(ptr->machine[i].btn_cartx_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].abs_cart.x-=ptr->machine[i].stepsize;
                    ptr->machine[i].btn_cartx_min=0;
                }

                ptr->machine[i].btn_carty_plus=queue_btn_ptr->machine[i].btn_carty_plus;
                if(ptr->machine[i].btn_carty_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].abs_cart.y+=ptr->machine[i].stepsize;
                    ptr->machine[i].btn_carty_plus=0;
                }

                ptr->machine[i].btn_carty_min=queue_btn_ptr->machine[i].btn_carty_min;
                if(ptr->machine[i].btn_carty_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].abs_cart.y-=ptr->machine[i].stepsize;
                    ptr->machine[i].btn_carty_min=0;
                }

                ptr->machine[i].btn_cartz_plus=queue_btn_ptr->machine[i].btn_cartz_plus;
                if(ptr->machine[i].btn_cartz_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].abs_cart.z+=ptr->machine[i].stepsize;
                    ptr->machine[i].btn_cartz_plus=0;
                }

                ptr->machine[i].btn_cartz_min=queue_btn_ptr->machine[i].btn_cartz_min;
                if(ptr->machine[i].btn_cartz_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].abs_cart.z-=ptr->machine[i].stepsize;
                    ptr->machine[i].btn_cartz_min=0;
                }

                ptr->machine[i].btn_eulerx_plus=queue_btn_ptr->machine[i].btn_eulerx_plus;
                if(ptr->machine[i].btn_eulerx_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_x;
                    ptr->machine[i].abs_euler.x=queue_btn_ptr->machine[i].abs_euler.x;
                    ptr->machine[i].abs_euler.x+=ptr->machine[i].stepsize*toRadians;
                    ptr->machine[i].btn_eulerx_plus=0;
                }

                ptr->machine[i].btn_eulerx_min=queue_btn_ptr->machine[i].btn_eulerx_min;
                if(ptr->machine[i].btn_eulerx_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_x;
                    ptr->machine[i].abs_euler.x=queue_btn_ptr->machine[i].abs_euler.x;
                    ptr->machine[i].abs_euler.x-=ptr->machine[i].stepsize*toRadians;
                    ptr->machine[i].btn_eulerx_min=0;
                }

                ptr->machine[i].btn_eulery_plus=queue_btn_ptr->machine[i].btn_eulery_plus;
                if(ptr->machine[i].btn_eulery_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_y;
                    ptr->machine[i].abs_euler.y=queue_btn_ptr->machine[i].abs_euler.y;
                    ptr->machine[i].abs_euler.y+=ptr->machine[i].stepsize*toRadians;
                    ptr->machine[i].btn_eulery_plus=0;
                }

                ptr->machine[i].btn_eulery_min=queue_btn_ptr->machine[i].btn_eulery_min;
                if(ptr->machine[i].btn_eulery_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_y;
                    ptr->machine[i].abs_euler.y=queue_btn_ptr->machine[i].abs_euler.y;
                    ptr->machine[i].abs_euler.y-=ptr->machine[i].stepsize*toRadians;
                    ptr->machine[i].btn_eulery_min=0;
                }

                ptr->machine[i].btn_eulerz_plus=queue_btn_ptr->machine[i].btn_eulerz_plus;
                if(ptr->machine[i].btn_eulerz_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_z;
                    ptr->machine[i].abs_euler.z=queue_btn_ptr->machine[i].abs_euler.z;
                    ptr->machine[i].abs_euler.z+=ptr->machine[i].stepsize*toRadians;
                    ptr->machine[i].btn_eulerz_plus=0;
                }

                ptr->machine[i].btn_eulerz_min=queue_btn_ptr->machine[i].btn_eulerz_min;
                if(ptr->machine[i].btn_eulerz_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    ptr->machine[i].eulmode=rotation_z;
                    ptr->machine[i].abs_euler.z=queue_btn_ptr->machine[i].abs_euler.z;
                    ptr->machine[i].abs_euler.z-=ptr->machine[i].stepsize*toRadians;
                    ptr->machine[i].btn_eulerz_min=0;
                }

                ptr->machine[i].btn_toolx_plus=queue_btn_ptr->machine[i].btn_toolx_plus;
                if(ptr->machine[i].btn_toolx_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    unsigned int last_joint=ptr->machine[i].joints-1;
                    //! 3d Translate the last matrix with our interconnecing 4x4 matrix.
                    //! Reference info about this used matrix functions can be found in /libskynet/matrix_3d.
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,ptr->machine[i].stepsize,0,0);
                    // rtapi_print_msg(RTAPI_MSG_ERR,"point p.x: %f \n",p.x);
                    ptr->machine[i].abs_cart=p;
                    ptr->machine[i].btn_toolx_plus=0;
                }

                ptr->machine[i].btn_toolx_min=queue_btn_ptr->machine[i].btn_toolx_min;
                if(ptr->machine[i].btn_toolx_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    unsigned int last_joint=ptr->machine[i].joints-1;
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,-ptr->machine[i].stepsize,0,0);
                    ptr->machine[i].abs_cart=p;
                    ptr->machine[i].btn_toolx_min=0;
                }

                ptr->machine[i].btn_tooly_plus=queue_btn_ptr->machine[i].btn_tooly_plus;
                if(ptr->machine[i].btn_tooly_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    unsigned int last_joint=ptr->machine[i].joints-1;
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,ptr->machine[i].stepsize,0);
                    ptr->machine[i].abs_cart=p;
                    ptr->machine[i].btn_tooly_plus=0;
                }

                ptr->machine[i].btn_tooly_min=queue_btn_ptr->machine[i].btn_tooly_min;
                if(ptr->machine[i].btn_tooly_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    unsigned int last_joint=ptr->machine[i].joints-1;
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,-ptr->machine[i].stepsize,0);
                    ptr->machine[i].abs_cart=p;
                    ptr->machine[i].btn_tooly_min=0;
                }

                ptr->machine[i].btn_toolz_plus=queue_btn_ptr->machine[i].btn_toolz_plus;
                if(ptr->machine[i].btn_toolz_plus){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    unsigned int last_joint=ptr->machine[i].joints-1;
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,0,ptr->machine[i].stepsize);
                    ptr->machine[i].abs_cart=p;
                    ptr->machine[i].btn_toolz_plus=0;
                }

                ptr->machine[i].btn_toolz_min=queue_btn_ptr->machine[i].btn_toolz_min;
                if(ptr->machine[i].btn_toolz_min){
                    ptr->machine[i].kinmode=inverse_kinematic;
                    unsigned int last_joint=ptr->machine[i].joints-1;
                    struct POINT p=translate_matrix(queue_btn_ptr->machine[i].joint[last_joint].matrix,0,0,-ptr->machine[i].stepsize);
                    ptr->machine[i].abs_cart=p;
                    ptr->machine[i].btn_toolz_min=0;
                }
            }
        }

        //! Reset queue.
        queue_btn_event=0;
        // rtapi_print_msg(RTAPI_MSG_ERR,"We updated a btn event from the mainwindow! \n");
    }

    //! Calculate the current relative offset position to update the rel_lineedits in the gui.
    for(unsigned int i=0; i<machines; i++){
        ptr->machine[i].rel_cart=point_min_point(ptr->machine[i].abs_cart,ptr->machine[i].rel_offset_cart);
        ptr->machine[i].rel_euler=euler_min_euler(ptr->machine[i].abs_euler,ptr->machine[i].rel_offset_euler);

        for(unsigned int j=0; j<joints[i]; j++){
            ptr->machine[i].joint[j].curpos_rel= ptr->machine[i].joint[j].curpos_abs-ptr->machine[i].joint[j].rel_offset;
        }
    }

    //! Motion, is machine offline or online.
    for(unsigned int i=0; i<machines; i++){

        //! In this mode program re-compile goes faster.
        if(ptr->machine[i].state_realtime==OFFLINE){
            if(ptr->machine[i].state_machine==MANUAL){
                if(ptr->machine[i].kinmode==forward_kinematic){
                    *ptr=fk(ptr);
                }
                if(ptr->machine[i].kinmode==inverse_kinematic){
                    *ptr=ik(ptr);
                }
            }
        }

        if(ptr->machine[i].state_realtime==ONLINE){
            if(ptr->machine[i].state_machine==AUTO || ptr->machine[i].state_machine==MDI){
                ptr->machine[i].kinmode=inverse_kinematic;
                //! Gcode runner.
                *ptr=gr(ptr);
                //! Inverse kinematic.
                *ptr=ik(ptr);
            }

            if(ptr->machine[i].state_machine==MANUAL){
                if(ptr->machine[i].kinmode==forward_kinematic){
                    *ptr=mr(ptr);
                    *ptr=fk(ptr);
                }
                if(ptr->machine[i].kinmode==inverse_kinematic){
                    *ptr=mr(ptr);
                    *ptr=ik(ptr);
                }
            }
        }
    }

    //! Update gui values
    c_to_cpp_ptr(ptr);


    write_pins();
}

//! Base thread. Ultra fast kernel thread.
static void follow(){

}

static void read_pins(){

}

static void write_pins(){
    //! A example how to write hal pins.
    for(unsigned int i=0; i<machines; i++){
        if(ptr->machine[i].gcode_macro_nr==3){
            *m3[i].Pin=true;
            *m5[i].Pin=false;
        }
        if(ptr->machine[i].gcode_macro_nr==5){
            *m3[i].Pin=false;
            *m5[i].Pin=true;
        }
        if(ptr->machine[i].gcode_macro_nr==7){
            *m7[i].Pin=true;
            *m9[i].Pin=false;
        }
        if(ptr->machine[i].gcode_macro_nr==8){
            *m8[i].Pin=true;
            *m9[i].Pin=false;
        }
        if(ptr->machine[i].gcode_macro_nr==9){
            *m7[i].Pin=true;
            *m8[i].Pin=true;
            *m9[i].Pin=false;
        }
    }

    int k=0;
    for(unsigned int i=0; i<machines; i++){
        for(unsigned int j=0; j<joints[i]; j++){
            *pos_abs[k].Pin=ptr->machine[i].joint[j].curpos_abs;
            k++;
        }
    }
}

static int setup_pins(){
    int r=0;
    //! A example how to setup pins and reserve memory.
    m3=(bit_data_t*)hal_malloc(machines* sizeof(bit_data_t));
    m5=(bit_data_t*)hal_malloc(machines* sizeof(bit_data_t));
    m7=(bit_data_t*)hal_malloc(machines* sizeof(bit_data_t));
    m8=(bit_data_t*)hal_malloc(machines* sizeof(bit_data_t));
    m9=(bit_data_t*)hal_malloc(machines* sizeof(bit_data_t));

    //! Allocate dynamic memory
    int jointsize=0;
    for(unsigned int i=0; i<machines; i++){
        for(unsigned int j=0; j<joints[i]; j++){
            jointsize++;
        }
    }
    pos_abs=(float_data_t*)hal_malloc(jointsize* sizeof(float_data_t));

    int k=0;
    for(unsigned int i=0; i<machines; i++){

        char str[100]={};
        strcat(str,"matrix.");
        char nr1[3];
        sprintf(nr1, "%d", i);
        strcat(str,nr1);
        strcat(str,".");

        char text[100]={};
        strcpy(text,str);
        strcat(text,"m3");
        r+=hal_pin_bit_new(text,HAL_OUT,&(m3[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"m5");
        r+=hal_pin_bit_new(text,HAL_OUT,&(m5[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"m7");
        r+=hal_pin_bit_new(text,HAL_OUT,&(m7[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"m8");
        r+=hal_pin_bit_new(text,HAL_OUT,&(m8[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"m9");
        r+=hal_pin_bit_new(text,HAL_OUT,&(m9[i].Pin),comp_idx);

        for(unsigned int j=0; j<joints[i]; j++){
            char str[100]={};
            strcat(str,"matrix.");
            char nr1[3], nr2[3];
            sprintf(nr1, "%d", i);
            sprintf(nr2, "%d", j);
            strcat(str,nr1);
            strcat(str,".");
            strcat(str,nr2);

            char text[100]={};
            strcpy(text,str);
            strcat(text,".pos-abs");
            r+=hal_pin_float_new(text,HAL_OUT,&(pos_abs[k].Pin),comp_idx);

            k++;
        }
    }
    return r;
}






















