/// Improved by Skynet 03-11-2021
/// Changed from halcompile cody style to c code style.
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
int machines;
RTAPI_MP_INT(machines,"");

//! Ammount of joints for a machines.
int joints[20];
RTAPI_MP_ARRAY_INT(joints,20,"");

//! Ammount of calculations to find the best kinematic solution.
int iterations;
RTAPI_MP_INT(iterations,"");

//! Print info to terminal.
int screen_update_ms;
RTAPI_MP_INT(screen_update_ms,"");

char *stepfilepath;
RTAPI_MP_STRING(stepfilepath,"");

//! Print info to terminal.
int debug;
RTAPI_MP_INT(debug,"");

//! Hal component int id.
static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynets;

//! Hal float pin in-out
typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *joint_out;
float_data_t *joint_acc;

//! Hal bool pin in-out
typedef struct {
    hal_bit_t *Pin;
} bit_data_t;

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
//! Kinematic & Opencascade parameters.
param_float_data_t *joint_init;
param_float_data_t *joint_min;
param_float_data_t *joint_max;
param_float_data_t *joint_setup_x;
param_float_data_t *joint_setup_y;
param_float_data_t *joint_setup_z;
param_float_data_t *joint_type;
param_float_data_t *machine_offset_x;
param_float_data_t *machine_offset_y;
param_float_data_t *machine_offset_z;
param_float_data_t *machine_rotation_x;
param_float_data_t *machine_rotation_y;
param_float_data_t *machine_rotation_z;
//! Machine cartesian speed, acc, jerk.
param_float_data_t *machine_maxvel_cart;
param_float_data_t *machine_maxacc_cart;
param_float_data_t *machine_jerk_cart;
//! Machine euler speed, acc, jerk.
param_float_data_t *machine_maxvel_euler;
param_float_data_t *machine_maxacc_euler;
param_float_data_t *machine_jerk_euler;

param_float_data_t *machine_curvel_cart_euler[6];
param_float_data_t *machine_curacc_cart_euler[6];

//! Ruckig motion in-parameters.
param_float_data_t *joint_jerk;
param_float_data_t *joint_maxacc;
param_float_data_t *joint_maxvel;
//! Ruckig motion out-parameters.
param_float_data_t *joint_curvel;
param_float_data_t *joint_curacc;

param_float_data_t *tarpos_cartx,*tarpos_carty,*tarpos_cartz;
param_float_data_t *curpos_cartx,*curpos_carty,*curpos_cartz;
param_float_data_t *tarpos_eulerx,*tarpos_eulery,*tarpos_eulerz;
param_float_data_t *curpos_eulerx,*curpos_eulery,*curpos_eulerz;

param_float_data_t *joint_curpos;

//! Parameter pin rw bool
typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;
param_bit_data_t *joint_limit;
param_bit_data_t *mode_fk;
param_bit_data_t *mode_ik_current;
param_bit_data_t *mode_ik_init;
param_bit_data_t *mode_error; //! using the name error in hal is not allowed.
param_bit_data_t *enable;

bool init_struct=0;

static void mainfunction();
static int setup_pins();
static void write_pins();
static void read_pins();
static void get_relative_tcp_offset();
static void update_relative_tcp_offset();
static void allocate_dynamic_c_struct(bool debug);
bool init_rt;
int rtapi_app_main(void) {

    comp_idx = hal_init("skynet");
    if(comp_idx < 0) return comp_idx;

    int r = 0;
    r = hal_export_funct("skynet", mainfunction, &skynets,0,0,comp_idx);
    r+=setup_pins();

    if(r) {
        hal_exit(comp_idx);

        //! Eventually free memory
        free(ptr);
        free(ptr->joint);

    } else {
        hal_ready(comp_idx);
    }

    return r;
}

void rtapi_app_exit(void) {
    hal_exit(comp_idx);

    //! Eventually free memory
    free(ptr);
    free(ptr->joint);
}

