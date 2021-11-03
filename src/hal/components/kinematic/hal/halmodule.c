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

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *pin_j0;
float_data_t *pin_j1;
float_data_t *pin_j2;
float_data_t *pin_j3;
float_data_t *pin_j4;
float_data_t *pin_j5;
float_data_t *pin_cartx;
float_data_t *pin_carty;
float_data_t *pin_cartz;
float_data_t *pin_eulerx;
float_data_t *pin_eulery;
float_data_t *pin_eulerz;
float_data_t *pin_cartx_in;
float_data_t *pin_carty_in;
float_data_t *pin_cartz_in;
float_data_t *iterations;

typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t *perform_fk;
bit_data_t *perform_ik;
bit_data_t *ik_from_init;

typedef struct {
    hal_s32_t *Pin;
} s32_data_t;

typedef struct {
    hal_u32_t *Pin;
} u32_data_t;

typedef struct {
    hal_port_t *Pin;
} port_data_t;

typedef struct {
    hal_float_t Pin;
} param_float_data_t;
param_float_data_t *j0_x;
param_float_data_t *j0_y;
param_float_data_t *j0_z;
param_float_data_t *j0_init;
param_float_data_t *j0_min;
param_float_data_t *j0_max;
param_float_data_t *j1_x;
param_float_data_t *j1_y;
param_float_data_t *j1_z;
param_float_data_t *j1_init;
param_float_data_t *j1_min;
param_float_data_t *j1_max;
param_float_data_t *j2_x;
param_float_data_t *j2_y;
param_float_data_t *j2_z;
param_float_data_t *j2_init;
param_float_data_t *j2_min;
param_float_data_t *j2_max;
param_float_data_t *j3_x;
param_float_data_t *j3_y;
param_float_data_t *j3_z;
param_float_data_t *j3_init;
param_float_data_t *j3_min;
param_float_data_t *j3_max;
param_float_data_t *j4_x;
param_float_data_t *j4_y;
param_float_data_t *j4_z;
param_float_data_t *j4_init;
param_float_data_t *j4_min;
param_float_data_t *j4_max;
param_float_data_t *j5_x;
param_float_data_t *j5_y;
param_float_data_t *j5_z;
param_float_data_t *j5_init;
param_float_data_t *j5_min;
param_float_data_t *j5_max;
param_float_data_t *j0_rw;
param_float_data_t *j1_rw;
param_float_data_t *j2_rw;
param_float_data_t *j3_rw;
param_float_data_t *j4_rw;
param_float_data_t *j5_rw;
param_float_data_t *cart_x;
param_float_data_t *cart_y;
param_float_data_t *cart_z;
param_float_data_t *euler_x;
param_float_data_t *euler_y;
param_float_data_t *euler_z;

typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;

bool init=0, debug=0;
struct data d; // give meat to the c++ code.
struct data r; // recieve meat from the c++ code.

static void function();
static int setup_pins();

int rtapi_app_main(void) {

    comp_idx = hal_init("hal_kinematic");
    if(comp_idx < 0) return comp_idx;

    int r = 0;
    r = hal_export_funct("hal_kinematic", function, &skynet,0,0,comp_idx);
    r+=setup_pins();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }
    return r;
}

void rtapi_app_exit(void) {
    hal_exit(comp_idx);
}

