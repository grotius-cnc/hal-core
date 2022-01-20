/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QDoubleSpinBox *doubleSpinBox_velocity_end;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_displacement;
    QDoubleSpinBox *doubleSpinBox_velocity_begin;
    QLabel *label_3;
    QPushButton *pushButton_run;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_velocity_max;
    QDoubleSpinBox *doubleSpinBox_gain;
    QLabel *label_velocity_end;
    QDoubleSpinBox *doubleSpinBox_acceleration;
    QLabel *label_max_acceleration;
    QLabel *label;
    QLabel *label_curve_time;
    QSpacerItem *horizontalSpacer;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout_opencascade;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(33, 38, 45, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush2(QColor(13, 17, 23, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        palette.setBrush(QPalette::Active, QPalette::Window, brush2);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        QBrush brush3(QColor(255, 255, 255, 128));
        brush3.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush3);
#endif
        QBrush brush4(QColor(190, 190, 190, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        QBrush brush5(QColor(0, 0, 0, 128));
        brush5.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush5);
#endif
        MainWindow->setPalette(palette);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_3 = new QGridLayout(centralwidget);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        doubleSpinBox_velocity_end = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_velocity_end->setObjectName(QString::fromUtf8("doubleSpinBox_velocity_end"));
        doubleSpinBox_velocity_end->setMinimum(-100000.000000000000000);
        doubleSpinBox_velocity_end->setMaximum(100000.000000000000000);
        doubleSpinBox_velocity_end->setValue(0.000000000000000);

        gridLayout->addWidget(doubleSpinBox_velocity_end, 3, 1, 1, 1);

        label_4 = new QLabel(centralwidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 7, 0, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 6, 2, 1, 1);

        doubleSpinBox_displacement = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_displacement->setObjectName(QString::fromUtf8("doubleSpinBox_displacement"));
        doubleSpinBox_displacement->setMinimum(-10000.000000000000000);
        doubleSpinBox_displacement->setMaximum(10000.000000000000000);
        doubleSpinBox_displacement->setSingleStep(0.500000000000000);
        doubleSpinBox_displacement->setValue(50.000000000000000);

        gridLayout->addWidget(doubleSpinBox_displacement, 6, 1, 1, 1);

        doubleSpinBox_velocity_begin = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_velocity_begin->setObjectName(QString::fromUtf8("doubleSpinBox_velocity_begin"));
        doubleSpinBox_velocity_begin->setMinimum(-10000.000000000000000);
        doubleSpinBox_velocity_begin->setMaximum(10000.000000000000000);
        doubleSpinBox_velocity_begin->setValue(8.000000000000000);

        gridLayout->addWidget(doubleSpinBox_velocity_begin, 2, 1, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        pushButton_run = new QPushButton(centralwidget);
        pushButton_run->setObjectName(QString::fromUtf8("pushButton_run"));

        gridLayout->addWidget(pushButton_run, 0, 1, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 4, 0, 1, 1);

        doubleSpinBox_velocity_max = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_velocity_max->setObjectName(QString::fromUtf8("doubleSpinBox_velocity_max"));
        doubleSpinBox_velocity_max->setMinimum(-10000.000000000000000);
        doubleSpinBox_velocity_max->setMaximum(10000.000000000000000);
        doubleSpinBox_velocity_max->setValue(5.000000000000000);

        gridLayout->addWidget(doubleSpinBox_velocity_max, 4, 1, 1, 1);

        doubleSpinBox_gain = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_gain->setObjectName(QString::fromUtf8("doubleSpinBox_gain"));
        doubleSpinBox_gain->setAccelerated(false);
        doubleSpinBox_gain->setMaximum(100.000000000000000);
        doubleSpinBox_gain->setSingleStep(5.000000000000000);
        doubleSpinBox_gain->setValue(100.000000000000000);

        gridLayout->addWidget(doubleSpinBox_gain, 7, 1, 1, 1);

        label_velocity_end = new QLabel(centralwidget);
        label_velocity_end->setObjectName(QString::fromUtf8("label_velocity_end"));

        gridLayout->addWidget(label_velocity_end, 3, 0, 1, 1);

        doubleSpinBox_acceleration = new QDoubleSpinBox(centralwidget);
        doubleSpinBox_acceleration->setObjectName(QString::fromUtf8("doubleSpinBox_acceleration"));
        doubleSpinBox_acceleration->setMinimum(-10000.000000000000000);
        doubleSpinBox_acceleration->setMaximum(10000.000000000000000);
        doubleSpinBox_acceleration->setSingleStep(0.100000000000000);
        doubleSpinBox_acceleration->setValue(1.000000000000000);

        gridLayout->addWidget(doubleSpinBox_acceleration, 1, 1, 1, 1);

        label_max_acceleration = new QLabel(centralwidget);
        label_max_acceleration->setObjectName(QString::fromUtf8("label_max_acceleration"));

        gridLayout->addWidget(label_max_acceleration, 1, 0, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 6, 0, 1, 1);

        label_curve_time = new QLabel(centralwidget);
        label_curve_time->setObjectName(QString::fromUtf8("label_curve_time"));

        gridLayout->addWidget(label_curve_time, 6, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 3, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

        frame = new QFrame(centralwidget);
        frame->setObjectName(QString::fromUtf8("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_opencascade = new QGridLayout();
        gridLayout_opencascade->setObjectName(QString::fromUtf8("gridLayout_opencascade"));

        gridLayout_2->addLayout(gridLayout_opencascade, 0, 0, 1, 1);


        gridLayout_3->addWidget(frame, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "scurve pro", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Gain 100%=full scurve", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Curve time:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Velocity begin", nullptr));
        pushButton_run->setText(QCoreApplication::translate("MainWindow", "run", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Velocity max", nullptr));
        label_velocity_end->setText(QCoreApplication::translate("MainWindow", "Velocity end", nullptr));
        label_max_acceleration->setText(QCoreApplication::translate("MainWindow", "Acceleration", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Displacement", nullptr));
        label_curve_time->setText(QCoreApplication::translate("MainWindow", "[Ct]", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
