// Author: Tom Fiset

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

void Quaternion::getRotationMatrix(float f[16]) {
    float x =  this->y;
    float y = -this->x;
    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;

    f[0]  = 1.0f - 2.0f * (y2 + z2);
    f[1]  = 2.0f * (xy - wz);
    f[2]  = 2.0f * (xz + wy);
    f[3]  = 0.0f;
    f[4]  = 2.0f * (xy + wz);
    f[5]  = 1.0f - 2.0f * (x2 + z2);
    f[6]  = 2.0f * (yz - wx);
    f[7]  = 0.0f;
    f[8]  = 2.0f * (xz - wy);
    f[9]  = 2.0f * (yz + wx);
    f[10] = 1.0f - 2.0f * (x2 + y2);
    f[11] = 0.0f;
    f[12] = 0.0f;
    f[13] = 0.0f;
    f[14] = 0.0f;
    f[15] = 1.0f;
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
