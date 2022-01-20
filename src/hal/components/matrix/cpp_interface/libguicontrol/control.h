#ifndef CONTROL_H
#define CONTROL_H

#include <QWidget>
#include <iostream>
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSlider>
#include <QScrollBar>
#include <QDial>
#include <QPalette>

//!  qdir.h changed :enum SortFlag -> Unsorted into Unsorteds, is violating x11 etc.
#include <QFileDialog>

//! Needed to retrieve palette data.
#include "mainwindow.h"

//! Read textfile.
#include <iostream>
#include <fstream>
#include <string>

//! Needed for *ptr.
#include "cpp_interface.h"
#include "external_var_gcode.h"
#include "libocct/opencascade.h"

namespace Ui {
class Control;
}

class Control : public QWidget
{
    Q_OBJECT


public:
    explicit Control(QWidget *parent = nullptr);
    ~Control();

    //! Identification.
    unsigned int machine_id=0;
    //! Gcode ready to load flag.
    bool gcode_ready_to_load=0;
    bool gcode_ready_to_close=0;

    //! Mdi ready to load flag.
    bool mdi_ready_to_load=0;
    bool mdi_ready_to_close=0;

    int gcode_previousline=0;

    std::string gcodefilename;

    //! 1.Load gcode.
    void gcode_load(std::string filename, int gcodevec_id);
    //! 2.Create rich html text.
    void gcode_parse_blockstrings(int gcodevec_id);
    //! Helper functions:
    BLOCK line_lenght(BLOCK b);
    BLOCK arc_lenght(BLOCK b);

    void highlight_gcode_line(int i);

    void load();

    QToolButton *tb_estop;

    QPushButton *pb_online, *pb_offline;
    QPushButton *pb_manual, *pb_auto, *pb_mdi;
    QPushButton *pb_run, *pb_pause, *pb_stop;
    QPushButton *pb_m3, *pb_m5, *pb_m7, *pb_m8, *pb_m9;

    QPushButton *pb_j0_min, *pb_j0_plus;
    QPushButton *pb_j1_min, *pb_j1_plus;
    QPushButton *pb_j2_min, *pb_j2_plus;
    QPushButton *pb_j3_min, *pb_j3_plus;
    QPushButton *pb_j4_min, *pb_j4_plus;
    QPushButton *pb_j5_min, *pb_j5_plus;

    QPushButton *pb_cart_x_min, *pb_cart_x_plus;
    QPushButton *pb_cart_y_min, *pb_cart_y_plus;
    QPushButton *pb_cart_z_min, *pb_cart_z_plus;

    QPushButton *pb_euler_x_min, *pb_euler_x_plus;
    QPushButton *pb_euler_y_min, *pb_euler_y_plus;
    QPushButton *pb_euler_z_min, *pb_euler_z_plus;

    QPushButton *pb_tool_x_min, *pb_tool_x_plus;
    QPushButton *pb_tool_y_min, *pb_tool_y_plus;
    QPushButton *pb_tool_z_min, *pb_tool_z_plus;

    QPushButton *pb_mdi_exec, *pb_reload_gcode;

    QPushButton *pb_zero_rel_all, *pb_zero_rel_cartx, *pb_zero_rel_carty, *pb_zero_rel_cartz;
    QPushButton *pb_zero_rel_eulerx, *pb_zero_rel_eulery, *pb_zero_rel_eulerz;
    QPushButton *pb_zero_rel_J0, *pb_zero_rel_J1, *pb_zero_rel_J2, *pb_zero_rel_J3, *pb_zero_rel_J4, *pb_zero_rel_J5;

    QLineEdit *le_J0_abs, *le_J0_rel;
    QLineEdit *le_J1_abs, *le_J1_rel;
    QLineEdit *le_J2_abs, *le_J2_rel;
    QLineEdit *le_J3_abs, *le_J3_rel;
    QLineEdit *le_J4_abs, *le_J4_rel;
    QLineEdit *le_J5_abs, *le_J5_rel;

    QLineEdit *le_cart_x_abs, *le_cart_x_rel;
    QLineEdit *le_cart_y_abs, *le_cart_y_rel;
    QLineEdit *le_cart_z_abs, *le_cart_z_rel;

    QLineEdit *le_euler_x_abs, *le_euler_x_rel;
    QLineEdit *le_euler_y_abs, *le_euler_y_rel;
    QLineEdit *le_euler_z_abs, *le_euler_z_rel;

    QLineEdit *run_from_line;

    QDoubleSpinBox *dspb_maxacc_cart, *dspb_maxacc_euler, *dspb_stepsize, *dspb_feed_overide;

    QSpinBox *spb_maxvel_cart, *spb_maxvel_euler;

    QLabel *lbl_kinmode, *lbl_message;

public slots:

    void pushButton_toolButton_estop_pressed();

    void pushButton_page_control_pressed();

    void pushButton_page_gcode_pressed();

    void pushButton_page_user_pressed();

    void pushButton_online_pressed();

    void pushButton_offline_pressed();

    void pushButton_manual_pressed();

    void pushButton_auto_pressed();

    void pushButton_mdi_pressed();

    void pushButton_run_pressed();

    void pushButton_pause_pressed();

    void pushButton_stop_pressed();

    void pushButton_m3_pressed();

    void pushButton_m5_pressed();

    void pushButton_m7_pressed();

    void pushButton_m8_pressed();

    void pushButton_m9_pressed();

    void pushButton_open_gcode_pressed();

    void pushButton_close_gcode_pressed();

    void pushButton_mdi_exec_pressed();

private:
    Ui::Control *ui;

    QString stylesheet_red="background-color: rgba(158, 35, 35, 125); color: rgb(0, 0, 0);";
    QString stylesheet_green="background-color: rgba(74, 222, 2, 125); color: rgb(0, 0, 0);";
    QString stylesheet_orange="background-color: rgba(255, 170, 0, 125); color: rgb(0, 0, 0);";
    QString stylesheet_purple="background-color: rgba(255, 85, 255, 125); color: rgb(0, 0, 0);";
    QString stylesheet_grey="background-color: rgba(48, 53, 60, 125); color: rgb(171, 171, 171);";
};

#endif // CONTROL_H
