#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QLineEdit"
#include "QLabel"
#include "QVector"
#include "QPalette"

std::vector<GCODE> gcodevec;

std::vector<Handle(AIS_Shape)> originvec, linevec, gcodetcpvec;
std::vector<gp_Trsf> TrsfVec={}, TrsfVecMachineOffset={};
bool startup=0, ptr_ready=0;
struct DATA *ptr;
extern "C" void btn_event_from_cpp_to_c(struct DATA *p);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    OpencascadeWidget = new Opencascade(this);
    ui->gridLayout_opencascade->addWidget(OpencascadeWidget);

    connect(timer, &QTimer::timeout, this, &MainWindow::mainloop);
    //! 20ms is too fast, startup problems.
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//! Recieving a 1ms *ptr update. This class don't cares about fast incoming traffic so far.
extern "C" void c_to_cpp_ptr(struct DATA *p){
    ptr=p;
}

void MainWindow::view_orthographic(){
    OpencascadeWidget->Set_orthographic();
}

void MainWindow::view_perspective(){
    OpencascadeWidget->Set_perspective();
}

void MainWindow::transparancy(double value){
    for(unsigned int i=0; i<ptr->machines; i++){
        //! Opencascade input value is 0.0-1.0
        ptr->machine[i].transparancy=value;
    }
}

void MainWindow::init(){

    //! Create dynamic gui controls.
    //! The *ptr recieved a oneshot data when gui was starteed by halmodule.c
    //!
    //! Stepfile's loading can be done by c++
    //! std::vector<std::string> strvec=read_textfile().read("/opt/hal-core/src/hal/components/matrix/hal/config.txt");

    //! Gcode graphics data container. Every machine has 2.
    //! gcodevec.at(0)=mdi.
    //! gcodevec.at(1)=program.
    //! gcodevec.at(2)=a move from current pos to gcode startpos.
    gcodevec.resize(ptr->machines*3);

    QStackedWidget *stackedwidget = new QStackedWidget();

    for(unsigned int i=0; i<ptr->machines; i++){

        GUICONTROLS r;

        Control *cnt = new Control();
        r.control=cnt;
        //! Machine identification.
        r.control->machine_id=i;
        //! Machine gui init values from hal.
        r.control->spb_maxvel_cart->setValue(ptr->machine[i].maxvel_cart);
        r.control->spb_maxvel_euler->setValue(ptr->machine[i].maxvel_euler);
        r.control->dspb_maxacc_cart->setValue(ptr->machine[i].maxacc_cart);
        r.control->dspb_maxacc_euler->setValue(ptr->machine[i].maxacc_euler);
        ptr->machine[i].sc.feed_overide=r.control->dspb_feed_overide->value();

        QGridLayout *gridlayout1 = new QGridLayout();
        QWidget *aPage1 = new QWidget;
        aPage1->setLayout(gridlayout1);
        gridlayout1->addWidget(cnt);
        ui->stackedWidget->addWidget(aPage1);

        //! New code control.ui
        ui->comboBox->addItem("Machine:"+QString::number(i));
        connect(ui->comboBox, QOverload<int>::of(&QComboBox::activated),ui->stackedWidget, &QStackedWidget::setCurrentIndex);

        //! Save values for this machine[i].
        guicontrolvec.push_back(r);
    }

    QComboBox *pageComboBox = new QComboBox;
    for(unsigned int i=0; i<ptr->machines; i++){
        pageComboBox->addItem(QString("Machine ")+QString::number(i));
    }
    connect(pageComboBox, QOverload<int>::of(&QComboBox::activated),
            stackedwidget, &QStackedWidget::setCurrentIndex);

    //! Load machine stepfiles.
    for(unsigned int i=0; i<ptr->machines; i++){
        OpencascadeWidget->Readstepfile(ptr->machine[i].stepfile,0);
        for(unsigned int j=0; j<ptr->machine[i].joints; j++){
            OpencascadeWidget->Readstepfile(ptr->machine[i].joint[j].stepfile,0);
        }
    }

    //! Calculate the amount of origins and lines.
    unsigned int k=0;
    for(unsigned int i=0; i<ptr->machines; i++){
        for(unsigned int j=0; j<ptr->machine[i].joints; j++){
            k++;
        }
    }

    originvec.resize(k);
    linevec.resize(k);
    TrsfVec.resize(k);
    TrsfVecMachineOffset.resize(ptr->machines);
    gcodetcpvec.resize(ptr->machines);

    //! Draw origins.
    for(unsigned int i=0; i<originvec.size(); i++){
        originvec.at(i)=draw_primitives().draw_3d_origin({0,0,0},50);
        Handle(AIS_Shape) sphere= draw_primitives().draw_3d_sphere(1,{0,0,0});
        originvec.at(i)->AddChild(sphere);
        OpencascadeWidget->display_shape(originvec.at(i));
    }

    //! Draw structure lines.
    for(unsigned int i=0; i<linevec.size(); i++){
        linevec.at(i)=draw_primitives().draw_3d_line({0,0,0},{100,0,0});
        linevec.at(i)=draw_primitives().colorize(linevec.at(i),Quantity_NOC_PURPLE,0);
        OpencascadeWidget->display_shape(linevec.at(i));
    }

    //! Draw gcode tcp solid.
    for(unsigned int i=0; i<ptr->machines; i++){
        gcodetcpvec.at(i)=draw_primitives().draw_3d_origin({0,0,0},20);
        Handle(AIS_Shape) solid= draw_primitives().draw_3d_cone({0,0,0},0,5,20);
        solid= draw_primitives().rotate_translate_3d_quaternion(solid,{0,0,0},0,-90*toRadians,0);
        solid= draw_primitives().transparancy(solid,0.5);
        solid=draw_primitives().colorize(solid,Quantity_NOC_GREEN,0);
        gcodetcpvec.at(i)->AddChild(solid);
        OpencascadeWidget->display_shape(gcodetcpvec.at(i));
    }
}

