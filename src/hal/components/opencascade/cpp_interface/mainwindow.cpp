#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cplusplus.h"
#include "libgcode/interface.h"
#include <libdialog/portable-file-dialogs.h>

bool init_model=0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    OpencascadeWidget = new Opencascade(this);
    ui->gridLayout_opencascade->addWidget(OpencascadeWidget);

    //This activates a screen update when robot is moving and screen needs to be updated automaticly.
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::Update_Opencascade);
    timer->start(200);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Update_Opencascade()
{    
    // Using extern variables from next class:
    ui->lineEdit_j0->setText(QString::number(databucket.J0,'f',3));
    ui->lineEdit_j1->setText(QString::number(databucket.J1,'f',3));
    ui->lineEdit_j2->setText(QString::number(databucket.J2,'f',3));
    ui->lineEdit_j3->setText(QString::number(databucket.J3,'f',3));
    ui->lineEdit_j4->setText(QString::number(databucket.J4,'f',3));
    ui->lineEdit_j5->setText(QString::number(databucket.J5,'f',3));
    ui->lineEdit_cartx->setText(QString::number(databucket.Cartx,'f',3));
    ui->lineEdit_carty->setText(QString::number(databucket.Carty,'f',3));
    ui->lineEdit_cartz->setText(QString::number(databucket.Cartz,'f',3));
    ui->lineEdit_eulerx->setText(QString::number(databucket.Eulerx,'f',3));
    ui->lineEdit_eulery->setText(QString::number(databucket.Eulery,'f',3));
    ui->lineEdit_eulerz->setText(QString::number(databucket.Eulerz,'f',3));

    ui->lineEdit_gcode_x->setText(QString::number(databucket.gcode_x,'f',3));
    ui->lineEdit_gcode_y->setText(QString::number(databucket.gcode_y,'f',3));
    ui->lineEdit_gcode_z->setText(QString::number(databucket.gcode_z,'f',3));
    ui->lineEdit_gcode_euler_x->setText(QString::number(databucket.gcode_euler_x,'f',3));
    ui->lineEdit_gcode_euler_y->setText(QString::number(databucket.gcode_euler_y,'f',3));
    ui->lineEdit_gcode_euler_z->setText(QString::number(databucket.gcode_euler_z,'f',3));

    // Load machine model stepfiles:
    if(!init_model){

        std::string basepath=stepfilepath_evar;
        OpencascadeWidget->Readstepfile(basepath+"/base.step");
        OpencascadeWidget->Readstepfile(basepath+"/joint_1.step");
        OpencascadeWidget->Readstepfile(basepath+"/joint_2.step");
        OpencascadeWidget->Readstepfile(basepath+"/joint_3.step");
        OpencascadeWidget->Readstepfile(basepath+"/joint_4.step");
        OpencascadeWidget->Readstepfile(basepath+"/joint_5.step");
        OpencascadeWidget->Readstepfile(basepath+"/joint_6.step");

        OpencascadeWidget->setup_tcp_origin();

        init_model=1;
    }

    // Update machine model joints following the hal input pins:
    if(init_model){
        OpencascadeWidget->update_jointpos(databucket.J0*toRadians,
                                           databucket.J1*toRadians,
                                           databucket.J2*toRadians,
                                           databucket.J3*toRadians,
                                           databucket.J4*toRadians,
                                           databucket.J5*toRadians);
    }


}

