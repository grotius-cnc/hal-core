#include "mainwindow.h"
#include "ui_mainwindow.h"

bool loadstepfiles=0;
MACHINE *e_ptr;
GCODE gcode;

//! From the hal component.
extern "C" void mainwindow_ptr(struct MACHINE *ptr){
    e_ptr=ptr;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpencascadeWidget = new Opencascade(this);
    ui->gridLayout_opencascade->addWidget(OpencascadeWidget);

    connect(timer, &QTimer::timeout, this, &MainWindow::update_opencascade);

    double t=e_ptr[0].data.screenupdatems;
    if(t<20){
        t=20;
        std::cout<<"screenupdatems correction to 100ms"<<std::endl;
    }
    timer->start(t);

    QObject::connect(ui->pushButton_joint_min, SIGNAL(pressed()),SLOT(jog_joint_min_start()));
    QObject::connect(ui->pushButton_joint_plus, SIGNAL(pressed()),SLOT(jog_joint_plus_start()));

    QObject::connect(ui->pushButton_cartx_min, SIGNAL(pressed()),SLOT(cartx_min_start()));
    QObject::connect(ui->pushButton_cartx_plus, SIGNAL(pressed()),SLOT(cartx_plus_start()));
    QObject::connect(ui->pushButton_carty_min, SIGNAL(pressed()),SLOT(carty_min_start()));
    QObject::connect(ui->pushButton_carty_plus, SIGNAL(pressed()),SLOT(carty_plus_start()));
    QObject::connect(ui->pushButton_cartz_min, SIGNAL(pressed()),SLOT(cartz_min_start()));
    QObject::connect(ui->pushButton_cartz_plus, SIGNAL(pressed()),SLOT(cartz_plus_start()));

    QObject::connect(ui->pushButton_eulerx_min, SIGNAL(pressed()),SLOT(eulerx_min_start()));
    QObject::connect(ui->pushButton_eulerx_plus, SIGNAL(pressed()),SLOT(eulerx_plus_start()));
    QObject::connect(ui->pushButton_eulery_min, SIGNAL(pressed()),SLOT(eulery_min_start()));
    QObject::connect(ui->pushButton_eulery_plus, SIGNAL(pressed()),SLOT(eulery_plus_start()));
    QObject::connect(ui->pushButton_eulerz_min, SIGNAL(pressed()),SLOT(eulerz_min_start()));
    QObject::connect(ui->pushButton_eulerz_plus, SIGNAL(pressed()),SLOT(eulerz_plus_start()));

    QObject::connect(ui->pushButton_toolx_min, SIGNAL(pressed()),SLOT(toolx_min_start()));
    QObject::connect(ui->pushButton_toolx_plus, SIGNAL(pressed()),SLOT(toolx_plus_start()));
    QObject::connect(ui->pushButton_tooly_min, SIGNAL(pressed()),SLOT(tooly_min_start()));
    QObject::connect(ui->pushButton_tooly_plus, SIGNAL(pressed()),SLOT(tooly_plus_start()));
    QObject::connect(ui->pushButton_toolz_min, SIGNAL(pressed()),SLOT(toolz_min_start()));
    QObject::connect(ui->pushButton_toolz_plus, SIGNAL(pressed()),SLOT(toolz_plus_start()));

    QObject::connect(ui-> pushButton_tool_eulerx_min, SIGNAL(pressed()),SLOT(tool_eulerx_min_start()));
    QObject::connect(ui-> pushButton_tool_eulerx_plus, SIGNAL(pressed()),SLOT(tool_eulerx_plus_start()));
    QObject::connect(ui-> pushButton_tool_eulery_min, SIGNAL(pressed()),SLOT(tool_eulery_min_start()));
    QObject::connect(ui-> pushButton_tool_eulery_plus, SIGNAL(pressed()),SLOT(tool_eulery_plus_start()));
    QObject::connect(ui-> pushButton_tool_eulerz_min, SIGNAL(pressed()),SLOT(tool_eulerz_min_start()));
    QObject::connect(ui-> pushButton_tool_eulerz_plus, SIGNAL(pressed()),SLOT(tool_eulerz_plus_start()));

    QObject::connect(ui->pushButton_go_home, SIGNAL(pressed()),SLOT(go_home()));

    QObject::connect(ui->checkBox_rt_motion, SIGNAL(pressed()),SLOT(rt_motion()));

    QObject::connect(ui->spinBox_max_vel_cart, SIGNAL(valueChanged(int)),SLOT(set_maxvel_cart(int)));
    QObject::connect(ui->spinBox_max_vel_euler, SIGNAL(valueChanged(int)),SLOT(set_maxvel_euler(int)));
    QObject::connect(ui->spinBox_max_vel_joints, SIGNAL(valueChanged(int)),SLOT(set_maxvel_joints(int)));

    QObject::connect(ui->spinBox_machine, SIGNAL(valueChanged(int)),SLOT(set_machine(int)));


    QObject::connect(ui->spinBox_joint, SIGNAL(valueChanged(int)),SLOT(set_joint(int)));

    QObject::connect(ui->pushButton_goto_position, SIGNAL(pressed()),SLOT(set_mdi_pos()));

    QObject::connect(ui->pushButton_set_machine_offset, SIGNAL(pressed()),SLOT(set_machine_offset()));

    QObject::connect(ui->pushButton_load_gcode, SIGNAL(pressed()),SLOT(load_gcode()));
    QObject::connect(ui->pushButton_move_gcode, SIGNAL(pressed()),SLOT(move_gcode()));

    QObject::connect(ui->pushButton_mode_auto, SIGNAL(pressed()),SLOT(mode_auto()));
    QObject::connect(ui->pushButton_mdi_manual, SIGNAL(pressed()),SLOT(mode_mdi_manual()));

    QObject::connect(ui->pushButton_set_relative_zero, SIGNAL(pressed()),SLOT(set_relative_zero()));

    ui->label_cart_maxvel->setText(QString::number(e_ptr[0].gui_maxvel_cart,'f',3));
    ui->label_cart_curvel->setText(QString::number(e_ptr[0].gui_maxvel_cart,'f',3));
    ui->label_euler_maxvel->setText(QString::number(e_ptr[0].gui_maxvel_euler,'f',3));
    ui->label_euler_curvel->setText(QString::number(e_ptr[0].gui_maxvel_euler,'f',3));
    ui->label_joints_maxvel->setText(QString::number(e_ptr[0].joint[0].gui_maxvel,'f',3));
    ui->label_joints_curvel->setText(QString::number(e_ptr[0].joint[0].gui_maxvel,'f',3));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_relative_zero(){
    int i=ui->spinBox_machine->value();
    for(int j=0; j<6; j++){
        e_ptr[i].curoffset_cart_euler_rel[j]=e_ptr[i].curpos_cart_euler[j];
    }

    //! Refresh gcode position
    gcode.offset.x=e_ptr[i].curoffset_cart_euler_rel[0];
    gcode.offset.y=e_ptr[i].curoffset_cart_euler_rel[1];
    gcode.offset.z=e_ptr[i].curoffset_cart_euler_rel[2];

    gcode.rotation.x=e_ptr[i].curoffset_cart_euler_rel[3];
    gcode.rotation.y=e_ptr[i].curoffset_cart_euler_rel[4];
    gcode.rotation.z=e_ptr[i].curoffset_cart_euler_rel[5];

    OpencascadeWidget->parse_gcode(gcode);

    //! Values are updated by the opencascade updater.
    command(e_ptr);
}

void MainWindow::mode_mdi_manual(){
    int i=ui->spinBox_machine->value();
    e_ptr[i].mode=MDI_MANUAL;
    e_ptr[i].exec_gcodeblock=0;
    command(e_ptr);
}

void MainWindow::mode_auto(){
    int i=ui->spinBox_machine->value();
    e_ptr[i].mode=AUTO;
    command(e_ptr);
}

void MainWindow::load_gcode(){
    int i=ui->spinBox_machine->value();

    //! gcode is a system wide variable.
    gcode=gcode_interface().load_gcode();
    //move_gcode();

    gcode.offset.x=e_ptr[i].curoffset_cart_euler_rel[0];
    gcode.offset.y=e_ptr[i].curoffset_cart_euler_rel[1];
    gcode.offset.z=e_ptr[i].curoffset_cart_euler_rel[2];

    gcode.rotation.x=e_ptr[i].curoffset_cart_euler_rel[3];
    gcode.rotation.y=e_ptr[i].curoffset_cart_euler_rel[4];
    gcode.rotation.z=e_ptr[i].curoffset_cart_euler_rel[5];

    OpencascadeWidget->parse_gcode(gcode);
    gcode_text(gcode);
    ui->label_gcodefile->setText(QString::fromStdString(gcode.gcodefilename));
}

void MainWindow::gcode_text(GCODE g){

    //! Show gcode file in text preview
    ui->listWidget_gcode->clear();

    if(g.blockvec.size()>0){ // Failsafe solution.
        for(unsigned int i=0; i<g.blockvec.size(); i++){

            QString str;
            //! Linenumber indicator starts with value 0
            str+="N";
            str+=QString::number(i);
            str+=" ";
            if(g.blockvec.at(i).blocktype==G0){
                str+=QString("G0 ");
            }
            if(g.blockvec.at(i).blocktype==G1){
                str+=QString("G1 ");
            }
            if(g.blockvec.at(i).blocktype==G2){
                str+=QString("G2 ");
            }
            if(g.blockvec.at(i).blocktype==G3){
                str+=QString("G3 ");
            }
            //! Cartesian xyz
            str+=" X:";
            str+=QString::number(g.blockvec.at(i).X,'f',3);
            str+=" Y:";
            str+=QString::number(g.blockvec.at(i).Y,'f',3);
            str+=" Z:";
            //! Arc center xyz
            str+=QString::number(g.blockvec.at(i).Z,'f',3);
            str+=" I:";
            str+=QString::number(g.blockvec.at(i).I,'f',3);
            str+=" J:";
            str+=QString::number(g.blockvec.at(i).J,'f',3);
            str+=" K:";
            str+=QString::number(g.blockvec.at(i).K,'f',3);
            //! Euler xyz
            str+=" U:";
            str+=QString::number(g.blockvec.at(i).U,'f',3);
            str+=" V:";
            str+=QString::number(g.blockvec.at(i).V,'f',3);
            str+=" W:";
            str+=QString::number(g.blockvec.at(i).W,'f',3);
            //! Feedrate G1,G2,G3
            str+=" F:";
            str+=QString::number(g.blockvec.at(i).F,'f',3);

            // Extra info:
            str+="\n"; // Newline.
            str+=" Lenght:";
            str+=QString::number(g.blockvec.at(i).blocklenght,'f',3);
            str+=" XCen:";
            str+=QString::number(g.blockvec.at(i).center.x,'f',3);
            str+=" YCen:";
            str+=QString::number(g.blockvec.at(i).center.y,'f',3);
            str+=" ZCen:";
            str+=QString::number(g.blockvec.at(i).center.z,'f',3);
            ui->listWidget_gcode->addItem(str);
        }
    }
}

void MainWindow::move_gcode(){
    //! Move gcode to user defined gui input values.
    POINT p; EULER e;
    gcode.offset.x=ui->lineEdit_gcode_offset_cartx->text().toDouble();
    gcode.offset.y=ui->lineEdit_gcode_offset_carty->text().toDouble();
    gcode.offset.z=ui->lineEdit_gcode_offset_cartz->text().toDouble();
    gcode.rotation.x=ui->lineEdit_gcode_offset_eulerx->text().toDouble()*toRadians;
    gcode.rotation.y=ui->lineEdit_gcode_offset_eulery->text().toDouble()*toRadians;
    gcode.rotation.z=ui->lineEdit_gcode_offset_eulerz->text().toDouble()*toRadians;
    OpencascadeWidget->move_gcode(gcode.offset,gcode.rotation);
}

void MainWindow::set_machine_offset(){
    int i=ui->spinBox_machine->value();
    double cartx=ui->lineEdit_offset_cartx->text().toDouble();
    double carty=ui->lineEdit_offset_carty->text().toDouble();
    double cartz=ui->lineEdit_offset_cartz->text().toDouble();
    double eulerx=ui->lineEdit_offset_eulerx->text().toDouble();
    double eulery=ui->lineEdit_offset_eulery->text().toDouble();
    double eulerz=ui->lineEdit_offset_eulerz->text().toDouble();

    e_ptr[i].machine_offset.x=cartx;
    e_ptr[i].machine_offset.y=carty;
    e_ptr[i].machine_offset.z=cartz;
    e_ptr[i].machine_rotation.x=eulerx*toRadians;
    e_ptr[i].machine_rotation.y=eulery*toRadians;
    e_ptr[i].machine_rotation.z=eulerz*toRadians;
    command(e_ptr);
}

void MainWindow::set_machine(int value){
    if(e_ptr[value].rt_motion){
        ui->checkBox_rt_motion->setChecked(true);
    } else {
        ui->checkBox_rt_motion->setChecked(false);
    }
}

void MainWindow::set_joint(int value){
    int i=ui->spinBox_machine->value();
    int j=ui->spinBox_joint->value();
    double maxvel=e_ptr[i].joint[j].gui_maxvel;

    ui->label_joints_maxvel->setText(QString::number(maxvel));
    ui->label_joints_curvel->setText(QString::number(ui->spinBox_max_vel_joints->value()*(0.01*e_ptr[i].joint[j].gui_maxvel)));
}

void MainWindow::set_maxvel_cart(int value){
    int i=ui->spinBox_machine->value();

    e_ptr[i].maxvel_cart=double(value)*(0.01*e_ptr[i].gui_maxvel_cart);
    ui->label_cart_curvel->setText(QString::number(e_ptr[i].maxvel_cart,'f',3));
}

void MainWindow::set_maxvel_euler(int value){
    int i=ui->spinBox_machine->value();

    e_ptr[i].maxvel_euler=double(value)*(0.01*e_ptr[i].gui_maxvel_euler);
    ui->label_euler_curvel->setText(QString::number(e_ptr[i].maxvel_euler,'f',3));
}

void MainWindow::set_maxvel_joints(int value){
    int i=ui->spinBox_machine->value();
    int j=ui->spinBox_joint->value();

    e_ptr[i].joint[j].maxvel=double(value)*(0.01*e_ptr[i].joint[j].gui_maxvel);
    ui->label_joints_curvel->setText(QString::number(e_ptr[i].joint[j].maxvel,'f',3));
}

void MainWindow::rt_motion(){
    int i=ui->spinBox_machine->value();
    if(!ui->checkBox_rt_motion->isChecked()){
        e_ptr[i].rt_motion=1;
        std::cout<<"checked"<<std::endl;
    }
    if(ui->checkBox_rt_motion->isChecked()){
        e_ptr[i].rt_motion=0;
        std::cout<<"not checked"<<std::endl;
    };
    command(e_ptr);
}

void MainWindow::go_home(){
    int i=ui->spinBox_machine->value();
    e_ptr[i].kinmode=KINMODE::fk_rapid;
    for(int j=0; j<e_ptr[i].data.joints[i]; j++){
        if(e_ptr[i].rt_motion){
            e_ptr[i].joint[j].tarpos=0;
        } else {
            e_ptr[i].joint[j].curpos=0;
        }
    }

    command(e_ptr);
}

void MainWindow::jog_joint_min_start(){

    int i=ui->spinBox_machine->value();
    int j=ui->spinBox_joint->value();
    double v=ui->spinBox_stepsize->value();
    //! If the joint is of type::rotation, add the stepsize value in radians.
    if(e_ptr[i].joint[j].rotationtype==0 || e_ptr[i].joint[j].rotationtype==1 || e_ptr[i].joint[j].rotationtype==2 || e_ptr[i].joint[j].rotationtype==6){
        v*=toRadians;
    }
    e_ptr[i].kinmode=KINMODE::fk_rapid;
    if(e_ptr[i].rt_motion){

        for(int k=0; k<e_ptr[i].data.joints[i]; k++){
            e_ptr[i].joint[k].tarpos=e_ptr[i].joint[k].curpos;
        }

        e_ptr[i].joint[j].tarpos=-abs(v)+e_ptr[i].joint[j].curpos;
    } else {
        e_ptr[i].joint[j].curpos=-abs(v)+e_ptr[i].joint[j].curpos;
        e_ptr[i].joint[j].tarpos=e_ptr[i].joint[j].curpos;
    }
    command(e_ptr);
}

void MainWindow::jog_joint_plus_start(){

    int i=ui->spinBox_machine->value();
    int j=ui->spinBox_joint->value();
    double v=ui->spinBox_stepsize->value();
    //! If the joint is of type::rotation, add the stepsize value in radians.
    if(e_ptr[i].joint[j].rotationtype==0 || e_ptr[i].joint[j].rotationtype==1 || e_ptr[i].joint[j].rotationtype==2 || e_ptr[i].joint[j].rotationtype==6){
        v*=toRadians;
    }
    e_ptr[i].kinmode=KINMODE::fk_rapid;
    if(e_ptr[i].rt_motion){

        for(int k=0; k<e_ptr[i].data.joints[i]; k++){
            e_ptr[i].joint[k].tarpos=e_ptr[i].joint[k].curpos;
        }

        e_ptr[i].joint[j].tarpos=abs(v)+e_ptr[i].joint[j].curpos;
    } else {
        e_ptr[i].joint[j].curpos=abs(v)+e_ptr[i].joint[j].curpos;
        e_ptr[i].joint[j].tarpos=e_ptr[i].joint[j].curpos;
    }
    command(e_ptr);

}

void MainWindow::cartx_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[0]=-abs(v)+e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[0]=-abs(v)+e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
    }
    command(e_ptr);
}