//! Function called by update_opencascade thread.
void MainWindow::set_label_values(){
    for(unsigned int i=0; i<ptr->machines; i++){

        if(ptr->machine[i].kinmode==KINMODE::forward_kinematic){
            guicontrolvec[i].control->lbl_kinmode->setText("Mode fk");
        }
        if(ptr->machine[i].kinmode==KINMODE::inverse_kinematic){
            guicontrolvec[i].control->lbl_kinmode->setText("Mode ik");
        }
    }

    //! Label servo-load.
    ui->label_servo_load->setText(QString("servo thread load ")+QString::number((hal_interface().print_servo_thread_time()/ptr->servoperiod)*100,'f',3)+"%" +
                                  QString(" base thread load ")+QString::number((hal_interface().print_base_thread_time()/ptr->baseperiod)*100,'f',3)+"%" );
}

//! Function called by update_opencascade thread.
void MainWindow::set_lineedit_values(){
    for(unsigned int i=0; i<ptr->machines; i++){

        guicontrolvec[i].control->le_J0_rel->setText(QString::number(ptr->machine[i].joint[0].curpos_rel,'f',3));
        guicontrolvec[i].control->le_J1_rel->setText(QString::number(ptr->machine[i].joint[1].curpos_rel,'f',3));
        guicontrolvec[i].control->le_J2_rel->setText(QString::number(ptr->machine[i].joint[2].curpos_rel,'f',3));
        guicontrolvec[i].control->le_J3_rel->setText(QString::number(ptr->machine[i].joint[3].curpos_rel,'f',3));
        guicontrolvec[i].control->le_J4_rel->setText(QString::number(ptr->machine[i].joint[4].curpos_rel,'f',3));
        guicontrolvec[i].control->le_J5_rel->setText(QString::number(ptr->machine[i].joint[5].curpos_rel,'f',3));

        guicontrolvec[i].control->le_cart_x_rel->setText(QString::number(ptr->machine[i].rel_cart.x,'f',3));
        guicontrolvec[i].control->le_cart_y_rel->setText(QString::number(ptr->machine[i].rel_cart.y,'f',3));
        guicontrolvec[i].control->le_cart_z_rel->setText(QString::number(ptr->machine[i].rel_cart.z,'f',3));
        guicontrolvec[i].control->le_euler_x_rel->setText(QString::number(ptr->machine[i].rel_euler.x*toDegrees,'f',3));
        guicontrolvec[i].control->le_euler_y_rel->setText(QString::number(ptr->machine[i].rel_euler.y*toDegrees,'f',3));
        guicontrolvec[i].control->le_euler_z_rel->setText(QString::number(ptr->machine[i].rel_euler.z*toDegrees,'f',3));

        guicontrolvec[i].control->le_cart_x_abs->setText(QString::number(ptr->machine[i].abs_cart.x,'f',3));
        guicontrolvec[i].control->le_cart_y_abs->setText(QString::number(ptr->machine[i].abs_cart.y,'f',3));
        guicontrolvec[i].control->le_cart_z_abs->setText(QString::number(ptr->machine[i].abs_cart.z,'f',3));
        guicontrolvec[i].control->le_euler_x_abs->setText(QString::number(ptr->machine[i].abs_euler.x*toDegrees,'f',3));
        guicontrolvec[i].control->le_euler_y_abs->setText(QString::number(ptr->machine[i].abs_euler.y*toDegrees,'f',3));
        guicontrolvec[i].control->le_euler_z_abs->setText(QString::number(ptr->machine[i].abs_euler.z*toDegrees,'f',3));

        //! # rotationtype : rot_x = 0, rot_y = 1, rot_z = 2, trans_x = 3, trans_y = 4, trans_z = 5, rot_axis = 6, trans_axis =7, none = 8
        //!
        //! If you have a machine with more joints, add them here and in the libguicontrol.
        //! This process can be automated, but then the libguicontrol gui design is less transparant for new users. This is a decision to make.

        int j=0; //! j=joint.
        if(ptr->machine[i].joint[j].rotationtype==0 || ptr->machine[i].joint[j].rotationtype==1 || ptr->machine[i].joint[j].rotationtype==2 || ptr->machine[i].joint[j].rotationtype==6){
            guicontrolvec[i].control->le_J0_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs*toDegrees,'f',3));
        } else {
            guicontrolvec[i].control->le_J0_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs,'f',3));
        }

        j=1;
        if(ptr->machine[i].joint[j].rotationtype==0 || ptr->machine[i].joint[j].rotationtype==1 || ptr->machine[i].joint[j].rotationtype==2 || ptr->machine[i].joint[j].rotationtype==6){
            guicontrolvec[i].control->le_J1_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs*toDegrees,'f',3));
        } else {
            guicontrolvec[i].control->le_J1_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs,'f',3));
        }

        j=2;
        if(ptr->machine[i].joint[j].rotationtype==0 || ptr->machine[i].joint[j].rotationtype==1 || ptr->machine[i].joint[j].rotationtype==2 || ptr->machine[i].joint[j].rotationtype==6){
            guicontrolvec[i].control->le_J2_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs*toDegrees,'f',3));
        } else {
            guicontrolvec[i].control->le_J2_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs,'f',3));
        }

        j=3;
        if(ptr->machine[i].joint[j].rotationtype==0 || ptr->machine[i].joint[j].rotationtype==1 || ptr->machine[i].joint[j].rotationtype==2 || ptr->machine[i].joint[j].rotationtype==6){
            guicontrolvec[i].control->le_J3_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs*toDegrees,'f',3));
        } else {
            guicontrolvec[i].control->le_J3_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs,'f',3));
        }

        j=4;
        if(ptr->machine[i].joint[j].rotationtype==0 || ptr->machine[i].joint[j].rotationtype==1 || ptr->machine[i].joint[j].rotationtype==2 || ptr->machine[i].joint[j].rotationtype==6){
            guicontrolvec[i].control->le_J4_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs*toDegrees,'f',3));
        } else {
            guicontrolvec[i].control->le_J4_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs,'f',3));
        }

        j=5;
        if(ptr->machine[i].joint[j].rotationtype==0 || ptr->machine[i].joint[j].rotationtype==1 || ptr->machine[i].joint[j].rotationtype==2 || ptr->machine[i].joint[j].rotationtype==6){
            guicontrolvec[i].control->le_J5_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs*toDegrees,'f',3));
        } else {
            guicontrolvec[i].control->le_J5_abs->setText(QString::number(ptr->machine[i].joint[j].curpos_abs,'f',3));
        }
    }
}

