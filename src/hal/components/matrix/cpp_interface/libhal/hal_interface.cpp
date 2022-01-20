#include "hal_interface.h"

int comp_idxx;

typedef struct {
    bool ok;
} skynet_t;
skynet_t *omicronx;

hal_interface::hal_interface()
{

}

hal_interface::~hal_interface()
{

}

double hal_interface::print_servo_thread_time(){

    /*
    if (halpr_find_comp_by_name("matrix") != 0) {
        std::cout << "matrix found!" << std::endl;
    }
    if (halpr_find_param_by_name("matrix.tmax") != 0) {
        std::cout << "matrix.time found!" << std::endl;
    }*/

    //! Get parameter value example
    /*
    hal_param_t *param;
    hal_type_t type;
    void *d_ptr;

    param = halpr_find_param_by_name("matrix.tmax");
    if (param) {
        type = param->type;
        d_ptr = SHMPTR(param->data_ptr);
        static char buf[15];
        snprintf(buf, 14, "%ld", (long)*((hal_s32_t *) d_ptr));

        std::cout<<"value:"<<buf<<std::endl;
    }*/

    //! Get pin value example
    /*
    if (halpr_find_pin_by_name("matrix.time") != 0) {
        std::cout << "matrix.time found!" << std::endl;
    } */

    hal_data_u *value;
    hal_type_t type;
    bool *con = nullptr;
    int r=hal_get_pin_value_by_name("matrix.mainfunction.time",&type,&value,con);
    if(r!=0){
        std::cout<<"error from /libhal/hal_interface function"<<std::endl;
    }
    // std::cout << "matrix.time function result:"<<ok<< std::endl;
    // std::cout << "matrix.time function result:"<<value->s<< std::endl;

    return value->s;
}

double hal_interface::print_base_thread_time(){

    /*
    if (halpr_find_comp_by_name("matrix") != 0) {
        std::cout << "matrix found!" << std::endl;
    }
    if (halpr_find_param_by_name("matrix.tmax") != 0) {
        std::cout << "matrix.time found!" << std::endl;
    }*/

    //! Get parameter value example
    /*
    hal_param_t *param;
    hal_type_t type;
    void *d_ptr;

    param = halpr_find_param_by_name("matrix.tmax");
    if (param) {
        type = param->type;
        d_ptr = SHMPTR(param->data_ptr);
        static char buf[15];
        snprintf(buf, 14, "%ld", (long)*((hal_s32_t *) d_ptr));

        std::cout<<"value:"<<buf<<std::endl;
    }*/

    //! Get pin value example
    /*
    if (halpr_find_pin_by_name("matrix.time") != 0) {
        std::cout << "matrix.time found!" << std::endl;
    } */

    hal_data_u *value;
    hal_type_t type;
    bool *con = nullptr;
    int r=hal_get_pin_value_by_name("matrix.follow.time",&type,&value,con);
    if(r!=0){
        std::cout<<"error from /libhal/hal_interface function"<<std::endl;
    }
    // std::cout << "matrix.time function result:"<<ok<< std::endl;
    // std::cout << "matrix.time function result:"<<value->s<< std::endl;

    return value->s;
}

void hal_interface::get_info(){

    comp_idxx = hal_init("gui");

    int error = hal_ready(comp_idxx);
    if(error==0){
        std::cout << "gui component ok" << std::endl;
    } else {
        std::cout << "gui component error" << std::endl;
    }
}