void MainWindow::cartx_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[0]=abs(v)+e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[0]=abs(v)+e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
    }
    command(e_ptr);
}

void MainWindow::carty_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[1]=-abs(v)+e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[1]=-abs(v)+e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
    }
    command(e_ptr);
}

void MainWindow::carty_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[1]=abs(v)+e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[1]=abs(v)+e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
    }
    command(e_ptr);
}

void MainWindow::cartz_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[2]=-abs(v)+e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[2]=-abs(v)+e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
    }
    command(e_ptr);
}

void MainWindow::cartz_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[2]=abs(v)+e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[2]=abs(v)+e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
    }
    command(e_ptr);
}

void MainWindow::eulerx_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    v*=toRadians;
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[3]=-abs(v)+e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[3]=-abs(v)+e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
    }
    command(e_ptr);
}

void MainWindow::eulerx_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    v*=toRadians;
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[3]=abs(v)+e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[3]=abs(v)+e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
    }
    command(e_ptr);
}

void MainWindow::eulery_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    v*=toRadians;
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[4]=-abs(v)+e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[4]=-abs(v)+e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
    }
    command(e_ptr);
}

void MainWindow::eulery_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    v*=toRadians;
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[4]=abs(v)+e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    } else {
        e_ptr[i].curpos_cart_euler[4]=abs(v)+e_ptr[i].curpos_cart_euler[4];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
    }
    command(e_ptr);
}

