#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// libocc
#include <opencascade.h>
using namespace occ;

#include <draw_primitives.h>

#include "iostream"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void draw_result();

private slots:
    void on_pushButton_run_pressed();

    void on_doubleSpinBox_acceleration_valueChanged(double arg1);

    void on_doubleSpinBox_velocity_end_valueChanged(double arg1);

    void on_doubleSpinBox_displacement_valueChanged(double arg1);

    void on_doubleSpinBox_velocity_max_valueChanged(double arg1);

    void on_doubleSpinBox_velocity_begin_valueChanged(double arg1);

    void on_doubleSpinBox_gain_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    Opencascade *OpencascadeWidget;
};
#endif // MAINWINDOW_H
