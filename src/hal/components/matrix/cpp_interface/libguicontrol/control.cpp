#include "control.h"
#include "ui_control.h"

Control::Control(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Control)
{
    ui->setupUi(this);

    ui->toolButton_estop->setStyleSheet(stylesheet_red);
    ui->pushButton_page_control->setStyleSheet(stylesheet_orange);
    ui->pushButton_manual->setStyleSheet(stylesheet_green);
    ui->pushButton_offline->setStyleSheet(stylesheet_purple);
    ui->pushButton_stop->setStyleSheet(stylesheet_red);

    //! Setup external controls.
    lbl_kinmode=ui->label_kinematic_mode_value;
    lbl_message=ui->label_message;

    pb_j0_min=ui->pushButton_J0_min;
    pb_j0_plus=ui->pushButton_J0_plus;
    pb_j1_min=ui->pushButton_J1_min;
    pb_j1_plus=ui->pushButton_J1_plus;
    pb_j2_min=ui->pushButton_J2_min;
    pb_j2_plus=ui->pushButton_J2_plus;
    pb_j3_min=ui->pushButton_J3_min;
    pb_j3_plus=ui->pushButton_J3_plus;
    pb_j4_min=ui->pushButton_J4_min;
    pb_j4_plus=ui->pushButton_J4_plus;
    pb_j5_min=ui->pushButton_J5_min;
    pb_j5_plus=ui->pushButton_J5_plus;

    pb_cart_x_min=ui->pushButton_cart_x_min;
    pb_cart_x_plus=ui->pushButton_cart_x_plus;
    pb_cart_y_min=ui->pushButton_cart_y_min;
    pb_cart_y_plus=ui->pushButton_cart_y_plus;
    pb_cart_z_min=ui->pushButton_cart_z_min;
    pb_cart_z_plus=ui->pushButton_cart_z_plus;

    pb_euler_x_min=ui->pushButton_euler_x_min;
    pb_euler_x_plus=ui->pushButton_euler_x_plus;
    pb_euler_y_min=ui->pushButton_euler_y_min;
    pb_euler_y_plus=ui->pushButton_euler_y_plus;
    pb_euler_z_min=ui->pushButton_euler_z_min;
    pb_euler_z_plus=ui->pushButton_euler_z_plus;

    pb_tool_x_min=ui->pushButton_tool_x_min;
    pb_tool_x_plus=ui->pushButton_tool_x_plus;
    pb_tool_y_min=ui->pushButton_tool_y_min;
    pb_tool_y_plus=ui->pushButton_tool_y_plus;
    pb_tool_z_min=ui->pushButton_tool_z_min;
    pb_tool_z_plus=ui->pushButton_tool_z_plus;

    pb_zero_rel_all=ui->pushButton_zero_all;
    pb_zero_rel_cartx=ui->pushButton_zero_cartx;
    pb_zero_rel_carty=ui->pushButton_zero_carty;
    pb_zero_rel_cartz=ui->pushButton_zero_cartz;
    pb_zero_rel_eulerx=ui->pushButton_zero_eulerx;
    pb_zero_rel_eulery=ui->pushButton_zero_eulery;
    pb_zero_rel_eulerz=ui->pushButton_zero_eulerz;
    pb_zero_rel_J0=ui->pushButton_zero_J0;
    pb_zero_rel_J1=ui->pushButton_zero_J1;
    pb_zero_rel_J2=ui->pushButton_zero_J2;
    pb_zero_rel_J3=ui->pushButton_zero_J3;
    pb_zero_rel_J4=ui->pushButton_zero_J4;
    pb_zero_rel_J5=ui->pushButton_zero_J5;

    pb_mdi_exec=ui->pushButton_mdi_exec;

    pb_reload_gcode=ui->pushButton_reload_gcode;

    le_J0_abs=ui->lineEdit_J0_abs;
    le_J0_rel=ui->lineEdit_J0_rel;
    le_J1_abs=ui->lineEdit_J1_abs;
    le_J1_rel=ui->lineEdit_J1_rel;
    le_J2_abs=ui->lineEdit_J2_abs;
    le_J2_rel=ui->lineEdit_J2_rel;
    le_J3_abs=ui->lineEdit_J3_abs;
    le_J3_rel=ui->lineEdit_J3_rel;
    le_J4_abs=ui->lineEdit_J4_abs;
    le_J4_rel=ui->lineEdit_J4_rel;
    le_J5_abs=ui->lineEdit_J5_abs;
    le_J5_rel=ui->lineEdit_J5_rel;

    le_cart_x_abs=ui->lineEdit_cart_x_abs;
    le_cart_x_rel=ui->lineEdit_cart_x_rel;
    le_cart_y_abs=ui->lineEdit_cart_y_abs;
    le_cart_y_rel=ui->lineEdit_cart_y_rel;
    le_cart_z_abs=ui->lineEdit_cart_z_abs;
    le_cart_z_rel=ui->lineEdit_cart_z_rel;

    le_euler_x_abs=ui->lineEdit_euler_x_abs;
    le_euler_x_rel=ui->lineEdit_euler_x_rel;
    le_euler_y_abs=ui->lineEdit_euler_y_abs;
    le_euler_y_rel=ui->lineEdit_euler_y_rel;
    le_euler_z_abs=ui->lineEdit_euler_z_abs;
    le_euler_z_rel=ui->lineEdit_euler_z_rel;

    tb_estop=ui->toolButton_estop;
    pb_offline=ui->pushButton_offline;
    pb_online=ui->pushButton_online;

    pb_manual=ui->pushButton_manual;
    pb_auto=ui->pushButton_auto;
    pb_run=ui->pushButton_run;
    pb_mdi=ui->pushButton_mdi;
    pb_pause=ui->pushButton_pause;
    pb_stop=ui->pushButton_stop;

    pb_m3=ui->pushButton_m3_on;
    pb_m5=ui->pushButton_m5_off;
    pb_m7=ui->pushButton_m7_on;
    pb_m8=ui->pushButton_m8_on;
    pb_m9=ui->pushButton_m9_off;

    spb_maxvel_cart=ui->spinBox_maxvel_cart;
    spb_maxvel_euler=ui->spinBox_maxvel_euler;

    dspb_maxacc_cart=ui->doubleSpinBox_maxacc_cart;
    dspb_maxacc_euler=ui->doubleSpinBox_maxacc_euler;
    dspb_stepsize=ui->doubleSpinBox_stepsize;
    dspb_feed_overide=ui->doubleSpinBox_feed_overide;

    run_from_line=ui->lineEdit_run_from_line;

    //! Setup internal controls.
    QObject::connect(ui->pushButton_page_control, SIGNAL(pressed()),SLOT(pushButton_page_control_pressed()));
    QObject::connect(ui->pushButton_page_gcode, SIGNAL(pressed()),SLOT(pushButton_page_gcode_pressed()));
    QObject::connect(ui->pushButton_page_user, SIGNAL(pressed()),SLOT(pushButton_page_user_pressed()));

    QObject::connect(ui->toolButton_estop, SIGNAL(pressed()),SLOT(pushButton_toolButton_estop_pressed()));

    QObject::connect(ui->pushButton_online, SIGNAL(pressed()),SLOT(pushButton_online_pressed()));
    QObject::connect(ui->pushButton_offline, SIGNAL(pressed()),SLOT(pushButton_offline_pressed()));

    QObject::connect(ui->pushButton_auto, SIGNAL(pressed()),SLOT(pushButton_auto_pressed()));
    QObject::connect(ui->pushButton_mdi, SIGNAL(pressed()),SLOT(pushButton_mdi_pressed()));
    QObject::connect(ui->pushButton_manual, SIGNAL(pressed()),SLOT(pushButton_manual_pressed()));

    QObject::connect(ui->pushButton_m3_on, SIGNAL(pressed()),SLOT(pushButton_m3_pressed()));
    QObject::connect(ui->pushButton_m5_off, SIGNAL(pressed()),SLOT(pushButton_m5_pressed()));
    QObject::connect(ui->pushButton_m7_on, SIGNAL(pressed()),SLOT(pushButton_m7_pressed()));
    QObject::connect(ui->pushButton_m8_on, SIGNAL(pressed()),SLOT(pushButton_m8_pressed()));
    QObject::connect(ui->pushButton_m9_off, SIGNAL(pressed()),SLOT(pushButton_m9_pressed()));

    QObject::connect(ui->pushButton_run, SIGNAL(pressed()),SLOT(pushButton_run_pressed()));
    QObject::connect(ui->pushButton_pause, SIGNAL(pressed()),SLOT(pushButton_pause_pressed()));
    QObject::connect(ui->pushButton_stop, SIGNAL(pressed()),SLOT(pushButton_stop_pressed()));

    QObject::connect(ui->pushButton_open_gcode, SIGNAL(pressed()),SLOT(pushButton_open_gcode_pressed()));
    QObject::connect(ui->pushButton_close_gcode, SIGNAL(pressed()),SLOT(pushButton_close_gcode_pressed()));
    QObject::connect(ui->pushButton_mdi_exec, SIGNAL(pressed()),SLOT(pushButton_mdi_exec_pressed()));

    //! Connect visa versa.
    QObject::connect(ui->spinBox_maxvel_cart, QOverload<int>::of(&QSpinBox::valueChanged),ui->horizontalScrollBar_maxvel_cart, &QScrollBar::setValue);
    QObject::connect(ui->horizontalScrollBar_maxvel_cart, QOverload<int>::of(&QScrollBar::valueChanged), ui->spinBox_maxvel_cart, &QSpinBox::setValue);

    QObject::connect(ui->spinBox_maxvel_euler, QOverload<int>::of(&QSpinBox::valueChanged),ui->horizontalScrollBar_maxvel_euler, &QScrollBar::setValue);
    QObject::connect(ui->horizontalScrollBar_maxvel_euler, QOverload<int>::of(&QScrollBar::valueChanged), ui->spinBox_maxvel_euler, &QSpinBox::setValue);
}

