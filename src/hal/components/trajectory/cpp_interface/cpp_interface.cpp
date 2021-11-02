#include "cpp_interface.h"

//! Different types of gcode operations.
enum BLOCKTYPE {
    G0,
    G1,
    G2,
    G3
};

//! A block is a chunk of gcode that represents a primitive like a line or arc.
struct BLOCK {
    BLOCKTYPE blocktype=BLOCKTYPE::G0;
    int mcode=0;
    double power_rpm=0;
    POINT start={0,0,0}, center={0,0,0}, end={0,0,0};
    EULER euler_start={0,0,0}, euler_end={0,0,0};
    //! Calculated line or arc lenght.
    double blocklenght=0;
    double feedrate=0;
};
BLOCK rapid; // Used for a rapid G0 move to go to the program start position.
BLOCK jog;
BLOCK mdi;

//! A path is a collection of blocks.
struct PATH {
    std::vector<BLOCK> blockvec;
    double pathlenght=0;
};
std::vector<PATH> pathvec;
//! Indicates in wich pathvec the traject is active.
unsigned int pvi=0;
bool in_position=0;
char *filename;

bool trigger_start=0;
bool trigger_pvi=0;
bool trigger_in_position=0;
bool trigger_stop=0;
bool trigger_run_from_line=0;
POINT a,b;
double gcode_velocity=0;

//! Defenition helper functions.
double line_lenght(BLOCK line);
double arc_lenght(BLOCK arc);
POINT interpolate_line(BLOCK line, double distancetogo, bool debug);
POINT interpolate_arc(BLOCK arc, double distancetogo, bool debug);
void open_gcode_file(std::string filename, bool debug);

ruckig::Ruckig<1> otg {0.001};
ruckig::InputParameter<1> in;
ruckig::OutputParameter<1> out;
std::array<double, 1> position, velocity, acceleration;
std::array<double, 1> rapid_position, rapid_velocity, rapid_acceleration;

//! jog mode
//! xyz axis
ruckig::Ruckig<1> otg_x {0.001};
ruckig::Ruckig<1> otg_y {0.001};
ruckig::Ruckig<1> otg_z {0.001};
ruckig::InputParameter<1> in_x;
ruckig::OutputParameter<1> out_x;
ruckig::InputParameter<1> in_y;
ruckig::OutputParameter<1> out_y;
ruckig::InputParameter<1> in_z;
ruckig::OutputParameter<1> out_z;
std::array<double, 1> jog_x_position, jog_x_velocity, jog_x_acceleration;
std::array<double, 1> jog_y_position, jog_y_velocity, jog_y_acceleration;
std::array<double, 1> jog_z_position, jog_z_velocity, jog_z_acceleration;

Cpp_interface::Cpp_interface()
{
}

//! Setup a demo traject
void Cpp_interface::load_gcode(char *name){

    std::cout<<"gcode to load :"<<name<<std::endl;

    //    if (!pfd::settings::available()){
    //        std::cout << "Portable File Dialogs are not available on this platform. \n"
    //                     "On linux install zenity, $ sudo apt-get install zenity\n";
    //    }
    //    auto f = pfd::open_file("Choose files to read", "/home/user/Desktop/Cam/build-qt-dxf-Desktop-Debug/" /*directory().currentdir()*/,
    //                            { "Dxf Files (.ngc)", "*.ngc",
    //                              "All Files", "*" }, pfd::opt::none); // Or ::multiselect.
    //    // functionname(f.result().at(0)); // This lib can open multiple results.

    //    // std::vector<gcodeinterface::block> block;

    filename=name;
    open_gcode_file(filename,/* debug */ true);
}