void MainWindow::mainloop(){

    if(!startup){
        init();
        //! Fill up the joint matrix arrays with kdl.
        *ptr=kinematic_interface().fk(ptr);
        startup=1;
    }

    //! Update the opencascade matrixes.
    unsigned int k=0;
    for(unsigned int i=0; i<ptr->machines; i++){

        //! Machine offsets:
        gp_Trsf trsf;
        gp_Quaternion aQuat;
        aQuat.SetEulerAngles (gp_YawPitchRoll, ptr->machine[i].offset_euler.z, ptr->machine[i].offset_euler.y, ptr->machine[i].offset_euler.x);
        trsf.SetRotation(aQuat);
        TrsfVecMachineOffset.at(i).SetTranslation({0,0,0},{ptr->machine[i].offset_cart.x,ptr->machine[i].offset_cart.y,ptr->machine[i].offset_cart.z});
        TrsfVecMachineOffset.at(i).Multiply(trsf);

        //! Joints, morph kdl matrixes into opencascade matrixes.
        for(unsigned int j=0; j<ptr->machine[i].joints; j++){

            //! Copy the *ptr joint matrix for each joint to the opencascade matrix.
            TrsfVec.at(k).SetValues(
                        ptr->machine[i].joint[j].matrix[0][0],
                    ptr->machine[i].joint[j].matrix[0][1],
                    ptr->machine[i].joint[j].matrix[0][2],
                    ptr->machine[i].joint[j].matrix[0][3],
                    ptr->machine[i].joint[j].matrix[1][0],
                    ptr->machine[i].joint[j].matrix[1][1],
                    ptr->machine[i].joint[j].matrix[1][2],
                    ptr->machine[i].joint[j].matrix[1][3],
                    ptr->machine[i].joint[j].matrix[2][0],
                    ptr->machine[i].joint[j].matrix[2][1],
                    ptr->machine[i].joint[j].matrix[2][2],
                    ptr->machine[i].joint[j].matrix[2][3] );
            k++;
        }
    }
    OpencascadeWidget->Move_assembly_joints(ptr, TrsfVec, TrsfVecMachineOffset);

    //! Update opencascade origins
    k=0;
    for(unsigned int i=0; i<ptr->machines; i++){
        for(unsigned int j=0; j<ptr->machine[i].joints; j++){
            //! First originvec we keep untouched to display the 0.0.0 origin.
            gp_Trsf result;
            result.Multiply(TrsfVecMachineOffset.at(i));
            result.Multiply(TrsfVec.at(k));
            originvec.at(k)->SetLocalTransformation(result);
            OpencascadeWidget->redisplay_shape(originvec.at(k));
            k++;
        }
    }

    //! Update opencascade machine kinematic structure lines
    k=0;
    for(unsigned int i=0; i<ptr->machines; i++){
        double x=0,y=0,z=0;
        for(unsigned int j=0; j<ptr->machine[i].joints; j++){

            //! Avoid a edge of lenght 0, wich will give a error.
            double l=sqrt(pow(TrsfVec.at(k).TranslationPart().X()-x,2)+pow(TrsfVec.at(k).TranslationPart().Y()-y,2)+pow(TrsfVec.at(k).TranslationPart().Z()-z,2));
            if(l>0){
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge({x,
                                                            y,
                                                            z},
                                                           {TrsfVec.at(k).TranslationPart().X(),
                                                            TrsfVec.at(k).TranslationPart().Y(),
                                                            TrsfVec.at(k).TranslationPart().Z()});
                linevec.at(k).get()->SetShape(edge);
                linevec.at(k)->SetLocalTransformation(TrsfVecMachineOffset.at(i));
                OpencascadeWidget->redisplay_shape(linevec.at(k));
            }
            x=TrsfVec.at(k).TranslationPart().X();
            y=TrsfVec.at(k).TranslationPart().Y();
            z=TrsfVec.at(k).TranslationPart().Z();
            k++;
        }
    }

    //! Update gcode tcp interpolated.
    for(unsigned int i=0; i<ptr->machines; i++){
        //! First originvec we keep untouched to display the 0.0.0 origin.
        gp_Trsf result;
        result.Multiply(TrsfVecMachineOffset.at(i));
        gp_Trsf MyTrsf;
        double x=ptr->machine[i].abs_cart.x;
        double y=ptr->machine[i].abs_cart.y;
        double z=ptr->machine[i].abs_cart.z;
        MyTrsf.SetTranslation({0,0,0},{x,y,z});
        result.Multiply(MyTrsf);
        gcodetcpvec.at(i)->SetLocalTransformation(result);
        OpencascadeWidget->redisplay_shape(gcodetcpvec.at(i));
    }

    set_lineedit_values();
    set_label_values();
    OpencascadeWidget->Redraw();

    //! Sequence the MODE.
    //! This is a feedback routine (status checker) from the halmodule.c to colorize the actual buttons state.
    for(unsigned int i=0; i<ptr->machines; i++){

        //! Not in startpos, give the gui a message.
        if(ptr->machine[i].sc.message_not_in_startpos){
            guicontrolvec.at(i).control->lbl_message->setText(ptr->machine[i].sc.message);
            //! Reset flag.
            ptr->machine[i].sc.message_not_in_startpos=0;
        }

        //! Maximum gcode line value for restart from line.
        if(guicontrolvec.at(i).control->run_from_line->text().toUInt()>=gcodevec.at(i+1).blockvec.size() && gcodevec.at(i+1).blockvec.size()>0){
            guicontrolvec.at(i).control->run_from_line->setText(QString::number(gcodevec.at(i+1).blockvec.size()-1));
            std::cout<<"maxvalue set to max gcode line nr."<<std::endl;
        }

        if(guicontrolvec.at(i).control->pb_reload_gcode->isDown()==1){
            ptr->machine[i].sc.run_from_line=guicontrolvec.at(i).control->run_from_line->text().toUInt();
            //! Reset program parameters.
            ptr->machine[i].sc.gcode_reset=1;
            //! Enable program run.
            ptr->machine[i].sc.gcode_ready_to_run=1;
            std::cout<<"program reload"<<std::endl;
        }

        if(guicontrolvec.at(i).control->gcode_ready_to_close){
            OpencascadeWidget->delete_gcode(i+1);
            //! Reset flag.
            guicontrolvec.at(i).control->gcode_ready_to_close=0;
        }

        //! If libguicontrol has a gcode to display, here it read's the flag if it's ready to draw.
        if(guicontrolvec.at(i).control->gcode_ready_to_load){
            OpencascadeWidget->draw_gcode(i,ptr,i+1);
            ptr->machine[i].sc.run_from_line=guicontrolvec.at(i).control->run_from_line->text().toUInt();
            ptr->machine[i].sc.gcode_reset=1;
            //! Reset flag.
            guicontrolvec.at(i).control->gcode_ready_to_load=0;
            ptr->machine[i].sc.gcode_ready_to_run=1;
        }

        if(guicontrolvec.at(i).control->mdi_ready_to_load){
            //! Draw mdi graphics.
            OpencascadeWidget->draw_gcode(i,ptr,i);
            //! Reset flag.
            guicontrolvec.at(i).control->mdi_ready_to_load=0;
            ptr->machine[i].sc.mdi_ready_to_run=1;
        }

        if(ptr->machine[i].sc.mdi_remove_graphics){
            OpencascadeWidget->delete_gcode(i);
            //! Reset flag.
            ptr->machine[i].sc.mdi_remove_graphics=0;
        }

        //! Highlight current exec. gcode line.
        if(ptr->machine[i].sc.flag_highlight_new_gcodeline==1){
            guicontrolvec[i].control->highlight_gcode_line(ptr->machine[i].sc.cur_gcodeline);
            //! Reset flag.
            ptr->machine[i].sc.flag_highlight_new_gcodeline=0;
        }

        //! Button states.
        ptr->machine[i].stepsize=guicontrolvec.at(i).control->dspb_stepsize->value();
        ptr->machine[i].joint[0].joint_min=guicontrolvec.at(i).control->pb_j0_min->isDown();
        ptr->machine[i].joint[0].joint_plus=guicontrolvec.at(i).control->pb_j0_plus->isDown();
        ptr->machine[i].joint[1].joint_min=guicontrolvec.at(i).control->pb_j1_min->isDown();
        ptr->machine[i].joint[1].joint_plus=guicontrolvec.at(i).control->pb_j1_plus->isDown();
        ptr->machine[i].joint[2].joint_min=guicontrolvec.at(i).control->pb_j2_min->isDown();
        ptr->machine[i].joint[2].joint_plus=guicontrolvec.at(i).control->pb_j2_plus->isDown();
        ptr->machine[i].joint[3].joint_min=guicontrolvec.at(i).control->pb_j3_min->isDown();
        ptr->machine[i].joint[3].joint_plus=guicontrolvec.at(i).control->pb_j3_plus->isDown();
        ptr->machine[i].joint[4].joint_min=guicontrolvec.at(i).control->pb_j4_min->isDown();
        ptr->machine[i].joint[4].joint_plus=guicontrolvec.at(i).control->pb_j4_plus->isDown();
        ptr->machine[i].joint[5].joint_min=guicontrolvec.at(i).control->pb_j5_min->isDown();
        ptr->machine[i].joint[5].joint_plus=guicontrolvec.at(i).control->pb_j5_plus->isDown();

        ptr->machine[i].btn_cartx_plus=guicontrolvec.at(i).control->pb_cart_x_plus->isDown();
        ptr->machine[i].btn_carty_plus=guicontrolvec.at(i).control->pb_cart_y_plus->isDown();
        ptr->machine[i].btn_cartz_plus=guicontrolvec.at(i).control->pb_cart_z_plus->isDown();
        ptr->machine[i].btn_cartx_min=guicontrolvec.at(i).control->pb_cart_x_min->isDown();
        ptr->machine[i].btn_carty_min=guicontrolvec.at(i).control->pb_cart_y_min->isDown();
        ptr->machine[i].btn_cartz_min=guicontrolvec.at(i).control->pb_cart_z_min->isDown();

        ptr->machine[i].btn_eulerx_plus=guicontrolvec.at(i).control->pb_euler_x_plus->isDown();
        ptr->machine[i].btn_eulery_plus=guicontrolvec.at(i).control->pb_euler_y_plus->isDown();
        ptr->machine[i].btn_eulerz_plus=guicontrolvec.at(i).control->pb_euler_z_plus->isDown();
        ptr->machine[i].btn_eulerx_min=guicontrolvec.at(i).control->pb_euler_x_min->isDown();
        ptr->machine[i].btn_eulery_min=guicontrolvec.at(i).control->pb_euler_y_min->isDown();
        ptr->machine[i].btn_eulerz_min=guicontrolvec.at(i).control->pb_euler_z_min->isDown();

        ptr->machine[i].btn_toolx_min=guicontrolvec.at(i).control->pb_tool_x_min->isDown();
        ptr->machine[i].btn_tooly_min=guicontrolvec.at(i).control->pb_tool_y_min->isDown();
        ptr->machine[i].btn_toolz_min=guicontrolvec.at(i).control->pb_tool_z_min->isDown();
        ptr->machine[i].btn_toolx_plus=guicontrolvec.at(i).control->pb_tool_x_plus->isDown();
        ptr->machine[i].btn_tooly_plus=guicontrolvec.at(i).control->pb_tool_y_plus->isDown();
        ptr->machine[i].btn_toolz_plus=guicontrolvec.at(i).control->pb_tool_z_plus->isDown();

        ptr->machine[i].btn_zero_rel_all=guicontrolvec.at(i).control->pb_zero_rel_all->isDown();
        ptr->machine[i].btn_zero_rel_cartx=guicontrolvec.at(i).control->pb_zero_rel_cartx->isDown();
        ptr->machine[i].btn_zero_rel_carty=guicontrolvec.at(i).control->pb_zero_rel_carty->isDown();
        ptr->machine[i].btn_zero_rel_cartz=guicontrolvec.at(i).control->pb_zero_rel_cartz->isDown();
        ptr->machine[i].btn_zero_rel_eulerx=guicontrolvec.at(i).control->pb_zero_rel_eulerx->isDown();
        ptr->machine[i].btn_zero_rel_eulery=guicontrolvec.at(i).control->pb_zero_rel_eulery->isDown();
        ptr->machine[i].btn_zero_rel_eulerz=guicontrolvec.at(i).control->pb_zero_rel_eulerz->isDown();
        ptr->machine[i].btn_zero_rel_J0=guicontrolvec.at(i).control->pb_zero_rel_J0->isDown();
        ptr->machine[i].btn_zero_rel_J1=guicontrolvec.at(i).control->pb_zero_rel_J1->isDown();
        ptr->machine[i].btn_zero_rel_J2=guicontrolvec.at(i).control->pb_zero_rel_J2->isDown();
        ptr->machine[i].btn_zero_rel_J3=guicontrolvec.at(i).control->pb_zero_rel_J3->isDown();
        ptr->machine[i].btn_zero_rel_J4=guicontrolvec.at(i).control->pb_zero_rel_J4->isDown();
        ptr->machine[i].btn_zero_rel_J5=guicontrolvec.at(i).control->pb_zero_rel_J5->isDown();

        ptr->machine[i].btn_mdi_exec=guicontrolvec.at(i).control->pb_mdi_exec->isDown();
        ptr->machine[i].btn_mode_manual=guicontrolvec.at(i).control->pb_manual->isDown();
        ptr->machine[i].btn_mode_auto=guicontrolvec.at(i).control->pb_auto->isDown();
        ptr->machine[i].btn_mode_mdi=guicontrolvec.at(i).control->pb_mdi->isDown();

        ptr->machine[i].btn_run=guicontrolvec.at(i).control->pb_run->isDown();
        ptr->machine[i].btn_stop=guicontrolvec.at(i).control->pb_stop->isDown();
        ptr->machine[i].btn_pause_resume=guicontrolvec.at(i).control->pb_pause->isDown();
        ptr->machine[i].btn_estop=guicontrolvec.at(i).control->tb_estop->isDown();
        ptr->machine[i].btn_offline=guicontrolvec.at(i).control->pb_offline->isDown();
        ptr->machine[i].btn_online=guicontrolvec.at(i).control->pb_online->isDown();

        ptr->machine[i].stepsize=guicontrolvec.at(i).control->dspb_stepsize->value();
        ptr->machine[i].maxvel_cart=guicontrolvec.at(i).control->spb_maxvel_cart->value();
        ptr->machine[i].maxvel_euler=guicontrolvec.at(i).control->spb_maxvel_euler->value();
        ptr->machine[i].maxacc_cart=guicontrolvec.at(i).control->dspb_maxacc_cart->value();
        ptr->machine[i].maxacc_euler=guicontrolvec.at(i).control->dspb_maxacc_euler->value();

        //! Machine state hal feedback.
        if(ptr->machine[i].state_machine==MACHINESTATE::AUTO){
            guicontrolvec[i].control->pb_manual->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_auto->setStyleSheet(stylesheet_green);
            guicontrolvec[i].control->tb_estop->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_mdi->setStyleSheet(stylesheet_grey);
        }
        if(ptr->machine[i].state_machine==MACHINESTATE::MDI){
            guicontrolvec[i].control->pb_manual->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_auto->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_mdi->setStyleSheet(stylesheet_green);
            guicontrolvec[i].control->tb_estop->setStyleSheet(stylesheet_grey);
        }
        if(ptr->machine[i].state_machine==MACHINESTATE::MDI || ptr->machine[i].state_machine==MACHINESTATE::AUTO){
            //! Sub state AUTO, MDI mode.
            if(ptr->machine[i].state_program==PROGRAMSTATE::NONE){
                guicontrolvec[i].control->pb_run->setStyleSheet(stylesheet_grey);
                guicontrolvec[i].control->pb_stop->setStyleSheet(stylesheet_grey);
                guicontrolvec[i].control->pb_pause->setStyleSheet(stylesheet_grey);
            }
            if(ptr->machine[i].state_program==PROGRAMSTATE::RUN){
                guicontrolvec[i].control->pb_run->setStyleSheet(stylesheet_green);
                guicontrolvec[i].control->pb_stop->setStyleSheet(stylesheet_grey);
                guicontrolvec[i].control->pb_pause->setStyleSheet(stylesheet_grey);
            }
            if(ptr->machine[i].state_program==PROGRAMSTATE::STOP){
                guicontrolvec[i].control->pb_run->setStyleSheet(stylesheet_grey);
                guicontrolvec[i].control->pb_stop->setStyleSheet(stylesheet_red);
                guicontrolvec[i].control->pb_pause->setStyleSheet(stylesheet_grey);
            }
            if(ptr->machine[i].state_program==PROGRAMSTATE::PAUSE_RESUME){
                guicontrolvec[i].control->pb_run->setStyleSheet(stylesheet_grey);
                guicontrolvec[i].control->pb_stop->setStyleSheet(stylesheet_grey);
                guicontrolvec[i].control->pb_pause->setStyleSheet(stylesheet_orange);
            }
        }
        if(ptr->machine[i].state_machine==MACHINESTATE::MANUAL){
            guicontrolvec[i].control->pb_manual->setStyleSheet(stylesheet_green);
            guicontrolvec[i].control->pb_auto->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_mdi->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->tb_estop->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_run->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_stop->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_pause->setStyleSheet(stylesheet_grey);
        }
        if(ptr->machine[i].state_machine==MACHINESTATE::ESTOP){
            guicontrolvec[i].control->pb_manual->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_auto->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_mdi->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->tb_estop->setStyleSheet(stylesheet_red);
            guicontrolvec[i].control->pb_run->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_stop->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_pause->setStyleSheet(stylesheet_grey);
        }

        //! Realtime state.
        if(ptr->machine[i].state_realtime==REALTIMESTATE::OFFLINE){
            guicontrolvec[i].control->pb_online->setStyleSheet(stylesheet_grey);
            guicontrolvec[i].control->pb_offline->setStyleSheet(stylesheet_purple);
        }
        if(ptr->machine[i].state_realtime==REALTIMESTATE::ONLINE){
            guicontrolvec[i].control->pb_online->setStyleSheet(stylesheet_purple);
            guicontrolvec[i].control->pb_offline->setStyleSheet(stylesheet_grey);
        }
    }

    //! Update hal.
    btn_event_from_cpp_to_c(ptr);
}


void MainWindow::on_actionView_perspective_triggered()
{
    view_perspective();
}

void MainWindow::on_actionView_orthographic_triggered()
{
    view_orthographic();
}

void MainWindow::on_action0_2_triggered()
{
    transparancy(0);
}

void MainWindow::on_action25_triggered()
{
    transparancy(0.25);
}

void MainWindow::on_action75_triggered()
{
    transparancy(0.75);
}

void MainWindow::on_action100_2_triggered()
{
    transparancy(1);
}

void MainWindow::on_actionHide_View_triggered()
{
    ui->frame_opencascade->setHidden(1);
    ui->frame_message->setHidden(1);
}

void MainWindow::on_actionShow_View_triggered()
{
    ui->frame_opencascade->setHidden(0);
    ui->frame_message->setHidden(0);
}

void MainWindow::on_actionHide_controls_triggered()
{
    ui->frame_controls->setHidden(1);
}

void MainWindow::on_actionShow_controls_triggered()
{
    ui->frame_controls->setHidden(0);
}