void MainWindow::eulerz_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    v*=toRadians;
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[5]=-abs(v)+e_ptr[i].curpos_cart_euler[5];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
    } else {
        e_ptr[i].curpos_cart_euler[5]=-abs(v)+e_ptr[i].curpos_cart_euler[5];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    }
    command(e_ptr);

}

void MainWindow::eulerz_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    v*=toRadians;
    e_ptr[i].kinmode=KINMODE::ik_rapid;
    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[5]=abs(v)+e_ptr[i].curpos_cart_euler[5];
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curpos_cart_euler[0];
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curpos_cart_euler[1];
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curpos_cart_euler[2];
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curpos_cart_euler[3];
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curpos_cart_euler[4];
    } else {
        e_ptr[i].curpos_cart_euler[5]=abs(v)+e_ptr[i].curpos_cart_euler[5];
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curpos_cart_euler[5];
    }
    command(e_ptr);

}

void MainWindow::toolx_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::fk_rapid_tooldir;

    e_ptr[i].tooldir_cart.x=-abs(v);
    e_ptr[i].tooldir_cart.y=0;
    e_ptr[i].tooldir_cart.z=0;
    command(e_ptr);
}

void MainWindow::toolx_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::fk_rapid_tooldir;

    e_ptr[i].tooldir_cart.x=abs(v);
    e_ptr[i].tooldir_cart.y=0;
    e_ptr[i].tooldir_cart.z=0;
    command(e_ptr);
}