//! Mode auto, performing a collection of trajects.
//! A traject is a collection of gcode blocks between M3 and M5 or a rapic G0.
//!
//! When the trajectory is running, user can set:
//!     max_velocity, results in forward- or backward path move.
//!     max_acceleration
//!     max_jerk
void Cpp_interface::trajectory_auto(double velocity_override,
                                   bool start, bool stop,
                                   double vel, double acc, double jerk,
                                   unsigned int startfromblock, bool run_from_line){

    POINT p;
    if(run_from_line){
        trigger_run_from_line=1;
    }
    if(trigger_run_from_line){
        trigger_pvi=0;
        trigger_in_position=0;
        trigger_run_from_line=0;
        trigger_start=0;
    }

    if(start){ // Stop current move with dcc path, and start again.
        trigger_start=1;
    }
    if(trigger_start){
        tcp.m3=0;
        tcp.m7=0;
        tcp.m8=0;

        trigger_stop=0;
        trigger_start=0;
    }

    if(stop){
        trigger_stop=1;
    }
    if(trigger_stop){
        tcp.m3=0;
        tcp.m7=0;
        tcp.m8=0;
        vel=0;
        trigger_start=0;
    }

    if(pathvec.size()>0){

        if(!trigger_pvi){ // Set start block.
            unsigned int nr=0;
            for(unsigned int i=0; i<pathvec.size(); i++){

                for(unsigned int j=0; j<pathvec.at(i).blockvec.size(); j++){
                    if(nr==startfromblock){
                        pvi=i;
                        break;
                    }
                    nr++;
                }
                if(nr==startfromblock){
                    break;
                }
            }

            a={tcp.p.x,tcp.p.y,tcp.p.z};
            b=pathvec.at(pvi).blockvec.front().start;
            rapid_position[0]=0; // set the rapic pathlenght to begin position;
            position[0]=0;
            trigger_pvi=1;
        }

        if(trigger_pvi && !trigger_in_position){
            BLOCK rapid;
            rapid.start=a;
            rapid.end=b;
            rapid.blocklenght=line_lenght(rapid);
            rapid.blocktype=BLOCKTYPE::G0;

            in.max_velocity[0]=abs(vel);
            in.max_acceleration[0]=acc;
            in.max_jerk[0]=jerk;
            in.current_position[0]=rapid_position[0];
            in.current_velocity[0]=rapid_velocity[0];
            in.current_acceleration[0]=rapid_acceleration[0];
            in.target_position[0]=rapid.blocklenght;
            in.target_velocity[0]=0;
            in.target_acceleration[0]=0;

            auto result = otg.update(in, out);
            result = ruckig::Result::Working;

            // One ms forward.
            out.trajectory.at_time(0.001, rapid_position, rapid_velocity, rapid_acceleration);

            // If in position, end this function.  double blocklenght=pathvec.at(pvi).blockvec.at(i).blocklenght;
            double distancetogo=rapid.blocklenght-rapid_position[0];
            p=interpolate_line(rapid,distancetogo,0);
            // std::cout<<"Moving to program start position p.x: "<<p.x<<" p.y:"<<p.y<<" p.z:"<<p.z<<std::endl;
            //tcp.vel=rapid_velocity[0];
            //tcp.acc=rapid_acceleration[0];

            // Forward destiny reached, increment to "next" path, if there is one.
            if(rapid_position[0]>rapid.blocklenght-1){
                trigger_in_position=1;
                // std::cout<<"In program start position"<<std::endl;
            }
        }

        if(trigger_pvi && trigger_in_position){ // Perform the program in auto mode.

            // Maxvel +/- is forward or backward, like adaptive feed.
            if(vel>0){
                in.target_position[0]=pathvec.at(pvi).pathlenght; // Goto path end.
            }
            if(vel<0){
                in.target_position[0]=0; // Goto path begin.
            }
            if(vel==0){ // Cannot input 0 as value. Create a stop sequence. For emergency a instand stop can be done like : vel=0
                vel=0.0001;
                if(velocity[0]<0.001 && velocity[0]>0.000){
                    vel=0;
                }
            }

            in.max_velocity[0]=/*abs(vel)*/ gcode_velocity; // std::cout<<"gcode velocity:"<<gcode_velocity<<std::endl;
            in.max_acceleration[0]=acc;
            in.max_jerk[0]=jerk;
            in.current_position[0]=position[0];
            in.current_velocity[0]=velocity[0];
            in.current_acceleration[0]=acceleration[0];
            in.target_velocity[0]=0;
            in.target_acceleration[0]=0;

            auto result = otg.update(in, out);
            result = ruckig::Result::Working;

            // One ms forward.
            out.trajectory.at_time(0.001, position, velocity, acceleration);
            // For ocsilloscope output.
            // tcp_auto.pos=position[0]; // std::cout<<"tcp pos: "<<tcp.pos<<std::endl;

            // Gcode block that is currently in use, output is interpolated xyz position.
            double lenght=0;
            for(unsigned int i=0; i<pathvec.at(pvi).blockvec.size(); i++){
                double begin=lenght;
                lenght+=pathvec.at(pvi).blockvec.at(i).blocklenght;
                double end=lenght;

                if(position[0]>=begin && position[0]<=end){
                    // std::cout<<"block i:"<<i<<std::endl;

                    // Set the gcode feedrate
                    if(pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G1 ||
                            pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G2 ||
                            pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G3){
                        gcode_velocity=abs(pathvec.at(pvi).blockvec.at(i).feedrate*velocity_override);
                        if(gcode_velocity>vel){
                            gcode_velocity=abs(vel); // Don't go faster then the max gui velocity value.
                        }
                    }

                    if(pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G0){
                        gcode_velocity=abs(vel);
                    }

                    double blocklenght=pathvec.at(pvi).blockvec.at(i).blocklenght;
                    double blockpos=position[0]-begin; // From 0 to blocklenght.
                    double distancetogo=blocklenght-blockpos;

                    // Interpolate xyz coordinates of the gcode primitive given the distancetogo.
                    if(pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G0 || pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G1){
                        p=interpolate_line(pathvec.at(pvi).blockvec.at(i),distancetogo,0);
                        // std::cout<<"line p.x: "<<p.x<<" p.y:"<<p.y<<" p.z:"<<p.z<<std::endl;
                        // tcp.vel=velocity[0];
                        //tcp.acc=acceleration[0];

                        if(pathvec.at(pvi).blockvec.at(i).mcode==3){
                            tcp.m3=1;
                            tcp.power_rpm=pathvec.at(pvi).blockvec.at(i).power_rpm;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==7){
                            tcp.m7=1;
                            tcp.power_rpm=pathvec.at(pvi).blockvec.at(i).power_rpm;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==8){
                            tcp.m8=1;
                            tcp.power_rpm=pathvec.at(pvi).blockvec.at(i).power_rpm;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==5){
                            tcp.m3=0;
                            tcp.power_rpm=0;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==9){
                            tcp.m7=0;
                            tcp.m8=0;
                            tcp.power_rpm=0;
                        }
                    }
                    if(pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G2 || pathvec.at(pvi).blockvec.at(i).blocktype==BLOCKTYPE::G3){
                        p=interpolate_arc(pathvec.at(pvi).blockvec.at(i),distancetogo,0);
                        // std::cout<<"arc p.x: "<<p.x<<" p.y:"<<p.y<<" p.z:"<<p.z<<std::endl;
                        //tcp.vel=velocity[0];
                        //tcp.acc=acceleration[0];

                        if(pathvec.at(pvi).blockvec.at(i).mcode==3){
                            tcp.m3=1;
                            tcp.power_rpm=pathvec.at(pvi).blockvec.at(i).power_rpm;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==7){
                            tcp.m7=1;
                            tcp.power_rpm=pathvec.at(pvi).blockvec.at(i).power_rpm;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==8){
                            tcp.m8=1;
                            tcp.power_rpm=pathvec.at(pvi).blockvec.at(i).power_rpm;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==5){
                            tcp.m3=0;
                            tcp.power_rpm=0;
                        }
                        if(pathvec.at(pvi).blockvec.at(i).mcode==9){
                            tcp.m7=0;
                            tcp.m8=0;
                            tcp.power_rpm=0;
                        }
                    }
                    break;
                }
            }

            // Forward destiny reached, increment to "next" path, if there is one.
            if(position[0]==pathvec.at(pvi).pathlenght && vel>0){
                if(pvi<pathvec.size()-1){
                    pvi++;
                    // std::cout<<"end of traject reached, forward velocity, into next path"<<std::endl;

                    // Set position to begin of next path.
                    position[0]=0;
                }
            }

            // backward destiny reached, decrement to "previous" path, if there is one.
            if(position[0]==0 && vel<0){
                if(pvi>0){
                    pvi--;
                    // std::cout<<"begin of traject reached, backward velocity, into previous path"<<std::endl;

                    // Set position to end of (already previous) path
                    position[0]=pathvec.at(pvi).pathlenght;
                }
            }

            // std::cout<<"traject pvi:"<<pvi<<std::endl;
            // std::cout<<"pos:"<<position[0]<<std::endl;
            // std::cout<<"vel:"<<velocity[0]<<std::endl;
            // std::cout<<"acc:"<<acceleration[0]<<std::endl;
        }
    }
    trajectory_jog_auto(p.x,p.y,p.z,7500,5000,2500);
}