static void function(){

    d.J0_x=j0_x->Pin;
    d.J0_y=j0_y->Pin;
    d.J0_z=j0_z->Pin;
    d.J0_init=j0_init->Pin*toRadians;
    d.J0=j0_rw->Pin*toRadians;
    d.J0_min=j0_min->Pin*toRadians;
    d.J0_max=j0_max->Pin*toRadians;

    d.J1_x=j1_x->Pin;
    d.J1_y=j1_y->Pin;
    d.J1_z=j1_z->Pin;
    d.J1_init=j1_init->Pin*toRadians;
    d.J1=j1_rw->Pin*toRadians;
    d.J1_min=j1_min->Pin*toRadians;
    d.J1_max=j1_max->Pin*toRadians;

    d.J2_x=j2_x->Pin;
    d.J2_y=j2_y->Pin;
    d.J2_z=j2_z->Pin;
    d.J2_init=j2_init->Pin*toRadians;
    d.J2=j2_rw->Pin*toRadians;
    d.J2_min=j2_min->Pin*toRadians;
    d.J2_max=j2_max->Pin*toRadians;

    d.J3_x=j3_x->Pin;
    d.J3_y=j3_y->Pin;
    d.J3_z=j3_z->Pin;
    d.J3_init=j3_init->Pin*toRadians;
    d.J3=j3_rw->Pin*toRadians;
    d.J3_min=j3_min->Pin*toRadians;
    d.J3_max=j3_max->Pin*toRadians;

    d.J4_x=j4_x->Pin;
    d.J4_y=j4_y->Pin;
    d.J4_z=j4_z->Pin;
    d.J4_init=j4_init->Pin*toRadians;
    d.J4=j4_rw->Pin*toRadians;
    d.J4_min=j4_min->Pin*toRadians;
    d.J4_max=j4_max->Pin*toRadians;

    d.J5_x=j5_x->Pin;
    d.J5_y=j5_y->Pin;
    d.J5_z=j5_z->Pin;
    d.J5_init=j5_init->Pin*toRadians;
    d.J5=j5_rw->Pin*toRadians;
    d.J5_min=j5_min->Pin*toRadians;
    d.J5_max=j5_max->Pin*toRadians;

    // Test axis_mm.
    cart_x->Pin=*pin_cartx_in->Pin;
    cart_y->Pin=*pin_carty_in->Pin;
    cart_z->Pin=*pin_cartz_in->Pin;

    d.Cartx=cart_x->Pin;
    d.Carty=cart_y->Pin;
    d.Cartz=cart_z->Pin;

    d.Eulerx=euler_x->Pin*toRadians;
    d.Eulery=euler_y->Pin*toRadians;
    d.Eulerz=euler_z->Pin*toRadians;
    
    d.Fk_mode=*perform_fk->Pin; // Forward kinematics selected.
    d.Ik_mode=*perform_ik->Pin;
    d.Ik_from_init=*ik_from_init->Pin;
    d.Iterations=*iterations->Pin;

    //struct data r=init_wrapper(d);

    if(!init){
        d.Fk_mode=1; // Always start up in fk mode. Set the cart values for one time.

        init=1;
    }

    if(d.Fk_mode){
        r=fk_wrapper(d);

        // Update cart and euler rw params. Joint params where the input and can stay the same.
        cart_x->Pin=r.Cartx;
        cart_y->Pin=r.Carty;
        cart_z->Pin=r.Cartz;
        euler_x->Pin=r.Eulerx*toDegrees;
        euler_y->Pin=r.Eulery*toDegrees;
        euler_z->Pin=r.Eulerz*toDegrees;

    }

    if(d.Ik_mode){
        r.error=0;
        r=ik_wrapper(d);

        // Update joint values
        j0_rw->Pin=r.J0*toDegrees;
        j1_rw->Pin=r.J1*toDegrees;
        j2_rw->Pin=r.J2*toDegrees;
        j3_rw->Pin=r.J3*toDegrees;
        j4_rw->Pin=r.J4*toDegrees;
        j5_rw->Pin=r.J5*toDegrees;

        if(r.error){
            rtapi_print_msg(RTAPI_MSG_ERR,"Kinematic error");
        }

    }

    // Pins to opencascade, net linked in opencascade.hal.
    if(!r.error && (d.Fk_mode || d.Ik_mode)){
        *pin_j0->Pin=r.J0*toDegrees;
        *pin_j1->Pin=r.J1*toDegrees;
        *pin_j2->Pin=r.J2*toDegrees;
        *pin_j3->Pin=r.J3*toDegrees;
        *pin_j4->Pin=r.J4*toDegrees;
        *pin_j5->Pin=r.J5*toDegrees;

        *pin_cartx->Pin=r.Cartx;
        *pin_carty->Pin=r.Carty;
        *pin_cartz->Pin=r.Cartz;

        *pin_eulerx->Pin=r.Eulerx*toDegrees;
        *pin_eulery->Pin=r.Eulery*toDegrees;
        *pin_eulerz->Pin=r.Eulerz*toDegrees;
    }

    if(debug){
        rtapi_print_msg(RTAPI_MSG_ERR,"MACHINE SETUP VALUES AFTER FIRST FORWARD KINEMATICS (fk): \n");
        rtapi_print_msg(RTAPI_MSG_ERR,"J0: %f \n", r.J0*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J1: %f \n", r.J1*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J2: %f \n", r.J2*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J3: %f \n", r.J3*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J4: %f \n", r.J4*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"J5: %f \n", r.J5*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"cart x: %f \n", r.Cartx);
        rtapi_print_msg(RTAPI_MSG_ERR,"cart y: %f \n", r.Carty);
        rtapi_print_msg(RTAPI_MSG_ERR,"cart z: %f \n", r.Cartz);
        rtapi_print_msg(RTAPI_MSG_ERR,"euler x: %f \n", r.Eulerx*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"euler y: %f \n", r.Eulery*toDegrees);
        rtapi_print_msg(RTAPI_MSG_ERR,"euler z: %f \n", r.Eulerz*toDegrees);
    }
}