void MainWindow::tooly_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::fk_rapid_tooldir;

    e_ptr[i].tooldir_cart.y=-abs(v);
    e_ptr[i].tooldir_cart.x=0;
    e_ptr[i].tooldir_cart.z=0;
    command(e_ptr);

}

void MainWindow::tooly_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::fk_rapid_tooldir;

    e_ptr[i].tooldir_cart.y=abs(v);
    e_ptr[i].tooldir_cart.x=0;
    e_ptr[i].tooldir_cart.z=0;
    command(e_ptr);
}

void MainWindow::toolz_min_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::fk_rapid_tooldir;

    e_ptr[i].tooldir_cart.z=-abs(v);
    e_ptr[i].tooldir_cart.x=0;
    e_ptr[i].tooldir_cart.y=0;
    command(e_ptr);
}

void MainWindow::toolz_plus_start(){

    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::fk_rapid_tooldir;

    e_ptr[i].tooldir_cart.z=abs(v);
    e_ptr[i].tooldir_cart.x=0;
    e_ptr[i].tooldir_cart.y=0;
    command(e_ptr);
}

void MainWindow::tool_eulerx_min_start(){
    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid_tooldir;

    e_ptr[i].tooldir_euler.x=-abs(v*toRadians);
    e_ptr[i].tooldir_euler.y=0;
    e_ptr[i].tooldir_euler.z=0;
    command(e_ptr);
}