Control::~Control()
{
    delete ui;
}

void Control::highlight_gcode_line(int i){

    //! Highlight a line.
    QTextCursor cursor_1(ui->textBrowser_gcode->document()->findBlockByLineNumber(i));
    QTextBlockFormat format;
    format.setBackground(Qt::lightGray);
    cursor_1.select(QTextCursor::LineUnderCursor);
    cursor_1.setBlockFormat(format);

    //! Restore color previous line.
    if(gcode_previousline%2==0){
        format.setBackground(QColor(170,255,255,125));
    } else {
        format.setBackground(QColor(170,255,255,255));
    }
    QTextCursor cursor_0(ui->textBrowser_gcode->document()->findBlockByLineNumber(gcode_previousline));
    cursor_0.select(QTextCursor::LineUnderCursor);
    cursor_0.setBlockFormat(format);

    //! Update previousline.
    gcode_previousline=i;

    //! Center cursor to current gcode text.
    double scrollbarmaxvalue=ui->textBrowser_gcode->verticalScrollBar()->maximum();

    double pos=(double(i)/gcodevec.at(machine_id+1).blockvec.size())*scrollbarmaxvalue;
    ui->textBrowser_gcode->verticalScrollBar()->setValue(int(pos));
}

void Control::pushButton_open_gcode_pressed()
{
    QFileDialog *d=new QFileDialog(this);
    QPalette p=MainWindow().palette();
    d->setPalette(p);

    if (d->exec()){
        gcodefilename = QString::fromUtf8(d->selectedFiles()[0].toUtf8().constData()).toStdString();
    }
    load();
}