//! C module function wrappers:
extern "C" void wrapper_load_gcode(char* name){ // C to cpp wrapper function
    Cpp_interface().load_gcode(name);
}

//! C module function wrappers:
extern "C" void wrapper_trajectory_auto(double velocity_override,
                                       bool start, bool stop,
                                       double vel, double acc, double jerk,
                                       unsigned int startfromblock, bool run_from_line){ // C to cpp wrapper function
    Cpp_interface().trajectory_auto(velocity_override,
                                          start, stop,
                                          vel, acc, jerk,
                                          startfromblock, run_from_line);
}

//! Mode jog.
void Cpp_interface::trajectory_jog_auto(double x, double y, double z,
                                  double vel, double acc, double jerk){

    // For each axis we use a seperate ruckig trajectory planner.
    double velx=vel;
    double vely=vel;
    double velz=vel;

    // Jog x ******************
    in_x.max_velocity[0]=abs(velx);
    in_x.max_acceleration[0]=acc;
    in_x.max_jerk[0]=jerk;
    in_x.current_position[0]=tcp.p.x;
    in_x.current_velocity[0]=jog_x_velocity[0];
    in_x.current_acceleration[0]=jog_x_acceleration[0];
    in_x.target_velocity[0]=0;
    in_x.target_acceleration[0]=0;
    in_x.target_position[0]=x;

    auto result_x = otg_x.update(in_x, out_x);
    result_x = ruckig::Result::Working;

    // One ms forward.
    out_x.trajectory.at_time(0.001, jog_x_position, jog_x_velocity, jog_x_acceleration);
    tcp.p.x=jog_x_position[0];
    // std::cout<<"vel jog x: "<<jog_x_velocity[0]<<std::endl;

    // Jog y ******************
    in_y.max_velocity[0]=abs(vely);
    in_y.max_acceleration[0]=acc;
    in_y.max_jerk[0]=jerk;
    in_y.current_position[0]=tcp.p.y;
    in_y.current_velocity[0]=jog_y_velocity[0];
    in_y.current_acceleration[0]=jog_y_acceleration[0];
    in_y.target_velocity[0]=0;
    in_y.target_acceleration[0]=0;
    in_y.target_position[0]=y;

    auto result_y = otg_y.update(in_y, out_y);
    result_y = ruckig::Result::Working;

    // One ms forward.
    out_y.trajectory.at_time(0.001, jog_y_position, jog_y_velocity, jog_y_acceleration);
    tcp.p.y=jog_y_position[0];
    // std::cout<<"vel jog y: "<<jog_y_velocity[0]<<std::endl;

    // Jog z ******************
    in_z.max_velocity[0]=abs(velz);
    in_z.max_acceleration[0]=acc;
    in_z.max_jerk[0]=jerk;
    in_z.current_position[0]=tcp.p.z;
    in_z.current_velocity[0]=jog_z_velocity[0];
    in_z.current_acceleration[0]=jog_z_acceleration[0];
    in_z.target_velocity[0]=0;
    in_z.target_acceleration[0]=0;
    in_z.target_position[0]=z;

    auto result_z = otg_z.update(in_z, out_z);
    result_z = ruckig::Result::Working;

    // One ms forward.
    out_z.trajectory.at_time(0.001, jog_z_position, jog_z_velocity, jog_z_acceleration);
    tcp.p.z=jog_z_position[0];
    // std::cout<<"vel jog z: "<<jog_z_velocity[0]<<std::endl;
}

