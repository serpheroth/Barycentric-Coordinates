#ifndef VEC_H
#define VEC_H

struct vec2 {
    float x,y;
    vec2()
        : x(0), y(0) {}
    vec2(float a_x, float a_y)
        : x(a_x), y(a_y) {}
    vec2 operator-(const vec2& v) {
        return vec2(x - v.x, y - v.y);
    }
    vec2 operator+(const vec2& v) {
        return vec2(x+v.x,y+v.y);
    }
    float operator*(const vec2& v) {
        return x*v.x + y*v.y;
    }
};

struct vec3 {
    float x,y,z;
    vec3()
        : x(0), y(0), z(0) {}
    vec3(float a_x, float a_y, float a_z)
        : x(a_x), y(a_y), z(a_z) {}
};

#endif