extern void command(struct MACHINE *aptr){

    ptr=aptr;
}

static void mainfunction(){

    if(!init_struct){
        allocate_dynamic_c_struct(debug);
        read_pins();
        mainwindow_ptr(ptr);
        start_gui();
        *ptr=forward_kinematic(ptr);
        get_relative_tcp_offset();
        init_struct=1;
    }

    //*ptr=gcode_runner(ptr);
    *ptr=forward_kinematic(ptr);
    *ptr=inverse_kinematic(ptr);
    *ptr=rtruckig(ptr);

    update_relative_tcp_offset();

    mainwindow_ptr(ptr);
    write_pins();
}

static void update_relative_tcp_offset(){
    ptr[0].data.machines=machines;

    for(int i=0; i<machines; i++){
        for(int j=0; j<6; j++){
            ptr[i].curpos_cart_euler_rel[j]=ptr[i].curpos_cart_euler[j]-ptr[i].curoffset_cart_euler_rel[j];
        }
    }
}

static void get_relative_tcp_offset(){
    ptr[0].data.machines=machines;

    for(int i=0; i<machines; i++){
        for(int j=0; j<6; j++){
            ptr[i].curoffset_cart_euler_rel[j]=ptr[i].curpos_cart_euler[j];
        }
    }
}

static void read_pins(){

    ptr[0].data.stepfilepath=stepfilepath;
    ptr[0].data.machines=machines;
    ptr[0].data.screenupdatems=screen_update_ms;
    ptr[0].data.joints=joints;

    int k=0;
    for(int i=0; i<machines; i++){

        //! Component argument.
        (ptr+i)->kinematic_iterations=iterations;

        (ptr+i)->kinmode=fk_rapid;

        //! Machine user defined offset, 3d rotation.
        (ptr+i)->machine_offset.x=machine_offset_x[i].Pin;
        (ptr+i)->machine_offset.y=machine_offset_y[i].Pin;
        (ptr+i)->machine_offset.z=machine_offset_z[i].Pin;
        (ptr+i)->machine_rotation.x=machine_rotation_x[i].Pin*toRadians;
        (ptr+i)->machine_rotation.y=machine_rotation_y[i].Pin*toRadians;
        (ptr+i)->machine_rotation.z=machine_rotation_z[i].Pin*toRadians;

        //! Machine cartesian vel, acc, jerk etc.
        (ptr+i)->maxvel_cart=machine_maxvel_cart[i].Pin;
        (ptr+i)->gui_maxvel_cart=machine_maxvel_cart[i].Pin;
        (ptr+i)->maxacc_cart=machine_maxacc_cart[i].Pin;
        (ptr+i)->jerk_cart=machine_jerk_cart[i].Pin;

        //! Machine euler vel, acc, jerk etc.
        (ptr+i)->maxvel_euler=machine_maxvel_euler[i].Pin;
        (ptr+i)->gui_maxvel_euler=machine_maxvel_euler[i].Pin;
        (ptr+i)->maxacc_euler=machine_maxacc_euler[i].Pin;
        (ptr+i)->jerk_euler=machine_jerk_euler[i].Pin;

        for(int j=0; j<joints[i]; j++){
            //! Enum joint type: rot_x = 0, rot_y = 1, rot_z = 2, trans_x = 3, trans_y = 4, trans_z = 5
            (ptr+i)->joint[j].rotationtype=joint_type[k].Pin;
            //! Axis endpoint xyz setup
            (ptr+i)->joint[j].axis_endpoint.x=joint_setup_x[k].Pin;
            (ptr+i)->joint[j].axis_endpoint.y=joint_setup_y[k].Pin;
            (ptr+i)->joint[j].axis_endpoint.z=joint_setup_z[k].Pin;

            //! Read ruckig motion parameters.
            (ptr+i)->joint[j].maxvel=joint_maxvel[k].Pin;
            (ptr+i)->joint[j].gui_maxvel=joint_maxvel[k].Pin;
            (ptr+i)->joint[j].maxacc=joint_maxacc[k].Pin;
            (ptr+i)->joint[j].jerk=joint_jerk[k].Pin;

            //! Important, above we first need to know the rotationtype !
            //! If joint::type is rotation, convert output from Radian to Degrees.
            if((ptr+i)->joint[j].rotationtype==0 || (ptr+i)->joint[j].rotationtype==1 || (ptr+i)->joint[j].rotationtype==2 || (ptr+i)->joint[j].rotationtype==6){
                (ptr+i)->joint[j].min=joint_min[k].Pin*toRadians;
                (ptr+i)->joint[j].max=joint_max[k].Pin*toRadians;
                (ptr+i)->joint[j].curpos=joint_curpos[k].Pin*toRadians;
                (ptr+i)->joint[j].init=joint_init[k].Pin*toRadians;
            }
            if((ptr+i)->joint[j].rotationtype==3 || (ptr+i)->joint[j].rotationtype==4 || (ptr+i)->joint[j].rotationtype==5 || (ptr+i)->joint[j].rotationtype==7){
                (ptr+i)->joint[j].min=joint_min[k].Pin;
                (ptr+i)->joint[j].max=joint_max[k].Pin;
                (ptr+i)->joint[j].curpos=joint_curpos[k].Pin;
                (ptr+i)->joint[j].init=joint_init[k].Pin;
            }
            k++;
        }
    }
}