void MainWindow::tool_eulerx_plus_start(){
    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid_tooldir;

    e_ptr[i].tooldir_euler.x=abs(v*toRadians);
    e_ptr[i].tooldir_euler.y=0;
    e_ptr[i].tooldir_euler.z=0;
    command(e_ptr);
}

void MainWindow::tool_eulery_min_start(){
    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid_tooldir;

    e_ptr[i].tooldir_euler.y=-abs(v*toRadians);
    e_ptr[i].tooldir_euler.x=0;
    e_ptr[i].tooldir_euler.z=0;
    command(e_ptr);
}

void MainWindow::tool_eulery_plus_start(){
    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid_tooldir;

    e_ptr[i].tooldir_euler.y=abs(v*toRadians);
    e_ptr[i].tooldir_euler.x=0;
    e_ptr[i].tooldir_euler.z=0;
    command(e_ptr);
}

void MainWindow::tool_eulerz_min_start(){
    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid_tooldir;

    e_ptr[i].tooldir_euler.z=-abs(v*toRadians);
    e_ptr[i].tooldir_euler.x=0;
    e_ptr[i].tooldir_euler.y=0;
    command(e_ptr);
}

void MainWindow::tool_eulerz_plus_start(){
    int i=ui->spinBox_machine->value();
    double v=ui->spinBox_stepsize->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid_tooldir;

    e_ptr[i].tooldir_euler.z=abs(v*toRadians);
    e_ptr[i].tooldir_euler.x=0;
    e_ptr[i].tooldir_euler.y=0;
    command(e_ptr);
}

