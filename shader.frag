// Author: Tom Fiset

#version 150

struct vertex {
    vec2 p;
    vec3 c;
};

uniform vertex v[3];

uniform vec2 windim;
uniform vec2 realdim;

// calculate barycentric coordinates
vec3 barycentric(in vec2 p, in vec2 a, in vec2 b, in vec2 c) {
    mat2 m = mat2( a-c, b-c );
    
    vec3 bary = vec3(inverse(m)*(p-c),1.0);
    bary[2] = 1 - bary[0] - bary[1];
    
    return bary;
}

void main() {
    // first convert gl_FragCoord into something we can use
    vec2 pos = gl_FragCoord.xy*realdim/windim;
    pos.y = realdim.y - pos.y;

    vec3 l = barycentric(pos,v[0].p,v[1].p,v[2].p);

    gl_FragColor.rgba = vec4(l.x*v[0].c + l.y*v[1].c + l.z*v[2].c,1);
}
