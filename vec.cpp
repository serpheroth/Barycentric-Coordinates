#include "vec.h"
#include <math.h>
#include <cstdio>

vec3::vec3() {
    init(0,0,0);
}
vec3::vec3(float a_x, float a_y, float a_z) {
    init(a_x,a_y,a_z);
}
vec3::vec3(const vec3& v) {
    init(v.x,v.y,v.z);
}
vec3::vec3(const vec2& v, float a_z) {
    init(v.x,v.y,a_z);
}
void vec3::init(float a_x, float a_y, float a_z) {
    x = a_x;
    y = a_y;
    z = a_z;
}

vec3 vec3::operator-(const vec3& v) {
    return vec3(x - v.x, y - v.y, z - v.z);
}
vec3 vec3::operator+(const vec3& v) {
    return vec3(x+v.x,y+v.y,z+v.z);
}

vec3 vec3::operator*(float f) {
    return vec3(x*f,y*f,z*f);
}
vec3 vec3::operator/(float f) {
    return vec3(x/f,y/f,z/f);
}

float vec3::operator*(const vec3& v) {
    return x*v.x + y*v.y + z*v.z;
}
vec3 vec3::operator^(const vec3& v) {
    vec3 result;
    result.x = y*v.z - z*v.y;
    result.y = z*v.x - x*v.z;
    result.z = x*v.y - y*v.x;
    return result;
}

vec3& vec3::operator-=(const vec3& v) {
    x -= v.x;
    y -= v.y;
    x -= v.z;
    return *this;
}
vec3& vec3::operator+=(const vec3& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}
vec3& vec3::operator*=(float f) {
    x *= f;
    y *= f;
    z *= f;
    return *this;
}
vec3& vec3::operator/=(float f) {
    x /= f;
    y /= f;
    z /= f;
}

vec2 vec3::xy() {
    return vec2(x,y);
}
void vec3::normalize() {
    vec3& v = *this;
    v /= sqrt(v*v);
}
void vec3::print(const char* prefix) {
    printf("(%f,%f,%f)\n",x,y,z);
}
