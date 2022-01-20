#include "gcode_interface.h"

//! Trajectory runner.
//!
ruckig::Ruckig<1> otg {0.001};
ruckig::InputParameter<1> in;
ruckig::OutputParameter<1> out;
std::array<double, 1> vel, acc, pos;

/*
setp skynet.0.machine-maxvel-cart 500
setp skynet.0.machine-maxacc-cart 350
setp skynet.0.machine-jerk-cart 200

setp skynet.0.machine-maxvel-euler 3
setp skynet.0.machine-maxacc-euler 3
setp skynet.0.machine-jerk-euler 3
*/

gcode_interface::gcode_interface()
{

}

MACHINE gcode_interface::gcode_runner(MACHINE *ptr){

    if(ptr[0].mode==AUTO){

        unsigned int machines=ptr[0].data.machines;

        for(unsigned int i=0; i<machines; i++){

            POINT tcp;


            //! Just one Dofs.
            unsigned int id=0;
            in.max_velocity[id]=ptr[i].maxvel_cart;
            in.max_acceleration[id]=ptr[i].maxacc_cart;
            in.max_jerk[id]=ptr[i].jerk_cart;

            in.current_velocity[id]=ptr[i].curvel_gcode;
            in.current_acceleration[id]=ptr[i].curacc_gcode;
            in.current_position[id]=ptr[i].curpos_gcode;

            //! Tarpos gcode comes from gcode variable.
            //! Current executed gcodeblock.
            int curblock=ptr[i].exec_gcodeblock;
            double blocklenght=abs(gcode.blockvec.at(curblock).blocklenght);

            //! Set Dofs target position.
            in.target_position[id]=blocklenght;

            //! This values can be used when the gcode intepreter can predict this values by a look-ahead function.
            in.target_velocity[id]=0;
            in.target_acceleration[id]=0;

            //! enum: position, velocity
            in.control_interface=ruckig::ControlInterface::Position;
            //! Phase, ///< Phase synchronize the DoFs when possible, else fallback to "Time" strategy
            //! Time, ///< Always synchronize the DoFs to reach the target at the same time (Default)
            //! TimeIfNecessary, ///< Synchronize only when necessary (e.g. for non-zero target velocity or acceleration)
            //! None, ///< Calculate every DoF independently
            in.synchronization=ruckig::Synchronization::None;
            //! Continuous, ///< Every trajectory duration is allowed (Default)
            //! Discrete, ///< The trajectory duration must be a multiple of the control cycle
            in.duration_discretization=ruckig::DurationDiscretization::Continuous;

            auto result_x = otg.update(in,out);
            result_x = ruckig::Result::Finished;

            //! One ms forward.
            out.trajectory.at_time(0.001,pos, vel, acc);

            //! Result
            ptr[i].curpos_gcode=pos[id];
            ptr[i].curvel_gcode=vel[id];
            ptr[i].curacc_gcode=acc[id];

            //! What is the distance to go?
            double dtg=abs(blocklenght-pos[id]);
            std::cout<<"gcodeblocknr:"<<curblock<<" dtg:"<<dtg<<std::endl;

            //! Interpolate dtg result to cart xyz and euler xyz coordinates.
            //! Interpolate toolcenterpoint "tcp" as a line?
            if(gcode.blockvec.at(curblock).blocktype==G0 || gcode.blockvec.at(curblock).blocktype==G1){
                tcp=interpolate_line(gcode.blockvec.at(curblock),dtg);

                //std::cout<<"line interpolated tcp x:"<<tcp.x<<std::endl;
                //std::cout<<"line interpolated tcp y:"<<tcp.y<<std::endl;
                //std::cout<<"line interpolated tcp z:"<<tcp.z<<std::endl;
            }
            //! Interpolate as a arc?
            if(gcode.blockvec.at(curblock).blocktype==G2 || gcode.blockvec.at(curblock).blocktype==G3){
                tcp=interpolate_arc(gcode.blockvec.at(curblock),dtg);

                // std::cout<<"arc interpolated tcp x:"<<tcp.x<<std::endl;
                // std::cout<<"arc interpolated tcp y:"<<tcp.y<<std::endl;
                // std::cout<<"arc interpolated tcp z:"<<tcp.z<<std::endl;
            }
            //! Interpolate euler angles for each gcodeblock.
            //! The gcodeblock already knows the blocklenght at this stage.
            EULER eul=interpolate_euler(gcode.blockvec.at(curblock),dtg);

            std::cout<<"euler x:"<<eul.x<<std::endl;
            std::cout<<"euler y:"<<eul.y<<std::endl;
            std::cout<<"euler z:"<<eul.z<<std::endl;

            //! Set the target position of the machine.
            ptr[i].tarpos_cart_euler[0]=ptr[i].curoffset_cart_euler_rel[0]+tcp.x;
            ptr[i].tarpos_cart_euler[1]=ptr[i].curoffset_cart_euler_rel[1]+tcp.y;
            ptr[i].tarpos_cart_euler[2]=ptr[i].curoffset_cart_euler_rel[2]+tcp.z;
            ptr[i].tarpos_cart_euler[3]=ptr[i].curoffset_cart_euler_rel[3]+(eul.x*toRadians);
            ptr[i].tarpos_cart_euler[4]=ptr[i].curoffset_cart_euler_rel[4]+(eul.y*toRadians);
            ptr[i].tarpos_cart_euler[5]=ptr[i].curoffset_cart_euler_rel[5]+(eul.z*toRadians);
            //! Set machine mode : ik mode
            ptr[i].kinmode=KINMODE::ik_rapid;

            //! Forward motion gcodeblock traject finished, increment gcodeblock.
            if(dtg<0.01){
                if(ptr[i].exec_gcodeblock<gcode.blockvec.size()-1){
                    ptr[i].exec_gcodeblock++;

                    ptr[i].curpos_gcode=0;
                    ptr[i].curvel_gcode=0;
                    ptr[i].curacc_gcode=0;

                    std::cout<<"traject finished, new gcodeblock:"<<ptr[i].exec_gcodeblock<<std::endl;
                }
            }
        }
    }
    return *ptr;
}