static int setup_pins(){
    int r=0;

    // Pins:
    pin_j0=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-j0",HAL_OUT,&(pin_j0->Pin),comp_idx);

    pin_j1=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-j1",HAL_OUT,&(pin_j1->Pin),comp_idx);

    pin_j2=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-j2",HAL_OUT,&(pin_j2->Pin),comp_idx);

    pin_j3=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-j3",HAL_OUT,&(pin_j3->Pin),comp_idx);

    pin_j4=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-j4",HAL_OUT,&(pin_j4->Pin),comp_idx);

    pin_j5=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-j5",HAL_OUT,&(pin_j5->Pin),comp_idx);

    pin_cartx=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-cartx",HAL_OUT,&(pin_cartx->Pin),comp_idx);

    pin_carty=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-carty",HAL_OUT,&(pin_carty->Pin),comp_idx);

    pin_cartz=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-cartz",HAL_OUT,&(pin_cartz->Pin),comp_idx);

    pin_eulerx=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-eulerx",HAL_OUT,&(pin_eulerx->Pin),comp_idx);

    pin_eulery=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-eulery",HAL_OUT,&(pin_eulery->Pin),comp_idx);

    pin_eulerz=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-eulerz",HAL_OUT,&(pin_eulerz->Pin),comp_idx);

    pin_cartx_in=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-cartx-in",HAL_IN,&(pin_cartx_in->Pin),comp_idx);

    pin_carty_in=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-carty-in",HAL_IN,&(pin_carty_in->Pin),comp_idx);

    pin_cartz_in=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.pin-cartz-in",HAL_IN,&(pin_cartz_in->Pin),comp_idx);

    iterations=(float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("hal_kinematic.iterations",HAL_IN,&(iterations->Pin),comp_idx);

    perform_fk=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_kinematic.perform-fk",HAL_IN,&(perform_fk->Pin),comp_idx);

    perform_ik=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_kinematic.perform-ik",HAL_IN,&(perform_ik->Pin),comp_idx);

    ik_from_init=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("hal_kinematic.ik-from-init",HAL_IN,&(ik_from_init->Pin),comp_idx);

    // Parameters
    j0_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j0-x",HAL_RW,&(j0_x->Pin),comp_idx);

    j0_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j0-y",HAL_RW,&(j0_y->Pin),comp_idx);

    j0_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j0-z",HAL_RW,&(j0_z->Pin),comp_idx);

    j0_init=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j0-init",HAL_RW,&(j0_init->Pin),comp_idx);

    j0_min=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j0-min",HAL_RW,&(j0_min->Pin),comp_idx);

    j0_max=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j0-max",HAL_RW,&(j0_max->Pin),comp_idx);


    j1_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j1-x",HAL_RW,&(j1_x->Pin),comp_idx);

    j1_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j1-y",HAL_RW,&(j1_y->Pin),comp_idx);

    j1_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j1-z",HAL_RW,&(j1_z->Pin),comp_idx);

    j1_init=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j1-init",HAL_RW,&(j1_init->Pin),comp_idx);

    j1_min=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j1-min",HAL_RW,&(j1_min->Pin),comp_idx);

    j1_max=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j1-max",HAL_RW,&(j1_max->Pin),comp_idx);


    j2_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j2-x",HAL_RW,&(j2_x->Pin),comp_idx);

    j2_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j2-y",HAL_RW,&(j2_y->Pin),comp_idx);

    j2_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j2-z",HAL_RW,&(j2_z->Pin),comp_idx);

    j2_init=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j2-init",HAL_RW,&(j2_init->Pin),comp_idx);

    j2_min=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j2-min",HAL_RW,&(j2_min->Pin),comp_idx);

    j2_max=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j2-max",HAL_RW,&(j2_max->Pin),comp_idx);


    j3_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j3-x",HAL_RW,&(j3_x->Pin),comp_idx);

    j3_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j3-y",HAL_RW,&(j3_y->Pin),comp_idx);

    j3_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j3-z",HAL_RW,&(j3_z->Pin),comp_idx);

    j3_init=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j3-init",HAL_RW,&(j3_init->Pin),comp_idx);

    j3_min=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j3-min",HAL_RW,&(j3_min->Pin),comp_idx);

    j3_max=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j3-max",HAL_RW,&(j3_max->Pin),comp_idx);


    j4_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j4-x",HAL_RW,&(j4_x->Pin),comp_idx);

    j4_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j4-y",HAL_RW,&(j4_y->Pin),comp_idx);

    j4_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j4-z",HAL_RW,&(j4_z->Pin),comp_idx);

    j4_init=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j4-init",HAL_RW,&(j4_init->Pin),comp_idx);

    j4_min=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j4-min",HAL_RW,&(j4_min->Pin),comp_idx);

    j4_max=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j4-max",HAL_RW,&(j4_max->Pin),comp_idx);


    j5_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j5-x",HAL_RW,&(j5_x->Pin),comp_idx);

    j5_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j5-y",HAL_RW,&(j5_y->Pin),comp_idx);

    j5_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j5-z",HAL_RW,&(j5_z->Pin),comp_idx);

    j5_init=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j5-init",HAL_RW,&(j5_init->Pin),comp_idx);

    j5_min=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j5-min",HAL_RW,&(j5_min->Pin),comp_idx);

    j5_max=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j5-max",HAL_RW,&(j5_max->Pin),comp_idx);


    j0_rw=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j0-rw",HAL_RW,&(j0_rw->Pin),comp_idx);

    j1_rw=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j1-rw",HAL_RW,&(j1_rw->Pin),comp_idx);

    j2_rw=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j2-rw",HAL_RW,&(j2_rw->Pin),comp_idx);

    j3_rw=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j3-rw",HAL_RW,&(j3_rw->Pin),comp_idx);

    j4_rw=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j4-rw",HAL_RW,&(j4_rw->Pin),comp_idx);

    j5_rw=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.j5-rw",HAL_RW,&(j5_rw->Pin),comp_idx);

    cart_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.cart-x",HAL_RW,&(cart_x->Pin),comp_idx);

    cart_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.cart-y",HAL_RW,&(cart_y->Pin),comp_idx);

    cart_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.cart-z",HAL_RW,&(cart_z->Pin),comp_idx);

    euler_x=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.euler-x",HAL_RW,&(euler_x->Pin),comp_idx);

    euler_y=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.euler-y",HAL_RW,&(euler_y->Pin),comp_idx);

    euler_z=(param_float_data_t*)hal_malloc(sizeof(param_float_data_t));
    r+=hal_param_float_new("hal_kinematic.euler-z",HAL_RW,&(euler_z->Pin),comp_idx);

    return r;
}






