int MainWindow::load_stepfiles(){

    int machines=e_ptr[0].data.machines;
    int *joints=e_ptr[0].data.joints;
    char *stepfilepath=e_ptr[0].data.stepfilepath;

    //! Initialize the ammount of joints to show in terminal output.
    int a=0;
    for(int i=0; i<machines; i++){
        for(int j=0; j<joints[i]; j++){
            a++;
        }
    }

    //! Loading the stepfiles into machine assembly's
    std::cout<<"loading stepfiles for "<<machines<<" machines"<<std::endl;
    std::cout<<"ammount of joints "<<a<<std::endl;
    int k=0;
    for(int i=0; i<machines; i++){
        for(int j=0; j<joints[i]+1; j++){ //! The first drawing is the machine environment and or base.

            std::string s=stepfilepath;
            s.append(std::to_string(i));        // 0
            s.append("/");                      // 0/
            s.append(std::to_string(j));        // 0/0
            s.append(".step");                  // 0/0.step

            std::cout<<"loading stepfile: "<<s<<std::endl;
            //! end of assembly
            if(j==joints[i]){
                OpencascadeWidget->Readstepfile(s, 1);
            } else {
                OpencascadeWidget->Readstepfile(s, 0);
            }

            k++;
        }
    }
    return 1;
}

