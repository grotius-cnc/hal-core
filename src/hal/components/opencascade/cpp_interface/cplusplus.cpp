#include "cplusplus.h"
#include "iostream"
#include "mainwindow.h"
#include <QApplication>
#include "thread"

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

// Example of extern variable:
data databucket;
char *stepfilepath_evar;
int display_hz_evar;

Cplusplus::Cplusplus()
{

}

// Start opencascade gui:
extern "C" void init_opencascade(char *stepfilepath, int display_hz){ // c to cpp wrapper function
    Cplusplus().start();
    stepfilepath_evar=stepfilepath;
    display_hz_evar=display_hz;
}


void Cplusplus::start(){
    Thread = new std::thread(&Cplusplus::gui,this);
    Thread->detach(); // Execute the thread independent from other stuff.
}

void Cplusplus::gui(){
        //std::cout<<"gui start executed"<<std::endl;
        int argc=0;
        char *argv[]={};
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        a.exec();
}

// Read kinematic data:
extern "C" void meat(data d){ // C to cpp wrapper function
    // update the global databucket of the gui program.
    Cplusplus().meat_cplusplus(d);
}

void Cplusplus::meat_cplusplus(data d){ // C++ implementation, including header
    // Example of extern variable:
     databucket=d;
}















