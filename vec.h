// Author: Tom Fiset

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
    vec2 operator*(float f) {
        return vec2(x*f,y*f);
    }
};

class vec3 {
    public:
        vec3();
        vec3(float a_x, float a_y, float a_z);
        vec3(const vec3& v);
        vec3(const vec2& v, float a_z);
    private:
        void init(float a_x, float a_y, float a_z);
    public:
        vec3  operator-(const vec3& v);
        vec3  operator+(const vec3& v);

        vec3  operator*(float f);
        vec3  operator/(float f);

        float operator*(const vec3& v);
        vec3  operator^(const vec3& v);

        vec3& operator-=(const vec3& v);
        vec3& operator+=(const vec3& v);
        vec3& operator*=(float f);
        vec3& operator/=(float f);
        
        vec2 xy();
        void normalize();
        void print(const char* prefix);

        float x,y,z;
};

#endif