//! Mode jog.
void Cpp_interface::trajectory_jog(int jog_x, int jog_y, int jog_z,
                                  double vel, double acc, double jerk){

    // For each axis we use a seperate ruckig trajectory planner.
    double velx=vel;
    double vely=vel;
    double velz=vel;

    // Jog x ******************
    if(jog_x==0){
        velx=0.0001;
        if(jog_x_velocity[0]<0.001 && jog_x_velocity[0]>0.000){
            velx=0;
        }
        // std::cout<<"stop the jog move"<<std::endl;
    }
    double dx;
    if(jog_x>0){
        dx=1000;
        // std::cout<<"jog x+"<<std::endl;
    }
    if(jog_x<0){
        dx=-1000;
        // std::cout<<"jog x-"<<std::endl;
    }

    in_x.max_velocity[0]=abs(velx);
    in_x.max_acceleration[0]=acc;
    in_x.max_jerk[0]=jerk;
    in_x.current_position[0]=tcp.p.x;
    in_x.current_velocity[0]=jog_x_velocity[0];
    in_x.current_acceleration[0]=jog_x_acceleration[0];
    in_x.target_velocity[0]=0;
    in_x.target_acceleration[0]=0;
    in_x.target_position[0]=jog_x_position[0]+dx;

    auto result_x = otg_x.update(in_x, out_x);
    result_x = ruckig::Result::Finished;

    // One ms forward.
    out_x.trajectory.at_time(0.001, jog_x_position, jog_x_velocity, jog_x_acceleration);
    tcp.p.x=jog_x_position[0];
    // std::cout<<"vel jog x: "<<jog_x_velocity[0]<<std::endl;

    // Jog y ******************
    if(jog_y==0){
        vely=0.0001;
        if(jog_y_velocity[0]<0.001 && jog_y_velocity[0]>0.000){
            vely=0;
        }
        // std::cout<<"stop the jog move"<<std::endl;
    }
    double dy;
    if(jog_y>0){
        dy=1000;
        // std::cout<<"jog y+"<<std::endl;
    }
    if(jog_y<0){
        dy=-1000;
        // std::cout<<"jog y-"<<std::endl;
    }

    in_y.max_velocity[0]=abs(vely);
    in_y.max_acceleration[0]=acc;
    in_y.max_jerk[0]=jerk;
    in_y.current_position[0]=tcp.p.y;
    in_y.current_velocity[0]=jog_y_velocity[0];
    in_y.current_acceleration[0]=jog_y_acceleration[0];
    in_y.target_velocity[0]=0;
    in_y.target_acceleration[0]=0;
    in_y.target_position[0]=jog_y_position[0]+dy;

    auto result_y = otg_y.update(in_y, out_y);
    result_y = ruckig::Result::Finished;

    // One ms forward.
    out_y.trajectory.at_time(0.001, jog_y_position, jog_y_velocity, jog_y_acceleration);
    tcp.p.y=jog_y_position[0];
    // std::cout<<"vel jog y: "<<jog_y_velocity[0]<<std::endl;

    // Jog z ******************
    if(jog_z==0){
        velz=0.0001;
        if(jog_z_velocity[0]<0.001 && jog_z_velocity[0]>0.000){
            velz=0;
        }
        // std::cout<<"stop the jog move"<<std::endl;
    }
    double dz;
    if(jog_z>0){
        dz=1000;
        // std::cout<<"jog z+"<<std::endl;
    }
    if(jog_z<0){
        dz=-1000;
        // std::cout<<"jog z-"<<std::endl;
    }

    in_z.max_velocity[0]=abs(velz);
    in_z.max_acceleration[0]=acc;
    in_z.max_jerk[0]=jerk;
    in_z.current_position[0]=tcp.p.z;
    in_z.current_velocity[0]=jog_z_velocity[0];
    in_z.current_acceleration[0]=jog_z_acceleration[0];
    in_z.target_velocity[0]=0;
    in_z.target_acceleration[0]=0;
    in_z.target_position[0]=jog_z_position[0]+dz;

    auto result_z = otg_z.update(in_z, out_z);
    result_z = ruckig::Result::Finished;

    // One ms forward.
    out_z.trajectory.at_time(0.001, jog_z_position, jog_z_velocity, jog_z_acceleration);
    tcp.p.z=jog_z_position[0];
    // std::cout<<"vel jog z: "<<jog_z_velocity[0]<<std::endl;
}

