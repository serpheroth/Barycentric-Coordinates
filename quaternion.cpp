#include "quaternion.h"

#include <math.h>

Quaternion::Quaternion() {
    init(0,0,0,0);
}

Quaternion::Quaternion(float a_x, float a_y, float a_z, float a_w) {
    init(a_x,a_y,a_z,a_w);
}

void Quaternion::init(float a_x, float a_y, float a_z, float a_w) {
    x = a_x;
    y = a_y;
    z = a_z;
    w = a_w;
}

Quaternion Quaternion::operator*(const Quaternion& q) {
    Quaternion result(
        w*q.x + x*q.w + y*q.z - z*q.y,
        w*q.y - x*q.z + y*q.w + z*q.x,
        w*q.z + x*q.y - y*q.x + z*q.w,
        w*q.w - x*q.x - y*q.y - z*q.z
    );

    return result;
}

Quaternion Quaternion::fromAxisAngle(float axis_x, float axis_y, float axis_z, float angle) {
    float sa2 = sin(angle/2.0);
    float ca2 = cos(angle/2.0);
    Quaternion result(
        axis_x*sa2,
        axis_y*sa2,
        axis_z*sa2,
        ca2
    );
    return result;
}
