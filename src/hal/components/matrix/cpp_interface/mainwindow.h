#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "QLineEdit"
#include "QPushButton"
#include "QLabel"
#include "QDoubleSpinBox"
#include "QComboBox"
#include "QStackedWidget"
#include "QListWidget"
#include "QGridLayout"
#include "QScrollArea"
#include "QScrollBar"
#include "QTextEdit"
#include "QTextCursor"
#include "QTextBlock"
#include "QTextBlockFormat"
#include "QPlainTextEdit"

#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>
#include <csignal>
#include <memory>
#include <atomic>

//! Make conversion's easy:
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

#ifdef Success
#undef Success
#endif

// global c & c++ variable
#include "external_var.h"
#include "external_var_gcode.h"

// libocc
#include <libocct/opencascade.h>

// libspline
#include <libspline/cubic_spline.h>

// libdxfrw
#include <libdxfrw/libdxfrw_functions.h>

// libgcode
#include <libgcode/gcode_interface.h>

// libskynet
#include "libskynet/matrix_3d.h"
#include "libskynet/read_textfile.h"

// Libkinematic
#include "libkinematic/kinematic_interface.h"

// libhal
#include "libhal/hal_interface.h"

// libguicontrol
#include "libguicontrol/control.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void init();

    class Opencascade *OpencascadeWidget;
    void mainloop();

private:

    //! Create dynamic gui controls.
    struct GUICONTROLS{
        class Control *control;
    };
    //! Holds the pointer to a certain lineedit. With this info we can update the correct lineedit item.
    std::vector<GUICONTROLS> guicontrolvec;

private slots:

    void view_orthographic();
    void view_perspective();
    void transparancy(double value);

    void set_lineedit_values();
    void set_label_values();

    void on_actionView_perspective_triggered();

    void on_actionView_orthographic_triggered();

    void on_action0_2_triggered();

    void on_action25_triggered();

    void on_action75_triggered();

    void on_action100_2_triggered();

    void on_actionHide_View_triggered();

    void on_actionShow_View_triggered();

    void on_actionHide_controls_triggered();

    void on_actionShow_controls_triggered();

private:
    Ui::MainWindow *ui;
    QTimer *timer = new QTimer(this);
    //! Predefined stylesheets:

    QString blue_text="color: rgb(0, 85, 255);";
    QString red_text="color: rgb(255, 0, 0);";
    QString white_text="color: rgb(255, 255, 255);";
    QString green_text="color: rgb(0, 255, 0);";

    QString stylesheet_red="background-color: rgba(158, 35, 35, 125); color: rgb(0, 0, 0);";
    QString stylesheet_green="background-color: rgba(74, 222, 2, 125); color: rgb(0, 0, 0);";
    QString stylesheet_orange="background-color: rgba(255, 170, 0, 125); color: rgb(0, 0, 0);";
    QString stylesheet_purple="background-color: rgba(255, 85, 255, 125); color: rgb(0, 0, 0);";
    QString stylesheet_grey="background-color: rgba(48, 53, 60, 125); color: rgb(171, 171, 171);";
};

#endif // MAINWINDOW_H














