// Author: Tom Fiset

#ifndef VERTEX_H
#define VERTEX_H

#include <cstdio>

#include "vec.h"

struct Vertex {
    vec3 pos;
    vec3 color;

    Vertex() {}

    Vertex(const Vertex& v)
        : pos(v.pos), color(v.color) {}

    void print() {
        printf(
            "    %f %f %f %f %f %f\n",
            pos.x,pos.y,pos.z,
            color.x,color.y,color.z);
    }
};

#endif
