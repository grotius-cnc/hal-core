/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QPushButton *pushButton_12;
    QPushButton *pushButton_9;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_20;
    QPushButton *pushButton_17;
    QPushButton *pushButton_2;
    QPushButton *pushButton_13;
    QPushButton *pushButton_8;
    QPushButton *pushButton_14;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_6;
    QSpinBox *spinBox_2;
    QPushButton *pushButton_18;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_10;
    QPushButton *pushButton_15;
    QPushButton *pushButton;
    QPushButton *pushButton_7;
    QPushButton *pushButton_19;
    QPushButton *pushButton_16;
    QSpinBox *spinBox;
    QPushButton *pushButton_11;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_3;
    QLabel *label_3;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(306, 364);
        QPalette palette;
        QBrush brush(QColor(142, 148, 156, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(33, 38, 45, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush2);
        QBrush brush3(QColor(139, 148, 158, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush3);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush4(QColor(13, 17, 23, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush4);
        QBrush brush5(QColor(0, 0, 0, 128));
        brush5.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush5);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush4);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush5);
#endif
        QBrush brush6(QColor(190, 190, 190, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush4);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush5);
#endif
        Dialog->setPalette(palette);
        gridLayout_2 = new QGridLayout(Dialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(Dialog);
        label->setObjectName(QString::fromUtf8("label"));
        QPalette palette1;
        QBrush brush7(QColor(56, 56, 56, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush7);
        QBrush brush8(QColor(170, 255, 255, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        label->setPalette(palette1);

        gridLayout->addWidget(label, 2, 0, 1, 1);

        pushButton_12 = new QPushButton(Dialog);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette2.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette2.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette2.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_12->setPalette(palette2);

        gridLayout->addWidget(pushButton_12, 7, 0, 1, 1);

        pushButton_9 = new QPushButton(Dialog);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette3.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette3.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette3.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette3.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_9->setPalette(palette3);

        gridLayout->addWidget(pushButton_9, 6, 1, 1, 1);

        lineEdit_4 = new QLineEdit(Dialog);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        gridLayout->addWidget(lineEdit_4, 8, 3, 1, 1);

        pushButton_5 = new QPushButton(Dialog);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette4.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette4.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette4.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_5->setPalette(palette4);

        gridLayout->addWidget(pushButton_5, 10, 0, 1, 1);

        pushButton_6 = new QPushButton(Dialog);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette5.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette5.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette5.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_6->setPalette(palette5);

        gridLayout->addWidget(pushButton_6, 10, 1, 1, 1);

        pushButton_20 = new QPushButton(Dialog);
        pushButton_20->setObjectName(QString::fromUtf8("pushButton_20"));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette6.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette6.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette6.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_20->setPalette(palette6);

        gridLayout->addWidget(pushButton_20, 4, 1, 1, 1);

        pushButton_17 = new QPushButton(Dialog);
        pushButton_17->setObjectName(QString::fromUtf8("pushButton_17"));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette7.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette7.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette7.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_17->setPalette(palette7);

        gridLayout->addWidget(pushButton_17, 14, 0, 1, 1);

        pushButton_2 = new QPushButton(Dialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette8.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette8.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette8.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette8.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_2->setPalette(palette8);

        gridLayout->addWidget(pushButton_2, 8, 0, 1, 1);

        pushButton_13 = new QPushButton(Dialog);
        pushButton_13->setObjectName(QString::fromUtf8("pushButton_13"));
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette9.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette9.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette9.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette9.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette9.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_13->setPalette(palette9);

        gridLayout->addWidget(pushButton_13, 12, 0, 1, 1);

        pushButton_8 = new QPushButton(Dialog);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette10.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette10.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette10.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette10.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette10.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_8->setPalette(palette10);

        gridLayout->addWidget(pushButton_8, 5, 1, 1, 1);

        pushButton_14 = new QPushButton(Dialog);
        pushButton_14->setObjectName(QString::fromUtf8("pushButton_14"));
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette11.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette11.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette11.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette11.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette11.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_14->setPalette(palette11);

        gridLayout->addWidget(pushButton_14, 12, 1, 1, 1);

        lineEdit = new QLineEdit(Dialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 5, 3, 1, 1);

        label_2 = new QLabel(Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette12.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette12.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette12.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette12.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette12.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        label_2->setPalette(palette12);

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        lineEdit_2 = new QLineEdit(Dialog);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

        gridLayout->addWidget(lineEdit_2, 6, 3, 1, 1);

        lineEdit_6 = new QLineEdit(Dialog);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        gridLayout->addWidget(lineEdit_6, 10, 3, 1, 1);

        spinBox_2 = new QSpinBox(Dialog);
        spinBox_2->setObjectName(QString::fromUtf8("spinBox_2"));
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette13.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette13.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette13.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette13.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette13.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        spinBox_2->setPalette(palette13);

        gridLayout->addWidget(spinBox_2, 3, 1, 1, 1);

        pushButton_18 = new QPushButton(Dialog);
        pushButton_18->setObjectName(QString::fromUtf8("pushButton_18"));
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette14.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette14.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette14.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette14.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette14.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_18->setPalette(palette14);

        gridLayout->addWidget(pushButton_18, 14, 1, 1, 1);

        pushButton_3 = new QPushButton(Dialog);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette15.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette15.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette15.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette15.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette15.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_3->setPalette(palette15);

        gridLayout->addWidget(pushButton_3, 9, 0, 1, 1);

        pushButton_4 = new QPushButton(Dialog);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette16.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette16.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette16.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette16.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette16.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_4->setPalette(palette16);

        gridLayout->addWidget(pushButton_4, 9, 1, 1, 1);

        pushButton_10 = new QPushButton(Dialog);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette17.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette17.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette17.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette17.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette17.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_10->setPalette(palette17);

        gridLayout->addWidget(pushButton_10, 6, 0, 1, 1);

        pushButton_15 = new QPushButton(Dialog);
        pushButton_15->setObjectName(QString::fromUtf8("pushButton_15"));
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette18.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette18.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette18.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette18.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette18.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_15->setPalette(palette18);

        gridLayout->addWidget(pushButton_15, 13, 0, 1, 1);

        pushButton = new QPushButton(Dialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette19.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette19.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette19.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette19.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette19.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton->setPalette(palette19);

        gridLayout->addWidget(pushButton, 8, 1, 1, 1);

        pushButton_7 = new QPushButton(Dialog);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        QPalette palette20;
        palette20.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette20.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette20.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette20.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette20.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette20.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_7->setPalette(palette20);

        gridLayout->addWidget(pushButton_7, 5, 0, 1, 1);

        pushButton_19 = new QPushButton(Dialog);
        pushButton_19->setObjectName(QString::fromUtf8("pushButton_19"));
        QPalette palette21;
        palette21.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette21.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette21.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette21.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette21.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette21.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_19->setPalette(palette21);

        gridLayout->addWidget(pushButton_19, 4, 0, 1, 1);

        pushButton_16 = new QPushButton(Dialog);
        pushButton_16->setObjectName(QString::fromUtf8("pushButton_16"));
        QPalette palette22;
        palette22.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette22.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette22.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette22.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette22.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette22.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_16->setPalette(palette22);

        gridLayout->addWidget(pushButton_16, 13, 1, 1, 1);

        spinBox = new QSpinBox(Dialog);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        QPalette palette23;
        QBrush brush9(QColor(187, 196, 203, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette23.setBrush(QPalette::Active, QPalette::WindowText, brush9);
        palette23.setBrush(QPalette::Active, QPalette::Light, brush2);
        palette23.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette23.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette23.setBrush(QPalette::Inactive, QPalette::WindowText, brush9);
        palette23.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette23.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette23.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette23.setBrush(QPalette::Disabled, QPalette::WindowText, brush6);
        palette23.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette23.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette23.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        spinBox->setPalette(palette23);

        gridLayout->addWidget(spinBox, 2, 1, 1, 1);

        pushButton_11 = new QPushButton(Dialog);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
        QPalette palette24;
        palette24.setBrush(QPalette::Active, QPalette::Text, brush7);
        palette24.setBrush(QPalette::Active, QPalette::Base, brush8);
        palette24.setBrush(QPalette::Inactive, QPalette::Text, brush7);
        palette24.setBrush(QPalette::Inactive, QPalette::Base, brush8);
        palette24.setBrush(QPalette::Disabled, QPalette::Text, brush6);
        palette24.setBrush(QPalette::Disabled, QPalette::Base, brush4);
        pushButton_11->setPalette(palette24);

        gridLayout->addWidget(pushButton_11, 7, 1, 1, 1);

        lineEdit_5 = new QLineEdit(Dialog);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        gridLayout->addWidget(lineEdit_5, 9, 3, 1, 1);

        lineEdit_7 = new QLineEdit(Dialog);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));

        gridLayout->addWidget(lineEdit_7, 4, 3, 1, 1);

        lineEdit_3 = new QLineEdit(Dialog);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        gridLayout->addWidget(lineEdit_3, 7, 3, 1, 1);

        label_3 = new QLabel(Dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 14, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QCoreApplication::translate("Dialog", "Skynet Cyberdyne Realtime Control Unit", nullptr));
        label->setText(QCoreApplication::translate("Dialog", "Machine", nullptr));
        pushButton_12->setText(QCoreApplication::translate("Dialog", "Cart Z -", nullptr));
        pushButton_9->setText(QCoreApplication::translate("Dialog", "Cart Y +", nullptr));
        lineEdit_4->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        pushButton_5->setText(QCoreApplication::translate("Dialog", "Euler Z -", nullptr));
        pushButton_6->setText(QCoreApplication::translate("Dialog", "Euler Z +", nullptr));
        pushButton_20->setText(QCoreApplication::translate("Dialog", "Joint +", nullptr));
        pushButton_17->setText(QCoreApplication::translate("Dialog", "Tool Z -", nullptr));
        pushButton_2->setText(QCoreApplication::translate("Dialog", "Euler X -", nullptr));
        pushButton_13->setText(QCoreApplication::translate("Dialog", "Tool X -", nullptr));
        pushButton_8->setText(QCoreApplication::translate("Dialog", "Cart X +", nullptr));
        pushButton_14->setText(QCoreApplication::translate("Dialog", "Tool X +", nullptr));
        lineEdit->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        label_2->setText(QCoreApplication::translate("Dialog", "Joint", nullptr));
        lineEdit_2->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        lineEdit_6->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        pushButton_18->setText(QCoreApplication::translate("Dialog", "Tool Z +", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Dialog", "Euler Y -", nullptr));
        pushButton_4->setText(QCoreApplication::translate("Dialog", "Euler Y +", nullptr));
        pushButton_10->setText(QCoreApplication::translate("Dialog", "Cart Y -", nullptr));
        pushButton_15->setText(QCoreApplication::translate("Dialog", "Tool Y -", nullptr));
        pushButton->setText(QCoreApplication::translate("Dialog", "Euler X +", nullptr));
        pushButton_7->setText(QCoreApplication::translate("Dialog", "Cart X -", nullptr));
        pushButton_19->setText(QCoreApplication::translate("Dialog", "Joint -", nullptr));
        pushButton_16->setText(QCoreApplication::translate("Dialog", "Tool Y +", nullptr));
        pushButton_11->setText(QCoreApplication::translate("Dialog", "Cart Z +", nullptr));
        lineEdit_5->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        lineEdit_7->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        lineEdit_3->setText(QCoreApplication::translate("Dialog", "0", nullptr));
        label_3->setText(QCoreApplication::translate("Dialog", "Units: mm degrees", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