void MainWindow::update_opencascade(){

    int machines=e_ptr[0].data.machines;
    int *joints=e_ptr[0].data.joints;

    //! Load the stepfiles of multiple machines.
    if(!loadstepfiles){
        timer->stop();
        int ok=load_stepfiles();
        if(!ok){
            std::cout<<"stepfiles error"<<std::endl;
        }
        if(ok){
            loadstepfiles=1;
            std::cout<<"stepfiles completed"<<std::endl;
            timer->start(200);
        }
    }

    //! Update mainwindow items.

    for(int i=0; i<machines; i++){
        if(ui->spinBox_machine->value()==i){

            //! Update absolute machine positions.
            ui->lineEdit_cartx->setText(QString::number(e_ptr[i].curpos_cart_euler[0],'f',3));
            ui->lineEdit_carty->setText(QString::number(e_ptr[i].curpos_cart_euler[1],'f',3));
            ui->lineEdit_cartz->setText(QString::number(e_ptr[i].curpos_cart_euler[2],'f',3));
            ui->lineEdit_cartx_tarpos->setText(QString::number(e_ptr[i].tarpos_cart_euler[0],'f',3));
            ui->lineEdit_carty_tarpos->setText(QString::number(e_ptr[i].tarpos_cart_euler[1],'f',3));
            ui->lineEdit_cartz_tarpos->setText(QString::number(e_ptr[i].tarpos_cart_euler[2],'f',3));
            ui->lineEdit_eulerx->setText(QString::number(e_ptr[i].curpos_cart_euler[3]*toDegrees,'f',3));
            ui->lineEdit_eulery->setText(QString::number(e_ptr[i].curpos_cart_euler[4]*toDegrees,'f',3));
            ui->lineEdit_eulerz->setText(QString::number(e_ptr[i].curpos_cart_euler[5]*toDegrees,'f',3));
            ui->lineEdit_eulerx_tarpos->setText(QString::number(e_ptr[i].tarpos_cart_euler[3]*toDegrees,'f',3));
            ui->lineEdit_eulery_tarpos->setText(QString::number(e_ptr[i].tarpos_cart_euler[4]*toDegrees,'f',3));
            ui->lineEdit_eulerz_tarpos->setText(QString::number(e_ptr[i].tarpos_cart_euler[5]*toDegrees,'f',3));

            //! Update relative machine positions.
            ui->lineEdit_tarpox_relative_cartx->setText(QString::number(e_ptr[i].curpos_cart_euler_rel[0],'f',3));
            ui->lineEdit_tarpox_relative_carty->setText(QString::number(e_ptr[i].curpos_cart_euler_rel[1],'f',3));
            ui->lineEdit_tarpox_relative_cartz->setText(QString::number(e_ptr[i].curpos_cart_euler_rel[2],'f',3));
            ui->lineEdit_tarpox_relative_eulerx->setText(QString::number(e_ptr[i].curpos_cart_euler_rel[3]*toDegrees,'f',3));
            ui->lineEdit_tarpox_relative_eulery->setText(QString::number(e_ptr[i].curpos_cart_euler_rel[4]*toDegrees,'f',3));
            ui->lineEdit_tarpox_relative_eulerz->setText(QString::number(e_ptr[i].curpos_cart_euler_rel[5]*toDegrees,'f',3));

            //! Update tool - workframe positions.
            ui->lineEdit_toolx->setText(QString::number(e_ptr[i].curpos_tool[0],'f',3));
            ui->lineEdit_tooly->setText(QString::number(e_ptr[i].curpos_tool[1],'f',3));
            ui->lineEdit_toolz->setText(QString::number(e_ptr[i].curpos_tool[2],'f',3));


            for(int j=0; j<joints[i]; j++){
                if(ui->spinBox_joint->value()==j){
                    if(e_ptr[i].joint[j].rotationtype==0 || e_ptr[i].joint[j].rotationtype==1 || e_ptr[i].joint[j].rotationtype==2 || e_ptr[i].joint[j].rotationtype==6){
                        ui->lineEdit_joint->setText(QString::number(e_ptr[i].joint[j].curpos*toDegrees,'f',3));
                        ui->lineEdit_joint_tarpos->setText(QString::number(e_ptr[i].joint[j].tarpos*toDegrees,'f',3));
                    } else {
                        ui->lineEdit_joint->setText(QString::number(e_ptr[i].joint[j].curpos,'f',3));
                        ui->lineEdit_joint_tarpos->setText(QString::number(e_ptr[i].joint[j].tarpos,'f',3));
                    }

                }
            }
        }
    }

    OpencascadeWidget->Move_assembly_joints(e_ptr);

    //! Check for machine limits.
    for(int i=0; i<machines; i++){

        if(e_ptr[i].error){
            ui->label_ik_error->setText("Id:"+QString::number(i)+" ik error");
            ui->label_ik_error->setStyleSheet("background-color: rgb(255, 0, 0);"); //red
        } else {
            ui->label_ik_error->setText("");
            ui->label_ik_error->setStyleSheet("background-color: rgb(85, 255, 0);"); //red

        }

        for(int j=0; j<joints[i]; j++){
            //! Check for joint limits. Unit::bool
            if(e_ptr[i].joint[j].jointlimit==1){
                ui->label_jointlimit->setText("Id:"+QString::number(i)+"Joint:"+QString::number(j));
                ui->label_jointlimit->setStyleSheet("background-color: rgb(255, 0, 0);"); //red

                break;
            } else {
                ui->label_jointlimit->setText("");
                ui->label_jointlimit->setStyleSheet("background-color: rgb(85, 255, 0);"); //green
            }
        }
    }

    //! Highlight current executed gcodeblock
    ui->listWidget_gcode->setCurrentRow(e_ptr[0].exec_gcodeblock);
}