void MainWindow::gcode(){

    if (!pfd::settings::available()){
        std::cout << "Portable File Dialogs are not available on this platform. \n"
                     "On linux install zenity, $ sudo apt-get install zenity\n";
    }
    auto f = pfd::open_file("Choose files to read", "/home/user/Desktop/Cam/build-qt-dxf-Desktop-Debug/" /*directory().currentdir()*/,
                            { "Dxf Files (.ngc)", "*.ngc",
                              "All Files", "*" }, pfd::opt::none); // Or ::multiselect.
    // functionname(f.result().at(0)); // This lib can open multiple results.

    // (Intro)
    // G21 (unit mm)
    // G40 (cutter compensation off)
    // G80 (cancel canned cycle modal motion)
    // G90 (absolute distance, no offsets)
    // G64P0.01 (path following accuracy)

    // M30 (outtro)

    std::vector<interface::block> blkvec=interface().read_gcode_file(f.result().at(0));

    for(unsigned int i=0; i<f.result().at(0).size(); i++){
        gcode_filename[i]= f.result().at(0).at(i);
    }

    gp_Pnt p1{0,0,0},p2{0,0,0},pc{0,0,0};

    // Robot offsets
    // Kuka x=630, y=0, z=890
    double x=databucket.gcode_x;
    double y=databucket.gcode_x;
    double z=databucket.gcode_x;
    double euler_x=databucket.gcode_euler_x;
    double euler_y=databucket.gcode_euler_y;
    double euler_z=databucket.gcode_euler_z;

    for(unsigned int i=0; i<blkvec.size(); i++){

        p2={blkvec.at(i).X,blkvec.at(i).Y,blkvec.at(i).Z};
        double I=blkvec.at(i).I;
        double J=blkvec.at(i).J;
        double K=blkvec.at(i).K;
        pc.SetX(p1.X()+I);
        pc.SetY(p1.Y()+J);
        pc.SetZ(p1.Z()+K);

        if(i>0 && blkvec.at(i).type=="G0"){
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_3d_line(p1,p2);
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLUE,0);

            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);

            //Ais_shape=draw_primitives().translate_3d(Ais_shape,{0,0,0},{x,y,z});
            OpencascadeWidget->show_shape(Ais_shape);
        }
        if(i>0 && blkvec.at(i).type=="G1"){
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_3d_line(p1,p2);

            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
            // Ais_shape=draw_primitives().translate_3d(Ais_shape,{0,0,0},{x,y,z});
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLACK,0);
            OpencascadeWidget->show_shape(Ais_shape);
        }
        if(i>0 && (blkvec.at(i).type=="G2")){

            // Arc section
            // X=xend, Y=yend. For arc given a G0 startposition and a XY endposition. http://linuxcnc.org/docs/html/gcode/g-code.html#gcode:g2-g3
            // I=offset xcenter-xstart, J=offset ycenter-ystart, G2=clockwise (cw), G3=counterclockwise (ccw)
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_cp_2d_arc(pc,p2,p1);

            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
            //Ais_shape=draw_primitives().translate_3d(Ais_shape,{0,0,0},{x,y,z});
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLACK,0);
            OpencascadeWidget->show_shape(Ais_shape);
        }
        if(i>0 && (blkvec.at(i).type=="G3")){
            Handle(AIS_Shape) Ais_shape=draw_primitives().draw_cp_2d_arc(pc,p2,p1);

            Ais_shape=draw_primitives().rotate_translate_3d_quaternion(Ais_shape,{x,y,z},euler_z,euler_y,euler_x);
            //Ais_shape=draw_primitives().translate_3d(Ais_shape,{0,0,0},{x,y,z});
            Ais_shape=draw_primitives().colorize(Ais_shape,Quantity_NOC_BLACK,0);
            OpencascadeWidget->show_shape(Ais_shape);
        }
        p1=p2;
    }
}

// Open dxf example :
/*
void MainWindow::open_dxf(){
    std::string filename=ui->lineEdit_dxf_file->text().toStdString();
    std::vector<Handle(AIS_Shape)> shapes=libdxfrw_functions().open_dxf_file(filename);

    for(unsigned int i=0; i<shapes.size(); i++){
        shapes.at(i)=draw_primitives().colorize(shapes.at(i),Quantity_NOC_BLUE,0);

        gp_Trsf trsf;
        trsf.SetTranslation({0,0,0},{500,0,500});

        shapes.at(i)->SetLocalTransformation(trsf);
        OpencascadeWidget->show_shape(shapes.at(i));
    }
}
*/


void MainWindow::on_pushButton_pressed()
{
    gcode();
}
