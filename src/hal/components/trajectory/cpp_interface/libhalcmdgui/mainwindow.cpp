#include "mainwindow.h"
#include "ui_mainwindow.h"

int comp_id=0;
typedef struct {
    hal_port_t *Pin;
} port_data_t;
port_data_t *port;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton_mode_auto, SIGNAL(pressed()),SLOT(mode_auto()));
    QObject::connect(ui->pushButton_mode_jog, SIGNAL(pressed()),SLOT(mode_jog()));

    QObject::connect(ui->pushButton_mode_auto_start, SIGNAL(pressed()),SLOT(mode_auto_run_pressed()));
    QObject::connect(ui->pushButton_mode_auto_stop, SIGNAL(pressed()),SLOT(mode_auto_stop_pressed()));

    QObject::connect(ui->pushButton_jog_x_plus, SIGNAL(pressed()),SLOT(jog_x_plus_pressed()));
    QObject::connect(ui->pushButton_jog_x_min, SIGNAL(pressed()),SLOT(jog_x_min_pressed()));
    QObject::connect(ui->pushButton_jog_y_plus, SIGNAL(pressed()),SLOT(jog_y_plus_pressed()));
    QObject::connect(ui->pushButton_jog_y_min, SIGNAL(pressed()),SLOT(jog_y_min_pressed()));
    QObject::connect(ui->pushButton_jog_z_plus, SIGNAL(pressed()),SLOT(jog_z_plus_pressed()));
    QObject::connect(ui->pushButton_jog_z_min, SIGNAL(pressed()),SLOT(jog_z_min_pressed()));

    QObject::connect(ui->pushButton_jog_x_plus, SIGNAL(released()),SLOT(jog_x_plus_released()));
    QObject::connect(ui->pushButton_jog_x_min, SIGNAL(released()),SLOT(jog_x_min_released()));
    QObject::connect(ui->pushButton_jog_y_plus, SIGNAL(released()),SLOT(jog_y_plus_released()));
    QObject::connect(ui->pushButton_jog_y_min, SIGNAL(released()),SLOT(jog_y_min_released()));
    QObject::connect(ui->pushButton_jog_z_plus, SIGNAL(released()),SLOT(jog_z_plus_released()));
    QObject::connect(ui->pushButton_jog_z_min, SIGNAL(released()),SLOT(jog_z_min_released()));

    QObject::connect(ui->pushButton_load_gcode_file, SIGNAL(pressed()),SLOT(loadfile()));
    QObject::connect(ui->pushButton_run_from_line, SIGNAL(pressed()),SLOT(gcodeblock()));

    QObject::connect(ui->pushButton_tooloffset, SIGNAL(pressed()),SLOT(tooloffset()));
    QObject::connect(ui->pushButton_max_values, SIGNAL(pressed()),SLOT(max_values()));

    QObject::connect(ui->pushButton_m3_on, SIGNAL(pressed()),SLOT(m3_on()));
    QObject::connect(ui->pushButton_m3_off, SIGNAL(pressed()),SLOT(m3_off()));
    QObject::connect(ui->pushButton_m7_on, SIGNAL(pressed()),SLOT(m7_on()));
    QObject::connect(ui->pushButton_m7_off, SIGNAL(pressed()),SLOT(m7_off()));
    QObject::connect(ui->pushButton_m8_on, SIGNAL(pressed()),SLOT(m8_on()));
    QObject::connect(ui->pushButton_m8_off, SIGNAL(pressed()),SLOT(m8_off()));

    QObject::connect(ui->pushButton_power_rpm, SIGNAL(pressed()),SLOT(power_rpm()));
    QObject::connect(ui->pushButton_load_mdi, SIGNAL(pressed()),SLOT(load_mdi()));

    // Setup a hal_port sender.
    comp_id = hal_init("gui");
    port = (port_data_t*)hal_malloc(sizeof(char[150]));
    hal_pin_port_new("gui.port",HAL_OUT,&(port->Pin),comp_id);
    // hal_signal_new("signal_filename",HAL_PORT);
    // hal_link("gui.port","signal_filename");
    hal_ready(comp_id);
}

MainWindow::~MainWindow()
{    
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/scripts/./halrun -U";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    std::cout<<"exit"<<std::endl;

    delete ui;
}

#include <fstream>
#include <libskynet/directory.h>
void MainWindow::load_mdi(){

    // No auto mode to prevent a vector crash.
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    std::string mdi_filename="mdi.ngc";

    // Clear mdi textfile.
    std::ofstream myfile;
    myfile.open(mdi_filename, std::ios::out); //ios::app = append true, ios::out = new
    myfile<<"";
    myfile.close();

    // Fill mdi textfile.
    myfile.open(mdi_filename, std::ios::app); //ios::app = append true, ios::out = new
    myfile.precision(3);
    myfile<<std::fixed;
    myfile<<ui->lineEdit_mdi->text().toStdString();
    myfile.close();

    // Get current directory.
    char* cwd = getcwd( 0, 0 ) ; // **** microsoft specific ****
    std::string working_directory(cwd) ;
    std::free(cwd) ;
    result=working_directory+"/"+mdi_filename;

    // Let the trajectory component know where the mdi file is.
    hal_port_write(*port->Pin,result.c_str(),140);

    // Load the mdi file into the trajectory component.
    command="/bin/./halcmd setp hal_trajectory.load_mdi 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::power_rpm(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;
    double value;

    command="/bin/./halcmd setp hal_trajectory.power_rpm_in ";
    value=ui->lineEdit_power_rpm->text().toDouble();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());
}

