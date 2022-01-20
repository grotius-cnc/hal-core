#include "matrix_3d.h"
#include <external_var.h>

matrix_3d::matrix_3d()
{

}

int matrix_3d::mult(double A[4][4], double B[4][4]){

    double C[4][4], num;

    for (int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            num = 0;
            for(int k=0; k<4; k++){
                num += A[i][k]*B[k][j];
            }
            C[i][j]=num;
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}

POINT matrix_3d::translate(POINT p, double x_translate, double y_translate, double z_translate){

    double matrix[4][4] = {
        {1, 0, 0, 0}, //[1 0 0 x]
        {0, 1, 0, 0}, //[0 1 0 y]
        {0, 0, 1, 0}, //[0 0 1 z]
        {0, 0, 0, 1}  //[0 0 0 1]
    };
    matrix[0][3]=p.x;
    matrix[1][3]=p.y;
    matrix[2][3]=p.z;

    double vector[4] = {0, 0, 0, 1}; //x,y,z,w //w=1 for translate, w=0 for direction
    vector[0]=x_translate;
    vector[1]=y_translate;
    vector[2]=z_translate;

    double x=(matrix[0][0]*vector[0])+
            (matrix[0][1]*vector[1])+
            (matrix[0][2]*vector[2])+
            (matrix[0][3]*vector[3]);

    double y=(matrix[1][0]*vector[0])+
            (matrix[1][1]*vector[1])+
            (matrix[1][2]*vector[2])+
            (matrix[1][3]*vector[3]);

    double z=(matrix[2][0]*vector[0])+
            (matrix[2][1]*vector[1])+
            (matrix[2][2]*vector[2])+
            (matrix[2][3]*vector[3]);
    /*
    double w=(matrix[3][0]*vector[0])+
            (matrix[3][1]*vector[1])+
            (matrix[3][2]*vector[2])+
            (matrix[3][3]*vector[3]);*/

    POINT result;
    result.x=x;
    result.y=y;
    result.z=z;
    return result;
}

POINT matrix_3d::scale(POINT p, double scale_x, double scale_y, double scale_z){

    double sx=scale_x;
    double sy=scale_y;
    double sz=scale_z;
    double matrix[4][4] = {
        {sx, 0,  0,  0}, //[x 0 0 0]
        {0,  sy, 0,  0}, //[0 y 0 0]
        {0,  0,  sz, 0}, //[0 0 z 0]
        {0,  0,  0,  1}  //[0 0 0 1]
    };

    double vector[4] = {0, 0, 0, 1}; //[x,y,z,w] //w=1 for translate, w=0 for direction
    vector[0]=p.x;
    vector[1]=p.y;
    vector[2]=p.z;

    double x=(matrix[0][0]*vector[0])+
            (matrix[0][1]*vector[1])+
            (matrix[0][2]*vector[2])+
            (matrix[0][3]*vector[3]);

    double y=(matrix[1][0]*vector[0])+
            (matrix[1][1]*vector[1])+
            (matrix[1][2]*vector[2])+
            (matrix[1][3]*vector[3]);

    double z=(matrix[2][0]*vector[0])+
            (matrix[2][1]*vector[1])+
            (matrix[2][2]*vector[2])+
            (matrix[2][3]*vector[3]);
    /*
    double w=(matrix[3][0]*vector[0])+
            (matrix[3][1]*vector[1])+
            (matrix[3][2]*vector[2])+
            (matrix[3][3]*vector[3]);*/

    POINT result;
    result.x=x;
    result.y=y;
    result.z=z;
    return result;
}

//! Implementation example:
//! POINT p=matrix_3d().rotate({100,0,0},{0,0,45},1);
//! std::cout<<"p.x:"<<p.x<<" p.y:"<<p.y<<" p.z:"<<p.z<<std::endl;
//! p.x:70.7107 p.y:70.7107 p.z:0
//!
//! POINT p=matrix_3d().rotate({0,0,100},{45,0,0},1);
//! std::cout<<"p.x:"<<p.x<<" p.y:"<<p.y<<" p.z:"<<p.z<<std::endl;
//! p.x:0 p.y:-70.7107 p.z:70.7107
POINT matrix_3d::rotate(POINT p, EULER e, bool unit_degrees){

    if(unit_degrees){
        e.x*=M_PI/180.0;
        e.y*=M_PI/180.0;
        e.z*=M_PI/180.0;
    }

    double cx=cos(e.x); //cos of rotate_degrees
    double sx=sin(e.x);
    double matrix_x[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                              { 1, 0,  0,  0 },
                              { 0, cx,-sx, 0 },
                              { 0, sx, cx, 0 },
                              { 0, 0,  0,  1 }
                            };

    double cy=cos(e.y); //cos of rotate_degrees
    double sy=sin(e.y);
    double matrix_y[4][4] = {
        { cy, 0, sy, 0 },
        { 0,  1, 0,  0 },
        {-sy, 0, cy, 0 },
        { 0,  0, 0,  1 },
    };

    double cz=cos(e.z); //cos of rotate_degrees
    double sz=sin(e.z);
    double matrix_z[4][4] = {
        { cz,-sz, 0, 0 },
        { sz, cz, 0, 0 },
        { 0,   0, 1, 0 },
        { 0,   0, 0, 1 }
    };

    double vector[4] = {0, 0, 0, 1}; //[x,y,z,w] //w=1 for translate, w=0 for direction
    double w=0;

    //rotate around z axis.
    vector[0]=p.x;
    vector[1]=p.y;
    vector[2]=p.z;
    p.x=(matrix_z[0][0]*vector[0])+(matrix_z[0][1]*vector[1])+(matrix_z[0][2]*vector[2])+(matrix_z[0][3]*vector[3]);
    p.y=(matrix_z[1][0]*vector[0])+(matrix_z[1][1]*vector[1])+(matrix_z[1][2]*vector[2])+(matrix_z[1][3]*vector[3]);
    p.z=(matrix_z[2][0]*vector[0])+(matrix_z[2][1]*vector[1])+(matrix_z[2][2]*vector[2])+(matrix_z[2][3]*vector[3]);
    w=(matrix_z[3][0]*vector[0])+(matrix_z[3][1]*vector[1])+(matrix_z[3][2]*vector[2])+(matrix_z[3][3]*vector[3]);

    //rotate around y axis.
    vector[0]=p.x;
    vector[1]=p.y;
    vector[2]=p.z;
    p.x=(matrix_y[0][0]*vector[0])+(matrix_y[0][1]*vector[1])+(matrix_y[0][2]*vector[2])+(matrix_y[0][3]*vector[3]);
    p.y=(matrix_y[1][0]*vector[0])+(matrix_y[1][1]*vector[1])+(matrix_y[1][2]*vector[2])+(matrix_y[1][3]*vector[3]);
    p.z=(matrix_y[2][0]*vector[0])+(matrix_y[2][1]*vector[1])+(matrix_y[2][2]*vector[2])+(matrix_y[2][3]*vector[3]);
    w=(matrix_y[3][0]*vector[0])+(matrix_y[3][1]*vector[1])+(matrix_y[3][2]*vector[2])+(matrix_y[3][3]*vector[3]);

    //rotate around x axis.
    vector[0]=p.x;
    vector[1]=p.y;
    vector[2]=p.z;
    p.x=(matrix_x[0][0]*vector[0])+(matrix_x[0][1]*vector[1])+(matrix_x[0][2]*vector[2])+(matrix_x[0][3]*vector[3]);
    p.y=(matrix_x[1][0]*vector[0])+(matrix_x[1][1]*vector[1])+(matrix_x[1][2]*vector[2])+(matrix_x[1][3]*vector[3]);
    p.z=(matrix_x[2][0]*vector[0])+(matrix_x[2][1]*vector[1])+(matrix_x[2][2]*vector[2])+(matrix_x[2][3]*vector[3]);
    w=(matrix_x[3][0]*vector[0])+(matrix_x[3][1]*vector[1])+(matrix_x[3][2]*vector[2])+(matrix_x[3][3]*vector[3]);

    return p;
}

POINT matrix_3d::mirror(POINT p, bool xy_plane, bool yz_plane, bool xz_plane){
    double vector[4] = {0, 0, 0, 1}; //[x,y,z,w] //w=1 for translate, w=0 for direction
    double x=0,y=0,z=0,w=0;

    if(xy_plane){
        double matrix_xy_plane[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                                         { 1, 0, 0, 0 },
                                         { 0, 1, 0, 0 },
                                         { 0, 0,-1, 0 },
                                         { 0, 0, 0, 1 }
                                       };
        //mirror around xy plane:
        vector[0]=p.x;
        vector[1]=p.y;
        vector[2]=p.z;
        x=(matrix_xy_plane[0][0]*vector[0])+(matrix_xy_plane[0][1]*vector[1])+(matrix_xy_plane[0][2]*vector[2])+(matrix_xy_plane[0][3]*vector[3]);
        y=(matrix_xy_plane[1][0]*vector[0])+(matrix_xy_plane[1][1]*vector[1])+(matrix_xy_plane[1][2]*vector[2])+(matrix_xy_plane[1][3]*vector[3]);
        z=(matrix_xy_plane[2][0]*vector[0])+(matrix_xy_plane[2][1]*vector[1])+(matrix_xy_plane[2][2]*vector[2])+(matrix_xy_plane[2][3]*vector[3]);
        w=(matrix_xy_plane[3][0]*vector[0])+(matrix_xy_plane[3][1]*vector[1])+(matrix_xy_plane[3][2]*vector[2])+(matrix_xy_plane[3][3]*vector[3]);
    }
    if(yz_plane){
        double matrix_yz_plane[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                                         {-1, 0, 0, 0 },
                                         { 0, 1, 0, 0 },
                                         { 0, 0, 1, 0 },
                                         { 0, 0, 0, 1 }
                                       };
        //mirror around yz plane:
        vector[0]=x;
        vector[1]=y;
        vector[2]=z;
        x=(matrix_yz_plane[0][0]*vector[0])+(matrix_yz_plane[0][1]*vector[1])+(matrix_yz_plane[0][2]*vector[2])+(matrix_yz_plane[0][3]*vector[3]);
        y=(matrix_yz_plane[1][0]*vector[0])+(matrix_yz_plane[1][1]*vector[1])+(matrix_yz_plane[1][2]*vector[2])+(matrix_yz_plane[1][3]*vector[3]);
        z=(matrix_yz_plane[2][0]*vector[0])+(matrix_yz_plane[2][1]*vector[1])+(matrix_yz_plane[2][2]*vector[2])+(matrix_yz_plane[2][3]*vector[3]);
        w=(matrix_yz_plane[3][0]*vector[0])+(matrix_yz_plane[3][1]*vector[1])+(matrix_yz_plane[3][2]*vector[2])+(matrix_yz_plane[3][3]*vector[3]);
    }
    if(xz_plane){
        double matrix_xz_plane[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                                         { 1, 0, 0, 0 },
                                         { 0,-1, 0, 0 },
                                         { 0, 0, 1, 0 },
                                         { 0, 0, 0, 1 }
                                       };
        //mirror around xz plane:
        vector[0]=x;
        vector[1]=y;
        vector[2]=z;
        x=(matrix_xz_plane[0][0]*vector[0])+(matrix_xz_plane[0][1]*vector[1])+(matrix_xz_plane[0][2]*vector[2])+(matrix_xz_plane[0][3]*vector[3]);
        y=(matrix_xz_plane[1][0]*vector[0])+(matrix_xz_plane[1][1]*vector[1])+(matrix_xz_plane[1][2]*vector[2])+(matrix_xz_plane[1][3]*vector[3]);
        z=(matrix_xz_plane[2][0]*vector[0])+(matrix_xz_plane[2][1]*vector[1])+(matrix_xz_plane[2][2]*vector[2])+(matrix_xz_plane[2][3]*vector[3]);
        w=(matrix_xz_plane[3][0]*vector[0])+(matrix_xz_plane[3][1]*vector[1])+(matrix_xz_plane[3][2]*vector[2])+(matrix_xz_plane[3][3]*vector[3]);
    }
    return p;
}

POINT matrix_3d::shearing(POINT p, double shearing_x, double shearing_y, double shearing_z, bool shearing_x_axis, bool shearing_y_axis, bool shearing_z_axis){

    double x=0; double y=0; double z=0; double w=0;
    double sx=shearing_x;
    double sy=shearing_y;
    double sz=shearing_z;

    if(shearing_x_axis){
        double matrix_x[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                                  { 1, 0, 0, 0 },
                                  { sy,1, 0, 0 },
                                  { sz,0, 1, 0 },
                                  { 0, 0, 0, 1 }
                                };

        double vector[4] = {0, 0, 0, 1}; //[x,y,z,w] //w=1 for translate, w=0 for direction
        //mirror around xy plane:
        vector[0]=p.x;
        vector[1]=p.y;
        vector[2]=p.z;
        x=(matrix_x[0][0]*vector[0])+(matrix_x[0][1]*vector[1])+(matrix_x[0][2]*vector[2])+(matrix_x[0][3]*vector[3]);
        y=(matrix_x[1][0]*vector[0])+(matrix_x[1][1]*vector[1])+(matrix_x[1][2]*vector[2])+(matrix_x[1][3]*vector[3]);
        z=(matrix_x[2][0]*vector[0])+(matrix_x[2][1]*vector[1])+(matrix_x[2][2]*vector[2])+(matrix_x[2][3]*vector[3]);
        w=(matrix_x[3][0]*vector[0])+(matrix_x[3][1]*vector[1])+(matrix_x[3][2]*vector[2])+(matrix_x[3][3]*vector[3]);
    }

    if(shearing_y_axis){
        double matrix_y[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                                  { 1, sx,0, 0 },
                                  { 0, 1, 0, 0 },
                                  { 0, sz,1, 0 },
                                  { 0, 0, 0, 1 }
                                };

        double vector[4] = {0, 0, 0, 1}; //[x,y,z,w] //w=1 for translate, w=0 for direction
        //mirror around xy plane:
        vector[0]=p.x;
        vector[1]=p.y;
        vector[2]=p.z;
        x=(matrix_y[0][0]*vector[0])+(matrix_y[0][1]*vector[1])+(matrix_y[0][2]*vector[2])+(matrix_y[0][3]*vector[3]);
        y=(matrix_y[1][0]*vector[0])+(matrix_y[1][1]*vector[1])+(matrix_y[1][2]*vector[2])+(matrix_y[1][3]*vector[3]);
        z=(matrix_y[2][0]*vector[0])+(matrix_y[2][1]*vector[1])+(matrix_y[2][2]*vector[2])+(matrix_y[2][3]*vector[3]);
        w=(matrix_y[3][0]*vector[0])+(matrix_y[3][1]*vector[1])+(matrix_y[3][2]*vector[2])+(matrix_y[3][3]*vector[3]);
    }

    if(shearing_z_axis){
        double matrix_z[4][4] = { //this matrix only calculates rotate_degrees into cos and sin value's
                                  { 1, 0 ,sx,0 },
                                  { 0, 1, sy,0 },
                                  { 0, 0 ,1, 0 },
                                  { 0, 0, 0, 1 }
                                };

        double vector[4] = {0, 0, 0, 1}; //[x,y,z,w] //w=1 for translate, w=0 for direction
        //mirror around xy plane:
        vector[0]=p.x;
        vector[1]=p.y;
        vector[2]=p.z;
        x=(matrix_z[0][0]*vector[0])+(matrix_z[0][1]*vector[1])+(matrix_z[0][2]*vector[2])+(matrix_z[0][3]*vector[3]);
        y=(matrix_z[1][0]*vector[0])+(matrix_z[1][1]*vector[1])+(matrix_z[1][2]*vector[2])+(matrix_z[1][3]*vector[3]);
        z=(matrix_z[2][0]*vector[0])+(matrix_z[2][1]*vector[1])+(matrix_z[2][2]*vector[2])+(matrix_z[2][3]*vector[3]);
        w=(matrix_z[3][0]*vector[0])+(matrix_z[3][1]*vector[1])+(matrix_z[3][2]*vector[2])+(matrix_z[3][3]*vector[3]);
    }

    POINT result;
    result.x=x;
    result.y=y;
    result.z=z;
    return result;
}


















