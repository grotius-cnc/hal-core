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
MODULE_DESCRIPTION("Halmodule SynchroDoFs");
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
float_data_t array_interfacetype[6];
float_data_t array_current_pos[6];
float_data_t array_current_vel[6];
float_data_t array_current_acc[6];
float_data_t array_target_pos[6];
float_data_t array_maxvel[6];
float_data_t array_maxacc[6];
float_data_t array_maxjerk[6];

typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t array_enable[6];

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
static int setup_pins();
char base[20]="synchrodofs.";

int rtapi_app_main(void) {

    comp_idx = hal_init("synchrodofs");
    if(comp_idx < 0) return comp_idx;

    int r = 0;
    r = hal_export_funct("synchrodofs.run", function, &skynet,0,0,comp_idx);
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

//! Perforn's every ms.
static void function(){

    // Init state
    struct result r[nr];

    // For every DoFs perform :
    for(unsigned int i=0; i<nr; i++){

        r[i].curpos=*array_current_pos[i].Pin;
        r[i].curvel=*array_current_vel[i].Pin;
        r[i].curacc=*array_current_acc[i].Pin;
        r[i].maxvel=*array_maxvel[i].Pin;
        r[i].maxacc=*array_maxacc[i].Pin;
        r[i].maxjerk=*array_maxjerk[i].Pin;
        r[i].tarpos=*array_target_pos[i].Pin;

        r[i].enable=*array_enable[i].Pin;
        r[i].size=nr; // Size of the DoFs array.

        // Calculate
        *r=wrapper_get_pos_array(r);

        // Update state
        *array_current_pos[i].Pin=r[i].curpos;
        *array_current_vel[i].Pin=r[i].curvel;
        *array_current_acc[i].Pin=r[i].curacc;
    }
}

static int setup_pins(){

    int r=0;

    for(unsigned int i=0; i<nr; i++){
        char name[20]="synchrodofs.enable.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        bit_data_t *dofs_bit;
        dofs_bit=(bit_data_t*)hal_malloc(sizeof(bit_data_t));
        r+=hal_pin_bit_new(name,HAL_IN,&(dofs_bit->Pin),comp_idx);

        array_enable[i].Pin=dofs_bit->Pin;
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="synchrodofs.curpos.";  // contains the component name.
        char charvalue[3];          // contains the int value up to 999.
        sprintf(charvalue,"%d",i);  // express a int to char.
        strcat(name,charvalue);     // assign text a + text b.
        float_data_t *dofs_float;
        dofs_float=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_OUT,&(dofs_float->Pin),comp_idx);

        array_current_pos[i].Pin=dofs_float->Pin;
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="synchrodofs.curvel.";  // contains the component name.
        char charvalue[3];          // contains the int value up to 999.
        sprintf(charvalue,"%d",i);  // express a int to char.
        strcat(name,charvalue);     // assign text a + text b.
        float_data_t *dofs_float;
        dofs_float=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_OUT,&(dofs_float->Pin),comp_idx);

        array_current_vel[i].Pin=dofs_float->Pin;
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="synchrodofs.curacc.";  // contains the component name.
        char charvalue[3];          // contains the int value up to 999.
        sprintf(charvalue,"%d",i);  // express a int to char.
        strcat(name,charvalue);     // assign text a + text b.
        float_data_t *dofs_float;
        dofs_float=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_OUT,&(dofs_float->Pin),comp_idx);

        array_current_acc[i].Pin=dofs_float->Pin;
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="synchrodofs.tarpos.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        float_data_t *dofs_float;
        dofs_float=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(dofs_float->Pin),comp_idx);

        array_target_pos[i].Pin=dofs_float->Pin;
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="synchrodofs.maxvel.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        float_data_t *dofs_float;
        dofs_float=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(dofs_float->Pin),comp_idx);

        array_maxvel[i].Pin=dofs_float->Pin;
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="synchrodofs.maxacc.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        float_data_t *dofs_float;
        dofs_float=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(dofs_float->Pin),comp_idx);

        array_maxacc[i].Pin=dofs_float->Pin;
    }

    for(unsigned int i=0; i<nr; i++){
        char name[30]="synchrodofs.maxjerk.";
        char charvalue[3];
        sprintf(charvalue,"%d",i);
        strcat(name,charvalue);
        float_data_t *dofs_float;
        dofs_float=(float_data_t*)hal_malloc(sizeof(float_data_t));
        r+=hal_pin_float_new(name,HAL_IN,&(dofs_float->Pin),comp_idx);

        array_maxjerk[i].Pin=dofs_float->Pin;
    }
    return r;
}







