void MainWindow::m3_on(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.m3_in 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::m3_off(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.m3_in 0";
    result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::m7_on(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.m7_in 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::m7_off(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.m7_in 0";
    result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::m8_on(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.m8_in 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::m8_off(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.m8_in 0";
    result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::max_values(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;
    double value;

    command="/bin/./halcmd setp hal_trajectory.max_velocity ";
    value=ui->lineEdit_max_velocity->text().toDouble();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.max_acceleration ";
    value=ui->lineEdit_max_acceleration->text().toDouble();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.max_jerk ";
    value=ui->lineEdit_max_jerk->text().toDouble();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());
}

void MainWindow::tooloffset(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;
    double value;

    command="/bin/./halcmd setp hal_trajectory.tcp_offset_x ";
    value=ui->lineEdit_tooloffset_x->text().toDouble();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.tcp_offset_y ";
    value=ui->lineEdit_tooloffset_y->text().toDouble();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.tcp_offset_z ";
    value=ui->lineEdit_tooloffset_z->text().toDouble();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());
}

void MainWindow::gcodeblock(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command,result;
    int value;

    command="/bin/./halcmd setp hal_trajectory.start_from_gcodeblock ";
    value=ui->lineEdit_run_from_line->text().toInt();
    result=path_to_lcnc_bin+command+std::to_string(value);
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.run_from_line 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.start 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    ui->pushButton_mode_auto_start->setStyleSheet(green);
    ui->pushButton_mode_auto_stop->setStyleSheet(darkgrey);
}

void MainWindow::loadfile(){

    // No auto mode to prevent a vector crash.
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    //char sent[140] = { 'a' , 'l', 'i', 'e' , 'n', 0};
    //hal_port_write(*port->Pin,sent,140);
    //std::cout<<"send alien to port"<<std::endl;

    if (!pfd::settings::available()){
         std::cout << "Portable File Dialogs are not available on this platform. \n"
                      "On linux install zenity, $ sudo apt-get install zenity\n";
     }
     auto f = pfd::open_file("Choose files to read", get_current_dir_name(),
                             { "Dxf Files (.ngc)", "*.ngc",
                               "All Files", "*" }, pfd::opt::none); // Or ::multiselect.
     // open_dxf_file(f.result().at(0)); // This lib can open multiple results.

     hal_port_write(*port->Pin,f.result().at(0).c_str(),140);

     // Set the load_file pin.
     command="/bin/./halcmd setp hal_trajectory.load_file 1";
     result=path_to_lcnc_bin+command;
     system(result.c_str());
}

void MainWindow::mode_auto_run_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.stop 0";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.start 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    ui->pushButton_mode_auto_start->setStyleSheet(green);
    ui->pushButton_mode_auto_stop->setStyleSheet(darkgrey);
}

void MainWindow::mode_auto_stop_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.stop 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.start 0";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    ui->pushButton_mode_auto_start->setStyleSheet(darkgrey);
    ui->pushButton_mode_auto_stop->setStyleSheet(red);
}

void MainWindow::mode_auto(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.mode_auto 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.mode_jog 0";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    ui->pushButton_mode_auto->setStyleSheet(green);
    ui->pushButton_mode_jog->setStyleSheet(darkgrey);
}

void MainWindow::mode_jog(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command, result;

    command="/bin/./halcmd setp hal_trajectory.mode_auto 0";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    command="/bin/./halcmd setp hal_trajectory.mode_jog 1";
    result=path_to_lcnc_bin+command;
    system(result.c_str());

    ui->pushButton_mode_auto->setStyleSheet(darkgrey);
    ui->pushButton_mode_jog->setStyleSheet(green);
}

void MainWindow::jog_x_plus_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_x_plus 1";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_x_min_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_x_min 1";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_y_min_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_y_min 1";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_y_plus_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_y_plus 1";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_z_min_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_z_min 1";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_z_plus_pressed(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_z_plus 1";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_x_plus_released(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_x_plus 0";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_x_min_released(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_x_min 0";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_y_min_released(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_y_min 0";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_y_plus_released(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_y_plus 0";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_z_min_released(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_z_min 0";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

void MainWindow::jog_z_plus_released(){
    std::string path_to_lcnc_bin=ui->lineEdit_path_to_lcnc_bin->text().toStdString();
    std::string command="/bin/./halcmd setp hal_trajectory.jog_z_plus 0";
    std::string result=path_to_lcnc_bin+command;
    system(result.c_str());
}

