#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <iostream>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

#ifdef Success
#undef Success
#endif

// libocc
#include <libocct/opencascade.h>

// libspline
#include <libspline/cubic_spline.h>

// libdxfrw
#include <libdxfrw/libdxfrw_functions.h>

// libgcode
#include <libgcode/gcode_interface.h>

// global c & c++ variable
#include "external_var.h"

extern "C" void command(MACHINE *ptr);

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Opencascade *OpencascadeWidget;
    void update_opencascade();
    int load_stepfiles();

private slots:

    void go_home();

    void jog_joint_min_start();
    void jog_joint_plus_start();

    void cartx_min_start();
    void cartx_plus_start();

    void carty_min_start();
    void carty_plus_start();

    void cartz_min_start();
    void cartz_plus_start();

    void eulerx_min_start();
    void eulerx_plus_start();

    void eulery_min_start();
    void eulery_plus_start();

    void eulerz_min_start();
    void eulerz_plus_start();

    void toolx_min_start();
    void toolx_plus_start();
    void tooly_min_start();
    void tooly_plus_start();
    void toolz_min_start();
    void toolz_plus_start();

    void tool_eulerx_min_start();
    void tool_eulerx_plus_start();
    void tool_eulery_min_start();
    void tool_eulery_plus_start();
    void tool_eulerz_min_start();
    void tool_eulerz_plus_start();

    void rt_motion();
    void set_maxvel_cart(int value);
    void set_maxvel_euler(int value);
    void set_maxvel_joints(int value);
    void set_joint(int value);

    void set_machine(int value);
    void set_mdi_pos();

    void set_machine_offset();

    void load_gcode();
    void move_gcode();

    void gcode_text(GCODE g);

    void mode_auto();
    void mode_mdi_manual();

    void set_relative_zero();

private:
    Ui::MainWindow *ui;
    QTimer *timer = new QTimer(this);
};

#endif // MAINWINDOW_H
