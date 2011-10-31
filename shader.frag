#version 150

in vec2 apos;
in vec2 bpos;
in vec2 cpos;

uniform vec3 acol;
uniform vec3 bcol;
uniform vec3 ccol;

void main() {
    //if(apos.x < 0)
    //    gl_FragColor = vec4(1,1,1,1);
    //return;
    mat2 m = mat2( apos-cpos, bpos-cpos );
    vec2 pos = gl_FragCoord.xy;

    vec3 lambda = vec3(inverse(m)*(pos-cpos),1.0);
    lambda[2] = 1 - lambda[0] - lambda[1]; 

    gl_FragColor = vec4(lambda[0]*acol +
                        lambda[1]*bcol +
                        lambda[2]*ccol,1);
}
