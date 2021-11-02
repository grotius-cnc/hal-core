#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>

//! Hal
#ifndef ULAPI
#define ULAPI
#endif
#include "hal.h"

//! Filechooserdialog
#include <libdialog/portable-file-dialogs.h>
#if _WIN32
#define DEFAULT_PATH "C:\\"
#else
#define DEFAULT_PATH "/tmp"
#endif

//! Getdirectory
#include <libskynet/directory.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void mode_auto();
    void mode_jog();

    void mode_auto_run_pressed();
    void mode_auto_stop_pressed();

    void jog_x_plus_pressed();
    void jog_x_min_pressed();
    void jog_y_plus_pressed();
    void jog_y_min_pressed();
    void jog_z_plus_pressed();
    void jog_z_min_pressed();

    void jog_x_plus_released();
    void jog_x_min_released();
    void jog_y_plus_released();
    void jog_y_min_released();
    void jog_z_plus_released();
    void jog_z_min_released();

    void loadfile();
    void gcodeblock();
    void tooloffset();
    void max_values();

    void m3_on();
    void m3_off();
    void m7_on();
    void m7_off();
    void m8_on();
    void m8_off();
    void power_rpm();
    void load_mdi();

private:
    Ui::MainWindow *ui;

    QString darkgrey="background-color: rgba(65, 65, 65, 0);\ncolor: rgb(255, 255, 255);";
    QString green="background-color: rgba(170, 255, 0, 0);\ncolor: rgb(0, 0, 0);";
    QString red="background-color: rgba(242, 0, 0, 0);\ncolor: rgb(0, 0, 0);";
    QString orange="background-color: rgba(255, 170, 0, 0);\ncolor: rgb(0, 0, 0);";
    QString grey="background-color: rgba(81, 81, 81, 0);\ncolor: rgb(255, 255, 255);";
};
#endif // MAINWINDOW_H