void Control::load(){
    //! Load the filename in memory again.
    if(gcodefilename.size()!=0){

        ui->textBrowser_gcode->clear();

        //! Load gcode file into ordered data.
        gcode_load(gcodefilename,machine_id+1);
        //! Parse gcode into rich html text to preview in colors.
        gcode_parse_blockstrings(machine_id+1);

        for(unsigned int i=0; i<gcodevec.at(machine_id+1).blockstringvec.size(); i++){
            ui->textBrowser_gcode->append(QString::fromStdString(gcodevec.at(machine_id+1).blockstringvec.at(i)));

            //! Colorize
            QTextBlockFormat format;
            if(i%2==0){
                format.setBackground(QColor(170,255,255,125));
            } else {
                format.setBackground(QColor(170,255,255,255));
            }
            QTextCursor cursor_0(ui->textBrowser_gcode->document()->findBlockByLineNumber(i));
            cursor_0.select(QTextCursor::LineUnderCursor);
            cursor_0.setBlockFormat(format);
        }

        gcode_previousline=0;

        //! Highlight gcode and set scrollbar to 0.
        highlight_gcode_line(1);
        //! Previous line indexed by this way.
        highlight_gcode_line(0);

        //! Draw graphics. This has to be done from within the mainwindow.
        //! We set the flag its ready to load. The flag will get a reset by the mainwindow.
        gcode_ready_to_load=true;
    }
}