extern "C" void wrapper_trajectory_jog(int jog_x, int jog_y, int jog_z,
                                      double vel, double acc, double jerk){
    Cpp_interface().trajectory_jog(jog_x, jog_y, jog_z,
                                         vel, acc, jerk);
}

extern "C" TCP get_tcp(){
    return tcp;
}

//! Helper functions
double line_lenght(BLOCK line){
    return sqrt(pow(line.end.x-line.start.x,2)+pow(line.end.y-line.start.y,2)+pow(line.end.z-line.start.z,2));
}

double arc_lenght(BLOCK arc){

    double radius=sqrt(pow(arc.end.x-arc.center.x,2)+pow(arc.end.y-arc.center.y,2)+pow(arc.end.z-arc.center.z,2));
    double diameter=radius*2;
    double circumfence=diameter*M_PI;
    double start_angle, end_angle, arc_angle, arc_lenght;

    if(arc.blocktype==BLOCKTYPE::G2){
        start_angle = atan2(arc.end.y-arc.center.y, arc.end.x-arc.center.x);
        end_angle = atan2(arc.start.y-arc.center.y, arc.start.x-arc.center.x);
    }

    if(arc.blocktype==BLOCKTYPE::G3){
        start_angle = atan2(arc.start.y-arc.center.y, arc.start.x-arc.center.x);
        end_angle = atan2(arc.end.y-arc.center.y, arc.end.x-arc.center.x);
    }

    if(end_angle < start_angle){  //this to avoid the start angle is bigger then the end angle value
        end_angle += 2*M_PI;
    }

    arc_angle=end_angle-start_angle;
    arc_lenght=(arc_angle/(2*M_PI))*circumfence;

    return arc_lenght;
}

