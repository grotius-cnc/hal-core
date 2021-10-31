#include "rtapi.h"
#include "rtapi_ctype.h"
#include "rtapi_app.h"
#include "rtapi_string.h"
#include "rtapi_errno.h"
#include "rtapi_math64.h"
#include <rtapi_io.h>
#include "hal.h"
#include <halmodule.h>

/* module information */
MODULE_AUTHOR("Skynet");
MODULE_DESCRIPTION("Halmodule DoFs");
MODULE_LICENSE("GPL");

static unsigned int nr;
RTAPI_MP_INT(nr, "");

static int comp_idx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *array_maxvel[16];
float_data_t *array_maxacc[16];
float_data_t *array_maxjerk[16];
float_data_t *array_interfacetype[16];
float_data_t *array_current_pos[16];
float_data_t *array_current_vel[16];
float_data_t *array_current_acc[16];
float_data_t *array_target_pos[16];

// Stepgen
float_data_t *cycle_delay[16], *pos_cmd[16], *pos_fb[16], *pulses_mm[16];

typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t *array_enable[16];
// Stepgen
bit_data_t *array_step[16], *array_dir[16], *array_error[16];

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

typedef struct {
    hal_bit_t Pin;
} param_bit_data_t;

static int comp_idx;

static void function();
static void pulse();
static void control();
static int setup_pins();
char base[20]="dofs.";

// Stepgen
int counter[16], pulsecounter[16], pulsecontrolcounter[16];
bool on[16]; // StepHigh, StepLow
float deltapos[16]; // DistanceToGo every 1ms.

int rtapi_app_main(void) {

    comp_idx = hal_init("dofs");
    if(comp_idx < 0) return comp_idx;

    int r = 0;
    r = hal_export_funct("dofs.run", function, &skynet,0,0,comp_idx);
    r = hal_export_funct("dofs.pulse", pulse, &skynet,0,0,comp_idx);
    r+=setup_pins();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }

    return 0;
}

void rtapi_app_exit(void){
    hal_exit(comp_idx);
}

//! Perforn's every servo-period 1ms.
static void function(){

    // For every DoFs perform :
    for(unsigned int i=0; i<nr; i++){

        // Init state
        struct result r;
        r.curpos=*array_current_pos[i]->Pin;
        r.curvel=*array_current_vel[i]->Pin;
        r.curacc=*array_current_acc[i]->Pin;
        r.maxvel=*array_maxvel[i]->Pin;
        r.maxacc=*array_maxacc[i]->Pin;
        r.maxjerk=*array_maxjerk[i]->Pin;
        r.tarpos=*array_target_pos[i]->Pin;

        r.enable=*array_enable[i]->Pin;
        r.interfacetype=0; // Enum index.
        r.synchronizationtype=3;
        r.durationdiscretizationtype=0;

        // Calculate
        r=wrapper_get_pos_array(r);

        // Update state
        *array_current_pos[i]->Pin=r.curpos;
        *array_current_vel[i]->Pin=r.curvel;
        *array_current_acc[i]->Pin=r.curacc;

        pulsecounter[i]=0;

        deltapos[i]=r.curpos-*pos_fb[i]->Pin;

        // Example, travel 5mm in 1ms.
        // Stepperdriver settings : 5000 pulse/rev
        // 5000 pulse/(travel 40mm a revolution) = 125 pulse/mm * 5 mm travel = 625 pulses.

        pulsecounter[i]=*pulses_mm[i]->Pin*deltapos[i];
    }
}

//! Perforn's every base-thread period.
static void pulse(){
    // 1 millisecond = 1000 microseconds = 1000000 nanoseconds us

    // For every DoFs perform :
    for(unsigned int i=0; i<nr; i++){

        if(pulsecounter[i]>0){
            *array_dir[i]->Pin=0;

            counter[i]++;
            if(on[i] && counter[i]>*cycle_delay[i]->Pin){
                *array_step[i]->Pin=1;
                on[i]=0;
                counter[i]=0;

                pulsecounter[i]--;
                pulsecontrolcounter[i]++;

            } else if(!on[i] && counter[i]>*cycle_delay[i]->Pin){
                *array_step[i]->Pin=0;
                on[i]=1;
                counter[i]=0;
            }
        }

        if(pulsecounter[i]<0){
            *array_dir[i]->Pin=1;

            counter[i]++;
            if(on[i] && counter[i]>*cycle_delay[i]->Pin){
                *array_step[i]->Pin=1;
                on[i]=0;
                counter[i]=0;

                pulsecounter[i]++;
                pulsecontrolcounter[i]--;

            } else if(!on[i] && counter[i]>*cycle_delay[i]->Pin){
                *array_step[i]->Pin=0;
                on[i]=1;
                counter[i]=0;
            }

        }

        *pos_fb[i]->Pin=pulsecontrolcounter[i]/ *pulses_mm[i]->Pin;
    }
}

static int setup_pins(){

    int r=0;

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.enable.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_enable[i]=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
        r+=hal_pin_bit_new(name,HAL_IN,&(array_enable[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.curpos.";  // contains the component name.
        char charvalue[3];          // contains the int value up to 999.
        sprintf(charvalue,"%d",i);  // express a int to char.
        strcat(name,charvalue);     // assign text a + text b.
        array_current_pos[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_OUT,&(array_current_pos[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.curvel.";  // contains the component name.
        char charvalue[3];          // contains the int value up to 999.
        sprintf(charvalue,"%d",i);  // express a int to char.
        strcat(name,charvalue);     // assign text a + text b.
        array_current_vel[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_OUT,&(array_current_vel[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.curacc.";  // contains the component name.
        char charvalue[3];          // contains the int value up to 999.
        sprintf(charvalue,"%d",i);  // express a int to char.
        strcat(name,charvalue);     // assign text a + text b.
        array_current_acc[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_OUT,&(array_current_acc[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.tarpos.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_target_pos[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(array_target_pos[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.maxvel.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_maxvel[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(array_maxvel[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.maxacc.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_maxacc[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(array_maxacc[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.maxjerk.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_maxjerk[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(array_maxjerk[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="dofs.stepgen.pulses_mm.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        pulses_mm[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(pulses_mm[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="dofs.stepgen.cycle_delay.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        cycle_delay[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(cycle_delay[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="dofs.stepgen.pos_fb.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        pos_fb[i]=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(pos_fb[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.stepgen.step.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_step[i]=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
        r+=hal_pin_bit_new(name,HAL_OUT,&(array_step[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.stepgen.dir.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_dir[i]=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
        r+=hal_pin_bit_new(name,HAL_OUT,&(array_dir[i]->Pin),comp_idx);
    }

    for(unsigned int i=0; i<nr; i++){
        char name[20]="dofs.stepgen.error.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        array_error[i]=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
        r+=hal_pin_bit_new(name,HAL_OUT,&(array_error[i]->Pin),comp_idx);
    }

    return r;
}







