//! Fill the mdivec[x] container.
void Control::pushButton_mdi_exec_pressed()
{
    std::setprecision(3);
    std::string id=std::to_string(machine_id);
    std::string filename="/opt/hal-core/rtlib/mdi_machine"+id+".ngc";

    //! Write mdi file.
    std::string aText=ui->plainTextEdit_mdi->toPlainText().toStdString();
    std::fstream f;
    f.open (filename, std::ios_base::out);
    if (!f.is_open()) {
        std::cout<<"error opening mdi file from guicontrol, mdi exec function"<<std::endl;
    } else {
        //! For the first move, we start from current machine position to the first mdi line position.
        //! To avoid scientific notation's we use a std::fixed output. The gpr gcode reader can not handle this otherwise.
        f <<std::fixed<< "G0 X" << ptr->machine[machine_id].rel_cart.x << " Y" << ptr->machine[machine_id].rel_cart.y << " Z" << ptr->machine[machine_id].rel_cart.z << " A" << ptr->machine[machine_id].rel_euler.x << " B" << ptr->machine[machine_id].rel_euler.y << " C" << ptr->machine[machine_id].rel_euler.z << " F1500"<< std::endl;
        //! The mdi line content.
        f <<std::fixed<< aText;
        f.close();
    }

    //! Load mdi file into ordered data.
    gcode_load(filename,0);

    //! Draw graphics. This has to be done from within the mainwindow.
    //! We set the flag its ready to load. The flag will get a reset by the mainwindow.
    mdi_ready_to_load=true;
}

void Control::pushButton_close_gcode_pressed()
{
    ui->textBrowser_gcode->clear();
    gcode_ready_to_close=true;
}

void Control::pushButton_m3_pressed()
{
    ui->pushButton_m3_on->setStyleSheet(stylesheet_green);
    ui->pushButton_m5_off->setStyleSheet(stylesheet_grey);
    ui->label_message->setText("m3");
}

void Control::pushButton_m5_pressed()
{
    ui->pushButton_m3_on->setStyleSheet(stylesheet_grey);
    ui->pushButton_m5_off->setStyleSheet(stylesheet_grey);
    ui->label_message->setText("m5");
}

void Control::pushButton_m7_pressed()
{
    ui->pushButton_m7_on->setStyleSheet(stylesheet_green);
    ui->pushButton_m9_off->setStyleSheet(stylesheet_grey);
    ui->label_message->setText("m7");
}

void Control::pushButton_m8_pressed()
{
    ui->pushButton_m8_on->setStyleSheet(stylesheet_green);
    ui->pushButton_m9_off->setStyleSheet(stylesheet_grey);
    ui->label_message->setText("m8");
}

void Control::pushButton_m9_pressed()
{
    ui->pushButton_m7_on->setStyleSheet(stylesheet_grey);
    ui->pushButton_m8_on->setStyleSheet(stylesheet_grey);
    ui->label_message->setText("m9");
}

void Control::pushButton_run_pressed()
{
    ui->label_message->setText("run");
}

