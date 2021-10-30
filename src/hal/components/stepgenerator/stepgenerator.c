#include "rtapi.h"		/* RTAPI realtime OS API */
#include "rtapi_app.h"  /* RTAPI realtime module decls */
#include "hal.h"		/* HAL public API decls */

#include <float.h>
#include "rtapi_math.h"

MODULE_AUTHOR("Skynet");
MODULE_DESCRIPTION("Example 1 axis step generator for Hal-core");
MODULE_LICENSE("GPL");

static unsigned int nr;
RTAPI_MP_INT(nr, "");

typedef struct {
    bool ok;
} skynet_t;
skynet_t *skynet;

typedef struct {
    hal_float_t *Pin;
} float_data_t;
float_data_t *cycle_delay, *pos_cmd, *pos_fb, *pulses_mm;

typedef struct {
    hal_bit_t *Pin;
} bit_data_t;
bit_data_t *step_01, *dir_01;

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

static void pulse();
static void update();
static int setup_pins();
int counter=0, pulsecounter, pulsecontrolcounter;
bool on=0;
float oldpos, newpos, deltapos;

int rtapi_app_main(void)
{
    comp_idx = hal_init("stepgenerator");
    if(comp_idx < 0) return comp_idx;

    int r = 0;

    ///* Pulse function attached to base-thread
    r = hal_export_funct("stepgenerator.pulse", pulse, &skynet,0,0,comp_idx);
    r = hal_export_funct("stepgenerator.update", update, &skynet,0,0,comp_idx);
    r+=setup_pins();

    if(r) {
        hal_exit(comp_idx);
    } else {
        hal_ready(comp_idx);
    }
    return 0;
}

void rtapi_app_exit(void)
{
    hal_exit(comp_idx);
}

static void pulse()
{
    // 1 millisecond = 1000 microseconds = 1000000 nanoseconds us

    if(pulsecounter>=0){
         *dir_01->Pin=0;
    }
    if(pulsecounter<0){
         *dir_01->Pin=1;
    }

    if(pulsecounter>0){
        counter++;
        if(on && counter>*cycle_delay->Pin){
            *step_01->Pin=1;
            on=0;
            counter=0;
        } else if(!on && counter>*cycle_delay->Pin){
            *step_01->Pin=0;
            on=1;
            counter=0;
        }
        pulsecounter--;
        pulsecontrolcounter++;
    }

    if(pulsecounter<0){
        counter++;
        if(on && counter>*cycle_delay->Pin){
            *step_01->Pin=1;
            on=0;
            counter=0;
        } else if(!on && counter>*cycle_delay->Pin){
            *step_01->Pin=0;
            on=1;
            counter=0;
        }
        pulsecounter++;
        pulsecontrolcounter--;
    }

    *pos_fb->Pin=pulsecontrolcounter/ *pulses_mm->Pin;
}

static void update()
{
    newpos=*pos_cmd->Pin;
    // every 1ms we look for new position.
    if(newpos!=oldpos){
        deltapos=newpos-oldpos;

        // Example, travel 5mm in 1ms.
        // Stepperdriver settings : 5000 pulse/rev
        // 5000 pulse/(travel 40mm a revolution) = 125 pulse/mm * 5 mm travel = 625 pulses.

        pulsecounter+=*pulses_mm->Pin*deltapos;
        oldpos=newpos;
    }
}

static int setup_pins(){
    int r=0;

    step_01 = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("stepgenerator.step",HAL_OUT,&(step_01->Pin),comp_idx);

    dir_01 = (bit_data_t*)hal_malloc(sizeof(bit_data_t));
    r+=hal_pin_bit_new("stepgenerator.dir",HAL_OUT,&(dir_01->Pin),comp_idx);

    cycle_delay = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("stepgenerator.cycle_delay",HAL_IN,&(cycle_delay->Pin),comp_idx);

    pos_cmd = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("stepgenerator.pos_cmd",HAL_IN,&(pos_cmd->Pin),comp_idx);

    pos_fb = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("stepgenerator.pos_fb",HAL_OUT,&(pos_fb->Pin),comp_idx);

    pulses_mm = (float_data_t*)hal_malloc(sizeof(float_data_t));
    r+=hal_pin_float_new("stepgenerator.pulses_mm",HAL_IN,&(pulses_mm->Pin),comp_idx);

    return r;
}
















