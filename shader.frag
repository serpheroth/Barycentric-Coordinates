#version 150

uniform vec2 apos;
uniform vec2 bpos;
uniform vec2 cpos;

uniform vec3 acol;
uniform vec3 bcol;
uniform vec3 ccol;

uniform vec2 windim;
uniform vec2 realdim;

void main() {
    mat2 m = mat2( apos-cpos, bpos-cpos );
    vec2 pos = vec2(gl_FragCoord.x*realdim.x/windim.x
                    gl_FragCoord.y*realdim.x/windim.y);

    vec3 lambda = vec3(inverse(m)*(pos-cpos),1.0);
    lambda[2] = 1 - lambda[0] - lambda[1]; 

    gl_FragColor = vec4(lambda[0]*acol +
                        lambda[1]*bcol +
                        lambda[2]*ccol,1);
}