void Control::pushButton_pause_pressed()
{
    ui->label_message->setText("pause");
}

void Control::pushButton_stop_pressed()
{
    ui->label_message->setText("stop");
}

void Control::pushButton_auto_pressed()
{
    ui->label_message->setText("auto");
}

void Control::pushButton_manual_pressed()
{
    ui->label_message->setText("manual");
}

void Control::pushButton_mdi_pressed()
{
    ui->label_message->setText("mdi");
}

void Control::pushButton_offline_pressed()
{
    ui->label_message->setText("offline");
}

void Control::pushButton_online_pressed()
{
    ui->label_message->setText("online");
}

void Control::pushButton_toolButton_estop_pressed()
{
    if(ui->toolButton_estop->isChecked()){
    } else {
    }
}

void Control::pushButton_page_control_pressed()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->pushButton_page_control->setStyleSheet(stylesheet_orange);
    ui->pushButton_page_gcode->setStyleSheet(stylesheet_grey);
    ui->pushButton_page_user->setStyleSheet(stylesheet_grey);
    ui->label_message->setText("control");
}

void Control::pushButton_page_gcode_pressed()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->pushButton_page_control->setStyleSheet(stylesheet_grey);
    ui->pushButton_page_gcode->setStyleSheet(stylesheet_orange);
    ui->pushButton_page_user->setStyleSheet(stylesheet_grey);
    ui->label_message->setText("gcode");
}

void Control::pushButton_page_user_pressed()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->pushButton_page_control->setStyleSheet(stylesheet_grey);
    ui->pushButton_page_gcode->setStyleSheet(stylesheet_grey);
    ui->pushButton_page_user->setStyleSheet(stylesheet_orange);
    ui->label_message->setText("user");
}