void MainWindow::set_mdi_pos()
{
    int i=ui->spinBox_machine->value();
    e_ptr[i].kinmode=KINMODE::ik_rapid;

    double cartx=ui->lineEdit_mdi_cartx->text().toDouble();
    double carty=ui->lineEdit_mdi_carty->text().toDouble();
    double cartz=ui->lineEdit_mdi_cartz->text().toDouble();
    double eulerx=ui->lineEdit_mdi_eulerx->text().toDouble();
    double eulery=ui->lineEdit_mdi_eulery->text().toDouble();
    double eulerz=ui->lineEdit_mdi_eulerz->text().toDouble();

    if(e_ptr[i].rt_motion){
        e_ptr[i].tarpos_cart_euler[0]=e_ptr[i].curoffset_cart_euler_rel[0]+cartx;
        e_ptr[i].tarpos_cart_euler[1]=e_ptr[i].curoffset_cart_euler_rel[1]+carty;
        e_ptr[i].tarpos_cart_euler[2]=e_ptr[i].curoffset_cart_euler_rel[2]+cartz;
        e_ptr[i].tarpos_cart_euler[3]=e_ptr[i].curoffset_cart_euler_rel[3]+eulerx*toRadians;
        e_ptr[i].tarpos_cart_euler[4]=e_ptr[i].curoffset_cart_euler_rel[4]+eulery*toRadians;
        e_ptr[i].tarpos_cart_euler[5]=e_ptr[i].curoffset_cart_euler_rel[5]+eulerz*toRadians;
    } else {
        e_ptr[i].curpos_cart_euler[0]=e_ptr[i].curoffset_cart_euler_rel[0]+cartx;
        e_ptr[i].curpos_cart_euler[1]=e_ptr[i].curoffset_cart_euler_rel[1]+carty;
        e_ptr[i].curpos_cart_euler[2]=e_ptr[i].curoffset_cart_euler_rel[2]+cartz;
        e_ptr[i].curpos_cart_euler[3]=e_ptr[i].curoffset_cart_euler_rel[3]+(eulerx*toRadians);
        e_ptr[i].curpos_cart_euler[4]=e_ptr[i].curoffset_cart_euler_rel[4]+(eulery*toRadians);
        e_ptr[i].curpos_cart_euler[5]=e_ptr[i].curoffset_cart_euler_rel[5]+(eulerz*toRadians);
    }
    command(e_ptr);
}




















