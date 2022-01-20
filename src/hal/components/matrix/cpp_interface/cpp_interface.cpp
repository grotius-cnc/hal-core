#include "cpp_interface.h"

Cpp_interface::Cpp_interface()
{
}

void Cpp_interface::gui_thread(){
    int argc=0;
    char *argv[]={};
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    a.exec();
}

void Cpp_interface::start_gui(){
    Thread = new std::thread(&Cpp_interface::gui_thread,this);
    Thread->detach(); //! Execute the thread independent from other stuff.
}

extern "C" void start_gui(struct DATA *p){
    ptr=p;
    Cpp_interface().start_gui();
}