static void write_pins(){

    int k=0;
    for(int i=0; i<machines; i++){
        for(int j=0; j<joints[i]; j++){
            *joint_out[k].Pin=ptr[i].joint[j].curpos;
            *joint_acc[k].Pin=ptr[i].joint[j].curacc;
            k++;
        }
    }
}

static void allocate_dynamic_c_struct(bool debug){
    if(debug){
        for(int i=0; i<machines; i++){
            rtapi_print_msg(RTAPI_MSG_ERR,"debug printed from kinematic halmodule.c\n");
            rtapi_print_msg(RTAPI_MSG_ERR,"machine :'%d'\n",i);
            rtapi_print_msg(RTAPI_MSG_ERR,"joints :'%d'\n",joints[i]);
        }
    }

    //! Allocate machine structure
    ptr = (struct MACHINE *)malloc((machines) * sizeof(struct MACHINE));
    if(ptr == NULL){
        rtapi_print_msg(RTAPI_MSG_ERR,"error loading dynamic memory");
    } else {
        rtapi_print_msg(RTAPI_MSG_ERR,"loading dynamic memory ok");
    }
    //! Allocate machine joints
    for(int i=0; i<machines; i++){
        (ptr+i)->joint = (struct JOINT *)malloc(joints[i] * sizeof(struct JOINT));
    }
}