extern "C" struct MACHINE gcode_runner(MACHINE *ptr){
    struct MACHINE result_ptr=gcode_interface().gcode_runner(ptr);
    return result_ptr;
};

//! Load a gcode file with a filedialog.
//! Then save the gcode in blocks and add extra info to the blocks like pathlengts for each block.
GCODE gcode_interface::load_gcode(){

    if (!pfd::settings::available()){
        std::cout << "Portable File Dialogs are not available on this platform. \n"
                     "On linux install zenity, $ sudo apt-get install zenity\n";
    }
    auto f = pfd::open_file("Choose files to read", "/opt/hal-core/src/hal/components/skynet/gcode/",
                            { "Ngc Files (.ngc)", "*.ngc",
                              "All Files", "*" }, pfd::opt::none); // Or ::multiselect.
    //open_dxf_file(f.result().at(0)); // This lib can open multiple results.

    //! Gcodefilename used to display in the gui.
    gcode.gcodefilename=f.result().at(0);

    BLOCK b;
    std::vector<BLOCK> blkvec;

    using namespace gpr;

    std::ifstream t(f.result().at(0));
    std::string file_contents((std::istreambuf_iterator<char>(t)),
                              std::istreambuf_iterator<char>());

    gcode_program p = parse_gcode(file_contents);

    for(int i=0; i<p.num_blocks(); i++){
        //std::cout<<"gcode line chunck size:"<<p.get_block(i).size()<<std::endl; // Text editor line +1.

        for(int chunk=0; chunk<p.get_block(i).size(); chunk++){

            /*
        std::cout<<"chunk data: "<<p.get_block(i).get_chunk(chunk)<<std::endl; // Text editor line +1.
        if(p.get_block(i).get_chunk(chunk).tp()==CHUNK_TYPE_WORD_ADDRESS){ // tp=type
            std::cout<<"chunk word id: "<<p.get_block(i).get_chunk(chunk).get_word()<<std::endl; // the chunk Id.

            block b=p.get_block(i);
            if(b.get_chunk(chunk).get_address().tp()==ADDRESS_TYPE_DOUBLE){
                std::cout<<"chunk double value: "<<b.get_chunk(chunk).get_address().double_value()<<std::endl;
            }

            if(b.get_chunk(chunk).get_address().tp()==ADDRESS_TYPE_INTEGER){
                std::cout<<"chunk integer value: "<<b.get_chunk(chunk).get_address().int_value()<<std::endl;
            }

            if(b.get_chunk(chunk).tp()==CHUNK_TYPE_COMMENT){
                std::cout<<"chunk comment: "<<b.get_chunk(chunk).get_comment_text()<<std::endl; // the chunk Id.
            }
        }
        */

            // Find the character : g,G
            char a='0';
            if(p.get_block(i).get_chunk(chunk).tp()==CHUNK_TYPE_WORD_ADDRESS){ // tp=type
                a=p.get_block(i).get_chunk(chunk).get_word();
            }
            char axisletter;
            int gtype=11111;


            if(a=='G' || a=='g'){
                // std::cout<<"G found"<<std::endl;
                // Find 0,1,2,3
                gtype=p.get_block(i).get_chunk(chunk).get_address().int_value();
                // std::cout<<"G type:"<<gtype<<std::endl;

                if(gtype==0){
                    // std::cout<<"G0, draw a rapid"<<std::endl;

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
                        if(axisletter=='U' || axisletter=='u'){
                            b.U=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='V' || axisletter=='v'){
                            b.V=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='W' || axisletter=='w'){
                            b.W=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G0;
                    blkvec.push_back(b);
                    std::cout<<"g0 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" u:"<<b.U<<" v:"<<b.V<<" w:"<<b.W<<std::endl;
                }
                if(gtype==1){
                    // std::cout<<"G1, draw a line"<<std::endl;

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
                        if(axisletter=='U' || axisletter=='u'){
                            b.U=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='V' || axisletter=='v'){
                            b.V=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='W' || axisletter=='w'){
                            b.W=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G1;
                    blkvec.push_back(b);
                    std::cout<<"g1 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" u:"<<b.U<<" v:"<<b.V<<" w:"<<b.W<<" f:"<<b.F<<std::endl;
                }
                if(gtype==2){
                    // std::cout<<"G2, draw a cw arc"<<std::endl;

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
                        if(axisletter=='U' || axisletter=='u'){
                            b.U=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='V' || axisletter=='v'){
                            b.V=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='W' || axisletter=='w'){
                            b.W=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G2;
                    blkvec.push_back(b);
                    std::cout<<"g2 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" i:"<<b.I<<" j:"<<b.J<<" k:"<<b.K<<" u:"<<b.U<<" v:"<<b.V<<" w:"<<b.W<<" f:"<<b.F<<std::endl;
                }
                if(gtype==3){
                    // std::cout<<"G3, draw a ccw arc"<<std::endl;

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
                        if(axisletter=='U' || axisletter=='u'){
                            b.U=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='V' || axisletter=='v'){
                            b.V=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='W' || axisletter=='w'){
                            b.W=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            b.F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    b.blocktype=G3;
                    blkvec.push_back(b);
                    std::cout<<"g3 x:"<<b.X<<" y:"<<b.Y<<" z:"<<b.Z<<" i:"<<b.I<<" j:"<<b.J<<" k:"<<b.K<<" u:"<<b.U<<" v:"<<b.V<<" w:"<<b.W<<" f:"<<b.F<<std::endl;
                }
            }
        }
    }
    std::cout<<" "<<std::endl; // the chunk Id.

    //! Process the gcodeblocks.
    POINT previous={0,0,0};
    EULER euler_previous={0,0,0};
    for(unsigned int i=0; i<blkvec.size(); i++){
        blkvec.at(i).start=previous;
        blkvec.at(i).euler_start=euler_previous;
        if(blkvec.at(i).blocktype==BLOCKTYPE::G2 || blkvec.at(i).blocktype==BLOCKTYPE::G3){
            // [G2] I=offset xcenter-xstart, J=offset ycenter-ystart, G2=clockwise (cw), G3=counterclockwise (ccw)
            blkvec.at(i).center={blkvec.at(i).I+blkvec.at(i).start.x,blkvec.at(i).J+blkvec.at(i).start.y,blkvec.at(i).start.z};
        }
        blkvec.at(i).end={blkvec.at(i).X,blkvec.at(i).Y,blkvec.at(i).Z};
        blkvec.at(i).euler_end={blkvec.at(i).U,blkvec.at(i).V,blkvec.at(i).W};
        previous=blkvec.at(i).end;
        euler_previous=blkvec.at(i).euler_end;
        blkvec.at(i)=line_lenght(blkvec.at(i));
        blkvec.at(i)=arc_lenght(blkvec.at(i));
    }
    GCODE gcode;
    gcode.blockvec=blkvec;
    return gcode;
}

/* .ngc example
G21 (unit mm)
G40 (cutter compensation off)
G80 (cancel canned cycle modal motion)
G90 (absolute distance, no offsets)
G64P0.01 (path following accuracy)
S2000.000000
G0 X500.000 Y-250.000 Z550.000
G0 X500.000 Y-250.000 Z500.000
M3
G1 X500.000 Y250.000 Z500.000 F2000
G1 X750.000 Y250.000 Z500.000 F2000
G1 X750.000 Y-250.000 Z500.000 F2000
G1 X500.000 Y-250.000 Z500.000 F2000
M5
G0 X500.000 Y-250.000 Z550.000 F2000
G0 X500.000 Y0.000 Z550.000 F2000
G0 X500.000 Y0.000 Z500.000 F2000
M3
G2 X750.000 Y0.000 Z500.000 I125 J0 K0.000 F2000
G2 X500.000 Y0.000 Z500.000 I-125 J0 K0.000 F2000
G0 X500.000 Y0.000 Z550.000
M30
*/

//! Helper functions
BLOCK gcode_interface::line_lenght(BLOCK b){
    if(b.blocktype==BLOCKTYPE::G0 || b.blocktype==BLOCKTYPE::G1){
        b.blocklenght=sqrt(pow(b.end.x-b.start.x,2)+pow(b.end.y-b.start.y,2)+pow(b.end.z-b.start.z,2));
    }
    return b;
}

BLOCK gcode_interface::arc_lenght(BLOCK b){

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

//! Calculate's the euler xyz.
EULER gcode_interface::interpolate_euler(BLOCK b, double distancetogo)
{
    double ratio=0;
    ratio=1-(distancetogo/b.blocklenght);

    EULER e;
    e.x= b.euler_start.x + ((b.euler_end.x-b.euler_start.x)*ratio);
    e.y= b.euler_start.y + ((b.euler_end.y-b.euler_start.y)*ratio);
    e.z= b.euler_start.z + ((b.euler_end.z-b.euler_start.z)*ratio);

    // std::cout<<"eulerx:"<<e.x<<" y:"<<e.y<<" z:"<<e.z<<std::endl;
    // std::cout<<""<<std::endl;

    return e;
}

//! Calculate's the tcp on a line.
POINT gcode_interface::interpolate_line(BLOCK b, double distancetogo)
{
    double lenght=sqrt(pow(b.end.x-b.start.x,2)+pow(b.end.y-b.start.y,2)+pow(b.end.z-b.start.z,2));
    double ratio=0;

    ratio=1-(distancetogo/lenght);

    POINT p;
    p.x= b.start.x + ((b.end.x-b.start.x)*ratio);
    p.y= b.start.y + ((b.end.y-b.start.y)*ratio);
    p.z= b.start.z + ((b.end.z-b.start.z)*ratio);

    // std::cout<<"blocklenght:"<<lenght<<std::endl;
    // std::cout<<"ratio:"<<ratio<<std::endl;
    // std::cout<<"posx:"<<p.x<<" y:"<<p.y<<" z:"<<p.z<<std::endl;
    // std::cout<<""<<std::endl;

    return p;
}

//! Calculate's the tcp on a arc.
POINT gcode_interface::interpolate_arc(BLOCK b, double distancetogo){

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

    double ratio=distancetogo/arc_lenght;

    POINT p;
    if(b.blocktype==BLOCKTYPE::G2){
        p.x = radius * cos(start_angle+(ratio*arc_angle));//we now start at the start angle
        p.y = radius * sin(start_angle+(ratio*arc_angle));
    }
    if(b.blocktype==BLOCKTYPE::G3){
        p.x = radius * cos(start_angle+((1-ratio)*arc_angle));//we now start at the start angle
        p.y = radius * sin(start_angle+((1-ratio)*arc_angle));
    }

    p.x+=b.center.x;
    p.y+=b.center.y;
    p.z=b.center.z;


    // if(b.blocktype==BLOCKTYPE::G2){
    // std::cout<<"arc of type G2"<<std::endl;
    // }
    // if(b.blocktype==BLOCKTYPE::G3){
    // std::cout<<"arc of type G3"<<std::endl;
    // }
    // std::cout<<"diameter:"<<diameter<<std::endl;
    // std::cout<<"circumfence:"<<circumfence<<std::endl;
    // std::cout<<"start_angle:"<<start_angle<<std::endl;
    // std::cout<<"end_angle:"<<end_angle<<std::endl;
    // std::cout<<"arclenght:"<<arc_lenght<<std::endl;
    // std::cout<<"ratio:"<<ratio<<std::endl;
    // std::cout<<"tcp_x:"<<p.x<<" y:"<<p.y<<" z:"<<p.z<<std::endl;
    // std::cout<<""<<std::endl;

    return p;
}
