#ifndef MATRIX_3D_H
#define MATRIX_3D_H

#include "external_var.h"
#include "iostream"

class matrix_3d
{
public:
    matrix_3d();

    int mult(double A[4][4], double B[4][4]);

    POINT translate(POINT p, double x_translate, double y_translate, double z_translate);
    POINT scale(POINT p, double scale_x, double scale_y, double scale_z);
    POINT rotate(POINT p, EULER e, bool unit_degrees);
    POINT mirror(POINT p, bool xy_plane, bool yz_plane, bool xz_plane);
    POINT shearing(POINT p, double shearing_x, double shearing_y, double shearing_z, bool shearing_x_axis, bool shearing_y_axis, bool shearing_z_axis);

};

#endif // MATRIX_3D_H