//! Load a gcode file with a filedialog.
//! Then save the gcode in blocks and add extra info to the blocks like pathlengts for each block.
void Control::gcode_load(std::string filename, int gcodevec_id){

    //! Create storage.
    BLOCK b;
    std::vector<BLOCK> bvec;

    using namespace gpr;

    std::ifstream t(filename);
    std::string file_contents((std::istreambuf_iterator<char>(t)),
                              std::istreambuf_iterator<char>());

    gcode_program p = parse_gcode(file_contents);

    for(int i=0; i<p.num_blocks(); i++){
        //! std::cout<<"gcode line chunck size:"<<p.get_block(i).size()<<std::endl; // Text editor line +1.
        for(int chunk=0; chunk<p.get_block(i).size(); chunk++){
            char a='0';
            if(p.get_block(i).get_chunk(chunk).tp()==CHUNK_TYPE_WORD_ADDRESS){ // tp=type
                a=p.get_block(i).get_chunk(chunk).get_word();
            }
            char axisletter;
            int gtype=11111;
            int macro_nr=11111;

            //! M3, Can acually be used in gcode as "M3 P22", this will call "tool on" with a value of 22.
            //! This can be handy for robot grippers startpositions, etc. You name it!
            //! It can also be used to add extra joints to the machine.
            //! This function can hold all types of M codes.
            if(a=='M' || a=='m'){
                macro_nr=p.get_block(i).get_chunk(chunk).get_address().int_value();

                for(int j=chunk+1; j<p.get_block(i).size(); j++){
                    axisletter=p.get_block(i).get_chunk(j).get_word();
                    if(axisletter=='P' || axisletter=='p'){
                        //! The macro value. Like "M3 P22", where "P" holds the value "22".
                        b.P=p.get_block(i).get_chunk(j).get_address().double_value();
                    }
                }
                b.blocktype=M;
                //! For example M3, where "3" is the gtype.
                b.Macro_nr=macro_nr;

                bvec.push_back(b);
            }

            if(a=='G' || a=='g'){
                //! std::cout<<"G found"<<std::endl;
                //! Find 0,1,2,3
                gtype=p.get_block(i).get_chunk(chunk).get_address().int_value();

                if(gtype==0){

                    for(int j=chunk+1; j<p.get_block(i).size(); j++){
                        // Get the xyz values.
                        axisletter=p.get_block(i).get_chunk(j).get_word();
                        if(axisletter=='X' || axisletter=='x'){
                            b.X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            b.Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            b.Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='I' || axisletter=='i'){
                            b.I=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='J' || axisletter=='j'){
                            b.J=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='K' || axisletter=='k'){
                            b.K=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='A' || axisletter=='a'){
                            b.A=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='B' || axisletter=='b'){
                            b.B=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='C' || axisletter=='c'){
                            b.C=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            //! For a G0 rapid. Use the current gui's maxvel.
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='S' || axisletter=='s'){
                            b.S=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G0;
                    bvec.push_back(b);
                    //! std::cout<<"g0 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" u:"<<b.A<<" v:"<<b.B<<" w:"<<b.C<<std::endl;
                }
                if(gtype==1){
                    //! std::cout<<"G1, draw a line"<<std::endl;

                    for(int j=chunk+1; j<p.get_block(i).size(); j++){
                        axisletter=p.get_block(i).get_chunk(j).get_word();
                        if(axisletter=='X' || axisletter=='x'){
                            b.X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            b.Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            b.Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='I' || axisletter=='i'){
                            b.I=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='J' || axisletter=='j'){
                            b.J=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='K' || axisletter=='k'){
                            b.K=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='A' || axisletter=='a'){
                            b.A=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='B' || axisletter=='b'){
                            b.B=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='C' || axisletter=='c'){
                            b.C=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='S' || axisletter=='s'){
                            b.S=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G1;
                    bvec.push_back(b);
                    //! std::cout<<"g1 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" u:"<<b.A<<" v:"<<b.B<<" w:"<<b.C<<" f:"<<b.F<<std::endl;
                }
                if(gtype==2){
                    //! std::cout<<"G2, draw a cw arc"<<std::endl;
                    for(int j=chunk+1; j<p.get_block(i).size(); j++){
                        axisletter=p.get_block(i).get_chunk(j).get_word();
                        if(axisletter=='X' || axisletter=='x'){
                            b.X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            b.Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            b.Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='I' || axisletter=='i'){
                            b.I=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='J' || axisletter=='j'){
                            b.J=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='K' || axisletter=='k'){
                            b.K=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='A' || axisletter=='a'){
                            b.A=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='B' || axisletter=='b'){
                            b.B=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='C' || axisletter=='c'){
                            b.C=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='S' || axisletter=='s'){
                            b.S=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G2;
                    bvec.push_back(b);
                    //! std::cout<<"g2 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" i:"<<b.I<<" j:"<<b.J<<" k:"<<b.K<<" u:"<<b.A<<" v:"<<b.B<<" w:"<<b.C<<" f:"<<b.F<<std::endl;
                }
                if(gtype==3){
                    //! std::cout<<"G3, draw a ccw arc"<<std::endl;
                    for(int j=chunk+1; j<p.get_block(i).size(); j++){
                        axisletter=p.get_block(i).get_chunk(j).get_word();
                        if(axisletter=='X' || axisletter=='x'){
                            b.X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            b.Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            b.Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='I' || axisletter=='i'){
                            b.I=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='J' || axisletter=='j'){
                            b.J=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='K' || axisletter=='k'){
                            b.K=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='A' || axisletter=='a'){
                            b.A=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='B' || axisletter=='b'){
                            b.B=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='C' || axisletter=='c'){
                            b.C=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='S' || axisletter=='s'){
                            b.S=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G3;
                    bvec.push_back(b);
                    //! std::cout<<"g3 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" i:"<<b.I<<" j:"<<b.J<<" k:"<<b.K<<" u:"<<b.A<<" v:"<<b.B<<" w:"<<b.C<<" f:"<<b.F<<std::endl;
                }
            }
        }
    }

    //! Process the gcodeblocks.
    POINT previous={0,0,0};
    EULER euler_previous={0,0,0};
    for(unsigned int i=0; i<bvec.size(); i++){
        bvec.at(i).start=previous;
        bvec.at(i).euler_start=euler_previous;
        if(bvec.at(i).blocktype==BLOCKTYPE::G2 || bvec.at(i).blocktype==BLOCKTYPE::G3){
            //! [G2] I=offset xcenter-xstart, J=offset ycenter-ystart, G2=clockwise (cw), G3=counterclockwise (ccw)
            bvec.at(i).center={bvec.at(i).I+bvec.at(i).start.x,bvec.at(i).J+bvec.at(i).start.y,bvec.at(i).start.z};
        }
        bvec.at(i).end={bvec.at(i).X,bvec.at(i).Y,bvec.at(i).Z};
        bvec.at(i).euler_end={bvec.at(i).A,bvec.at(i).B,bvec.at(i).C};
        previous=bvec.at(i).end;
        euler_previous=bvec.at(i).euler_end;

        //! Blocklenghts:
        bvec.at(i)=line_lenght(bvec.at(i));
        bvec.at(i)=arc_lenght(bvec.at(i));

        bvec.at(i).vel_start=0;
        bvec.at(i).vel_end=0;
    }
    gcodevec.at(gcodevec_id).blockvec=bvec;
}

//! Helper functions
BLOCK Control::line_lenght(BLOCK b){
    if(b.blocktype==BLOCKTYPE::G0 || b.blocktype==BLOCKTYPE::G1){
        b.blocklenght=sqrt(pow(b.end.x-b.start.x,2)+pow(b.end.y-b.start.y,2)+pow(b.end.z-b.start.z,2));
    }
    return b;
}

BLOCK Control::arc_lenght(BLOCK b){

    if(b.blocktype==BLOCKTYPE::G2 || b.blocktype==BLOCKTYPE::G3){
        double radius=sqrt(pow(b.end.x-b.center.x,2)+pow(b.end.y-b.center.y,2)+pow(b.end.z-b.center.z,2));
        double diameter=radius*2;
        double circumfence=diameter*M_PI;
        double start_angle, end_angle, arc_angle, arc_lenght;

        if(b.blocktype==BLOCKTYPE::G2){
            start_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
            end_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
        }

        if(b.blocktype==BLOCKTYPE::G3){
            start_angle = atan2(b.start.y-b.center.y, b.start.x-b.center.x);
            end_angle = atan2(b.end.y-b.center.y, b.end.x-b.center.x);
        }

        if(end_angle < start_angle){  //this to avoid the start angle is bigger then the end angle value
            end_angle += 2*M_PI;
        }

        arc_angle=end_angle-start_angle;
        arc_lenght=(arc_angle/(2*M_PI))*circumfence;
        b.blocklenght=arc_lenght;
    }
    return b;
}

//! Gui gcode text display
void Control::gcode_parse_blockstrings(int gcodevec_id){

    double X=-1, Y=-1, Z=-1, A=-1, B=-1, C=-1, II=0, J=0, K=0, F=0, S=0;

    for(unsigned int i=0; i<gcodevec.at(gcodevec_id).blockvec.size(); i++){

        QTextEdit *textedit = new QTextEdit();

        textedit->moveCursor (QTextCursor::End);

        //! Add line number.
        textedit->setTextColor(QColor("blue"));
        textedit->setFontWeight(QFont::DemiBold);
        textedit->insertPlainText ("N");
        textedit->moveCursor (QTextCursor::End);

        textedit->setTextColor(QColor("black"));
        textedit->setFontWeight(QFont::Normal);
        textedit->insertPlainText (QString::number(i)+" ");
        textedit->moveCursor (QTextCursor::End);

        //! A workaround to create alignment without a cosmetic too large \t tab insert.
        //! It seems to need 2 spaces for each char move.
        if(i>9999){
            textedit->insertPlainText ("");
        }
        if(i>999 && i<10000){
            textedit->insertPlainText ("  ");
        }
        if(i>99 && i<1000){
            textedit->insertPlainText ("    ");
        }
        if(i>9 && i<100){
            textedit->insertPlainText ("      ");
        }
        if(i<10){
            textedit->insertPlainText ("        ");
        }
        textedit->moveCursor (QTextCursor::End);

        //! Insert G..
        if(gcodevec.at(gcodevec_id).blockvec.at(i).blocktype==BLOCKTYPE::G0){
            textedit->setTextColor(QColor("blue"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("G");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText ("0   ");
        }
        if(gcodevec.at(gcodevec_id).blockvec.at(i).blocktype==BLOCKTYPE::G1){
            textedit->setTextColor(QColor("blue"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("G");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText ("1   ");
        }
        if(gcodevec.at(gcodevec_id).blockvec.at(i).blocktype==BLOCKTYPE::G2){
            textedit->setTextColor(QColor("blue"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("G");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText ("2   ");
        }
        if(gcodevec.at(gcodevec_id).blockvec.at(i).blocktype==BLOCKTYPE::G3){
            textedit->setTextColor(QColor("blue"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("G");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText ("3   ");
        }

        //! Insert Macro..
        if(gcodevec.at(gcodevec_id).blockvec.at(i).blocktype==BLOCKTYPE::M){
            textedit->setTextColor(QColor("red"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("M");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            //! Macro id, type int.
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).Macro_nr,'f',0)+"  ");
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("P ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            //! P carry's the macro value, type double.
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).P,'f',3)+"  ");
        }

        textedit->moveCursor (QTextCursor::End);

        //! Insert positions.

        if(X!=gcodevec.at(gcodevec_id).blockvec.at(i).X){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("X ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).X,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }
        if(Y!=gcodevec.at(gcodevec_id).blockvec.at(i).Y){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("Y ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).Y,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }

        if(Z!=gcodevec.at(gcodevec_id).blockvec.at(i).Z){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("Z ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).Z,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }

        if(A!=gcodevec.at(gcodevec_id).blockvec.at(i).A){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("A ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).A,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }

        if(B!=gcodevec.at(gcodevec_id).blockvec.at(i).B){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("B ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).B,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }
        if(C!=gcodevec.at(gcodevec_id).blockvec.at(i).C){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("C ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).C,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }
        if(II!=gcodevec.at(gcodevec_id).blockvec.at(i).I){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("I ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).I,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }
        if(J!=gcodevec.at(gcodevec_id).blockvec.at(i).J){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("J ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).J,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }
        if(K!=gcodevec.at(gcodevec_id).blockvec.at(i).K){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("K ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).K,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }
        if(F!=gcodevec.at(gcodevec_id).blockvec.at(i).F){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("F ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).F,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }
        if(S!=gcodevec.at(gcodevec_id).blockvec.at(i).S){
            textedit->setTextColor(QColor("purple"));
            textedit->setFontWeight(QFont::DemiBold);
            textedit->insertPlainText ("S ");
            textedit->setTextColor(QColor("black"));
            textedit->setFontWeight(QFont::Normal);
            textedit->insertPlainText (QString::number(gcodevec.at(gcodevec_id).blockvec.at(i).S,'f',3)+"  ");
            textedit->moveCursor (QTextCursor::End);
        }

        X=gcodevec.at(gcodevec_id).blockvec.at(i).X;
        Y=gcodevec.at(gcodevec_id).blockvec.at(i).Y;
        Z=gcodevec.at(gcodevec_id).blockvec.at(i).Z;
        A=gcodevec.at(gcodevec_id).blockvec.at(i).A;
        B=gcodevec.at(gcodevec_id).blockvec.at(i).B;
        C=gcodevec.at(gcodevec_id).blockvec.at(i).C;

        II=gcodevec.at(gcodevec_id).blockvec.at(i).I;
        J=gcodevec.at(gcodevec_id).blockvec.at(i).J;
        K=gcodevec.at(gcodevec_id).blockvec.at(i).K;

        F=gcodevec.at(gcodevec_id).blockvec.at(i).F;
        S=gcodevec.at(gcodevec_id).blockvec.at(i).S;

        //! Convert to html format, to be used as rich text string wich is compatible for a QLabel later on.
        gcodevec.at(gcodevec_id).blockstringvec.push_back(textedit->toHtml().toStdString());

    }
}