POINT interpolate_line(BLOCK line, double distancetogo, bool debug)
{
    double lenght=sqrt(pow(line.end.x-line.start.x,2)+pow(line.end.y-line.start.y,2)+pow(line.end.z-line.start.z,2));
    double ratio=0;

    ratio=1-(distancetogo/lenght);

    POINT p;
    p.x= line.start.x + ((line.end.x-line.start.x)*ratio);
    p.y= line.start.y + ((line.end.y-line.start.y)*ratio);
    p.z= line.start.z + ((line.end.z-line.start.z)*ratio);

    if(debug){
        std::cout<<"blocklenght:"<<lenght<<std::endl;
        std::cout<<"ratio:"<<ratio<<std::endl;
        std::cout<<"posx:"<<p.x<<" y:"<<p.y<<" z:"<<p.z<<std::endl;
        std::cout<<""<<std::endl;
    }
    return p;
}

POINT interpolate_arc(BLOCK arc, double distancetogo, bool debug){

    double radius=sqrt(pow(arc.end.x-arc.center.x,2)+pow(arc.end.y-arc.center.y,2)+pow(arc.end.z-arc.center.z,2));
    double diameter=radius*2;
    double circumfence=diameter*M_PI;
    double start_angle, end_angle, arc_angle, arc_lenght;

    if(arc.blocktype==BLOCKTYPE::G2){
        start_angle = atan2(arc.end.y-arc.center.y, arc.end.x-arc.center.x);
        end_angle = atan2(arc.start.y-arc.center.y, arc.start.x-arc.center.x);
    }

    if(arc.blocktype==BLOCKTYPE::G3){
        start_angle = atan2(arc.start.y-arc.center.y, arc.start.x-arc.center.x);
        end_angle = atan2(arc.end.y-arc.center.y, arc.end.x-arc.center.x);
    }

    if(end_angle < start_angle){  //this to avoid the start angle is bigger then the end angle value
        end_angle += 2*M_PI;
    }

    arc_angle=end_angle-start_angle;
    arc_lenght=(arc_angle/(2*M_PI))*circumfence;

    double ratio=distancetogo/arc_lenght;

    POINT p;
    if(arc.blocktype==BLOCKTYPE::G2){
        p.x = radius * cos(start_angle+(ratio*arc_angle));//we now start at the start angle
        p.y = radius * sin(start_angle+(ratio*arc_angle));
    }
    if(arc.blocktype==BLOCKTYPE::G3){
        p.x = radius * cos(start_angle+((1-ratio)*arc_angle));//we now start at the start angle
        p.y = radius * sin(start_angle+((1-ratio)*arc_angle));
    }

    p.x+=arc.center.x;
    p.y+=arc.center.y;
    p.z=arc.center.z;

    if(debug){
        if(arc.blocktype==BLOCKTYPE::G2){
            std::cout<<"arc of type G2"<<std::endl;
        }
        if(arc.blocktype==BLOCKTYPE::G3){
            std::cout<<"arc of type G3"<<std::endl;
        }
        std::cout<<"diameter:"<<diameter<<std::endl;
        std::cout<<"circumfence:"<<circumfence<<std::endl;
        std::cout<<"start_angle:"<<start_angle<<std::endl;
        std::cout<<"end_angle:"<<end_angle<<std::endl;
        std::cout<<"arclenght:"<<arc_lenght<<std::endl;
        std::cout<<"ratio:"<<ratio<<std::endl;
        std::cout<<"tcp_x:"<<p.x<<" y:"<<p.y<<" z:"<<p.z<<std::endl;
        std::cout<<""<<std::endl;
    }
    return p;
}

