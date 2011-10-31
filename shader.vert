#version 120

uniform vec2 avert;
uniform vec2 bvert;
uniform vec2 cvert;

varying vec2 apos;
varying vec2 bpos;
varying vec2 cpos;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
    apos = (gl_ProjectionMatrix*vec4(avert,1,1)).xy;
    bpos = (gl_ProjectionMatrix*vec4(bvert,1,1)).xy;
    cpos = (gl_ProjectionMatrix*vec4(cvert,1,1)).xy;
    //apos = avert;
    //bpos = bvert;
    //cpos = cvert;
}
