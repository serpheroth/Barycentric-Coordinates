// Author: Tom Fiset
#ifndef QUATERNION_H
#define QUATERNION_H

class Quaternion {
    public:
        Quaternion();
        Quaternion(float a_x, float a_y, float a_z, float a_w);
    private:
        void init(float a_x, float a_y, float a_z, float a_w);

    public:
        Quaternion operator*(const Quaternion& q);
        
        static Quaternion fromAxisAngle(float axis_x, float axis_y, float axis_z, float angle);

    public:
        float x,y,z,w;
};

#endif