void open_gcode_file(std::string filename, bool debug){

    pathvec.clear();
    PATH path;
    BLOCK l;
    POINT previous={0,0,0};
    bool inpath=0;
    int mtype=11111;
    double svalue=0;

    std::ifstream t(filename.c_str());
    std::string file_contents((std::istreambuf_iterator<char>(t)),
                              std::istreambuf_iterator<char>());

    gpr::gcode_program p = gpr::parse_gcode(file_contents);

    // Value's are only changed when value changes.
    double X=0,Y=0,Z=0,I=0,J=0,K=0,F=0;

    for(int i=0; i<p.num_blocks(); i++){
        //std::cout<<"gcode line chunck size:"<<p.get_block(i).size()<<std::endl; // Text editor line +1.

        for(int chunk=0; chunk<p.get_block(i).size(); chunk++){

            /* Example
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
            if(p.get_block(i).get_chunk(chunk).tp()==gpr::CHUNK_TYPE_WORD_ADDRESS){ // tp=type
                a=p.get_block(i).get_chunk(chunk).get_word();
            }
            char axisletter;
            int gtype=11111;

            if(a=='S' || a=='s'){
                if(debug){
                    // std::cout<<"S found"<<std::endl;
                }
                // Find 3,5
                svalue=p.get_block(i).get_chunk(chunk).get_address().double_value();
                if(debug){
                    std::cout<<"S value:"<<gtype<<std::endl;
                }
            }

            if(a=='M' || a=='m'){
                if(debug){
                    // std::cout<<"M found"<<std::endl;
                }
                // Find 3,5
                mtype=p.get_block(i).get_chunk(chunk).get_address().int_value();
                if(debug){
                    std::cout<<"M type:"<<mtype<<std::endl;
                }

                // Trigger is we are in a path, else save each item as a path.
                if(mtype==3 || mtype==7 || mtype==8){
                    inpath=1;
                }
                if(mtype==5 || mtype==9){
                    inpath=0;
                }
                if(mtype==30){
                    inpath=0;
                }
            }

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
                            X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    l.mcode=mtype;
                    l.power_rpm=svalue;
                    l.blocktype=BLOCKTYPE::G0;
                    l.start={previous.x,previous.y,previous.z};
                    l.end={X,Y,Z};
                    previous.x=X; previous.y=Y; previous.z=Z;
                    l.blocklenght=line_lenght(l);
                    path.pathlenght+=l.blocklenght;
                    path.blockvec.push_back(l);

                    if(!inpath){
                        pathvec.push_back(path);
                        path.blockvec.clear();
                        path.pathlenght=0;
                    }
                    if(debug){
                        std::cout<<"g0 x:"<<X<<" y:"<<Y<<" z:"<<Z<<" f:"<<F<<std::endl;
                    }
                }
                if(gtype==1){
                    // std::cout<<"G1, draw a line"<<std::endl;

                    for(int j=chunk+1; j<p.get_block(i).size(); j++){
                        // Get the xyz values.
                        axisletter=p.get_block(i).get_chunk(j).get_word();
                        if(axisletter=='X' || axisletter=='x'){
                            X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    l.mcode=mtype;
                    l.power_rpm=svalue;
                    l.blocktype=BLOCKTYPE::G1;
                    l.start={previous.x,previous.y,previous.z};
                    l.end={X,Y,Z};
                    l.feedrate=F;
                    previous.x=X; previous.y=Y; previous.z=Z;
                    l.blocklenght=line_lenght(l);
                    path.pathlenght+=l.blocklenght;
                    path.blockvec.push_back(l);

                    if(!inpath){
                        pathvec.push_back(path);
                        path.blockvec.clear();
                        path.pathlenght=0;
                    }
                    if(debug){
                        std::cout<<"g1 x:"<<X<<" y:"<<Y<<" z:"<<Z<<" f:"<<F<<std::endl;
                    }
                }
                if(gtype==2){
                    // std::cout<<"G2, draw a cw arc"<<std::endl;

                    for(int j=chunk+1; j<p.get_block(i).size(); j++){
                        // Get the xyz values.
                        axisletter=p.get_block(i).get_chunk(j).get_word();
                        if(axisletter=='X' || axisletter=='x'){
                            X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='I' || axisletter=='i'){
                            I=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='J' || axisletter=='j'){
                            J=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='K' || axisletter=='k'){
                            K=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    l.mcode=mtype;
                    l.power_rpm=svalue;
                    l.blocktype=BLOCKTYPE::G2;
                    l.start={previous.x,previous.y,previous.z};
                    l.end={X,Y,Z};
                    // [G2] I=offset xcenter-xstart, J=offset ycenter-ystart, G2=clockwise (cw), G3=counterclockwise (ccw)
                    l.center={I+l.start.x,J+l.start.y,Z};
                    l.feedrate=F;
                    previous.x=X; previous.y=Y; previous.z=Z;
                    l.blocklenght=arc_lenght(l);
                    path.pathlenght+=l.blocklenght;
                    path.blockvec.push_back(l);

                    if(!inpath){
                        pathvec.push_back(path);
                        path.blockvec.clear();
                        path.pathlenght=0;
                    }
                    if(debug){
                        std::cout<<"g2 x:"<<X<<" y:"<<Y<<" z:"<<Z<<" i:"<<I<<" j:"<<J<<" k:"<<K<<" f:"<<F<<std::endl;
                    }
                }
                if(gtype==3){
                    // std::cout<<"G3, draw a ccw arc"<<std::endl;

                    for(int j=chunk+1; j<p.get_block(i).size(); j++){
                        // Get the xyz values.
                        axisletter=p.get_block(i).get_chunk(j).get_word();
                        if(axisletter=='X' || axisletter=='x'){
                            X=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Y' || axisletter=='y'){
                            Y=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='Z' || axisletter=='z'){
                            Z=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='I' || axisletter=='i'){
                            I=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='J' || axisletter=='j'){
                            J=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='K' || axisletter=='k'){
                            K=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                        if(axisletter=='F' || axisletter=='f'){
                            F=p.get_block(i).get_chunk(j).get_address().double_value();
                        }
                    }
                    l.mcode=mtype;
                    l.power_rpm=svalue;
                    l.blocktype=BLOCKTYPE::G3;
                    l.start={previous.x,previous.y,previous.z};
                    l.end={X,Y,Z};
                    // [G2] I=offset xcenter-xstart, J=offset ycenter-ystart, G2=clockwise (cw), G3=counterclockwise (ccw)
                    l.center={I+l.start.x,J+l.start.y,Z};
                    l.feedrate=F;
                    previous.x=X; previous.y=Y; previous.z=Z;
                    l.blocklenght=arc_lenght(l);
                    path.pathlenght+=l.blocklenght;
                    path.blockvec.push_back(l);

                    if(!inpath){
                        pathvec.push_back(path);
                        path.blockvec.clear();
                        path.pathlenght=0;
                    }
                    if(debug){
                        std::cout<<"g3 x:"<<X<<" y:"<<Y<<" z:"<<Z<<" i:"<<I<<" j:"<<J<<" k:"<<K<<" f:"<<F<<std::endl;
                    }
                }
            }
        }
    }
    if(debug){
        std::cout<<" "<<std::endl;
    }

    if(debug){
        for(unsigned int i=0; i<pathvec.size(); i++){
            std::cout<<"Path:"<<i<<std::endl;
            for(unsigned int j=0; j<pathvec.at(i).blockvec.size(); j++){

                std::cout<<"BLock:"<<j<<std::endl;
                if(pathvec.at(i).blockvec.at(j).blocktype==BLOCKTYPE::G0){
                    std::cout<<"BLocktype G0"<<std::endl;
                }
                if(pathvec.at(i).blockvec.at(j).blocktype==BLOCKTYPE::G1){
                    std::cout<<"BLocktype G1"<<std::endl;
                }
                if(pathvec.at(i).blockvec.at(j).blocktype==BLOCKTYPE::G2){
                    std::cout<<"BLocktype G2"<<std::endl;
                }
                if(pathvec.at(i).blockvec.at(j).blocktype==BLOCKTYPE::G3){
                    std::cout<<"BLocktype G3"<<std::endl;
                }
                std::cout<<"Start x:"<<pathvec.at(i).blockvec.at(j).start.x<<" y:"<<pathvec.at(i).blockvec.at(j).start.y<<" z:"<<pathvec.at(i).blockvec.at(j).start.z<<std::endl;
                if(pathvec.at(i).blockvec.at(j).blocktype==BLOCKTYPE::G2 || pathvec.at(i).blockvec.at(j).blocktype==BLOCKTYPE::G3){
                    std::cout<<"Center x:"<<pathvec.at(i).blockvec.at(j).center.x<<" y:"<<pathvec.at(i).blockvec.at(j).center.y<<" z:"<<pathvec.at(i).blockvec.at(j).center.z<<std::endl;
                }
                std::cout<<"End x:"<<pathvec.at(i).blockvec.at(j).end.x<<" y:"<<pathvec.at(i).blockvec.at(j).end.y<<" z:"<<pathvec.at(i).blockvec.at(j).end.z<<std::endl;

                std::cout<<"Feedrate:"<<pathvec.at(i).blockvec.at(j).feedrate<<std::endl;

                std::cout<<"Mcode:"<<pathvec.at(i).blockvec.at(j).mcode<<std::endl;
                std::cout<<"Svalue:"<<pathvec.at(i).blockvec.at(j).power_rpm<<std::endl;

            }
        }
    }
}