static int setup_pins(){
    int r=0;

    mode_ik_init=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("skynet.mode-ik-init",HAL_RW,&(mode_ik_init->Pin),comp_idx);

    mode_ik_current=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("skynet.mode-ik-current",HAL_RW,&(mode_ik_current->Pin),comp_idx);

    mode_fk=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("skynet.mode-fk",HAL_RW,&(mode_fk->Pin),comp_idx);

    enable=(param_bit_data_t*)hal_malloc(sizeof(param_bit_data_t));
    r+=hal_param_bit_new("skynet.enable",HAL_RW,&(enable->Pin),comp_idx);

    //! Allocate dynamic memory
    int jointsize=0;
    for(int i=0; i<machines; i++){
        for(int j=0; j<joints[i]; j++){
            jointsize++;
        }
    }
    mode_error=(param_bit_data_t*)hal_malloc(machines* sizeof(param_bit_data_t));

    tarpos_cartx=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    tarpos_carty=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    tarpos_cartz=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    tarpos_eulerx=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    tarpos_eulery=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    tarpos_eulerz=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));

    curpos_cartx=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    curpos_carty=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    curpos_cartz=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    curpos_eulerx=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    curpos_eulery=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    curpos_eulerz=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));

    machine_offset_x=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_offset_y=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_offset_z=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_rotation_x=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_rotation_y=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_rotation_z=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));

    for(unsigned int i=0; i<6; i++){
        machine_curvel_cart_euler[i]=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
        machine_curacc_cart_euler[i]=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));

    }

    machine_maxvel_cart=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_maxacc_cart=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_jerk_cart=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));

    machine_maxvel_euler=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_maxacc_euler=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));
    machine_jerk_euler=(param_float_data_t*)hal_malloc(machines* sizeof(param_float_data_t));

    //! Kinematic & Opencascade parameters.
    joint_curpos=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_min=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_max=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_init=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_setup_x=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_setup_y=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_setup_z=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_type=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));

    //! Ruckig motion in-parameters.
    joint_maxvel=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_maxacc=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_jerk=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    //! Ruckig motion out-parameters, to update to user or halscope.
    joint_curvel=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    joint_curacc=(param_float_data_t*)hal_malloc(jointsize* sizeof(param_float_data_t));
    //! Ruckig is handling the joint limit.
    joint_limit=(param_bit_data_t*)hal_malloc(jointsize* sizeof(param_bit_data_t));

    joint_out=(float_data_t*)hal_malloc(jointsize* sizeof(float_data_t));
    joint_acc=(float_data_t*)hal_malloc(jointsize* sizeof(float_data_t));

    // Parameters:
    int k=0;
    for(int i=0; i<machines; i++){

        char str[100]={};
        strcat(str,"skynet.");
        char nr1[3];
        sprintf(nr1, "%d", i);
        strcat(str,nr1);
        strcat(str,".");

        char text[100]={};
        strcpy(text,str);
        strcat(text,"tarpos-cartx");
        r+=hal_param_float_new(text,HAL_RW,&(tarpos_cartx[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"curpos-cartx");
        r+=hal_param_float_new(text,HAL_RW,&(curpos_cartx[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"tarpos-carty");
        r+=hal_param_float_new(text,HAL_RW,&(tarpos_carty[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"curpos-carty");
        r+=hal_param_float_new(text,HAL_RW,&(curpos_carty[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"tarpos-cartz");
        r+=hal_param_float_new(text,HAL_RW,&(tarpos_cartz[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"curpos-cartz");
        r+=hal_param_float_new(text,HAL_RW,&(curpos_cartz[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"tarpos-eulerx");
        r+=hal_param_float_new(text,HAL_RW,&(tarpos_eulerx[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"curpos-eulerx");
        r+=hal_param_float_new(text,HAL_RW,&(curpos_eulerx[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"tarpos-eulery");
        r+=hal_param_float_new(text,HAL_RW,&(tarpos_eulery[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"curpos-eulery");
        r+=hal_param_float_new(text,HAL_RW,&(curpos_eulery[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"tarpos-eulerz");
        r+=hal_param_float_new(text,HAL_RW,&(tarpos_eulerz[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"curpos-eulerz");
        r+=hal_param_float_new(text,HAL_RW,&(curpos_eulerz[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"kinematic-error");
        r+=hal_param_bit_new(text,HAL_RW,&(mode_error[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-offset-x");
        r+=hal_param_float_new(text,HAL_RW,&(machine_offset_x[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-offset-y");
        r+=hal_param_float_new(text,HAL_RW,&(machine_offset_y[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-offset-z");
        r+=hal_param_float_new(text,HAL_RW,&(machine_offset_z[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-rotation-x");
        r+=hal_param_float_new(text,HAL_RW,&(machine_rotation_x[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-rotation-y");
        r+=hal_param_float_new(text,HAL_RW,&(machine_rotation_y[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-rotation-z");
        r+=hal_param_float_new(text,HAL_RW,&(machine_rotation_z[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-maxvel-cart");
        r+=hal_param_float_new(text,HAL_RW,&(machine_maxvel_cart[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curvel-cartx");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curvel_cart_euler[i][0].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curvel-carty");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curvel_cart_euler[i][1].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curvel-cartz");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curvel_cart_euler[i][2].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curvel-eulerx");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curvel_cart_euler[i][3].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curvel-eulery");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curvel_cart_euler[i][4].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curvel-eulerz");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curvel_cart_euler[i][5].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-maxacc-cart");
        r+=hal_param_float_new(text,HAL_RW,&(machine_maxacc_cart[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curacc-cartx");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curacc_cart_euler[i][0].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curacc-carty");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curacc_cart_euler[i][1].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curacc-cartz");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curacc_cart_euler[i][2].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curacc-eulerx");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curacc_cart_euler[i][3].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curacc-eulery");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curacc_cart_euler[i][4].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-curacc-eulerz");
        r+=hal_param_float_new(text,HAL_RW,&(machine_curacc_cart_euler[i][5].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-jerk-cart");
        r+=hal_param_float_new(text,HAL_RW,&(machine_jerk_cart[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-maxvel-euler");
        r+=hal_param_float_new(text,HAL_RW,&(machine_maxvel_euler[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-maxacc-euler");
        r+=hal_param_float_new(text,HAL_RW,&(machine_maxacc_euler[i].Pin),comp_idx);

        strcpy(text,"");
        strcpy(text,str);
        strcat(text,"machine-jerk-euler");
        r+=hal_param_float_new(text,HAL_RW,&(machine_jerk_euler[i].Pin),comp_idx);

        for(int j=0; j<joints[i]; j++){

            char str[100]={};
            strcat(str,"skynet.");
            char nr1[3], nr2[3];
            sprintf(nr1, "%d", i);
            sprintf(nr2, "%d", j);
            strcat(str,nr1);
            strcat(str,".");
            strcat(str,nr2);

            char text[100]={};
            strcpy(text,str);
            strcat(text,".joint-max");
            r+=hal_param_float_new(text,HAL_RW,&(joint_max[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-min");
            r+=hal_param_float_new(text,HAL_RW,&(joint_min[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-init");
            r+=hal_param_float_new(text,HAL_RW,&(joint_init[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-type");
            r+=hal_param_float_new(text,HAL_RW,&(joint_type[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-setup-x");
            r+=hal_param_float_new(text,HAL_RW,&(joint_setup_x[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-setup-y");
            r+=hal_param_float_new(text,HAL_RW,&(joint_setup_y[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-setup-z");
            r+=hal_param_float_new(text,HAL_RW,&(joint_setup_z[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-curpos");
            r+=hal_param_float_new(text,HAL_RW,&(joint_curpos[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-maxvel");
            r+=hal_param_float_new(text,HAL_RW,&(joint_maxvel[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-maxacc");
            r+=hal_param_float_new(text,HAL_RW,&(joint_maxacc[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-jerk");
            r+=hal_param_float_new(text,HAL_RW,&(joint_jerk[k].Pin),comp_idx);

            //! Ruckig motion out-parameters.
            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-curvel");
            r+=hal_param_float_new(text,HAL_RW,&(joint_curvel[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-curacc");
            r+=hal_param_float_new(text,HAL_RW,&(joint_curacc[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-limit");
            r+=hal_param_bit_new(text,HAL_RW,&(joint_limit[k].Pin),comp_idx);

            // Output pins to hal.
            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-out");
            r+=hal_pin_float_new(text,HAL_OUT,&(joint_out[k].Pin),comp_idx);

            strcpy(text,"");
            strcpy(text,str);
            strcat(text,".joint-acc");
            r+=hal_pin_float_new(text,HAL_OUT,&(joint_acc[k].Pin),comp_idx);

            k++;
        }
    }
    return r;
}






















