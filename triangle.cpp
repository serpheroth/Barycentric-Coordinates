#include <GL/glew.h>
#include <GL/glut.h>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "shader.h"

using namespace std;

struct vec2 {
    float x,y;
    vec2() : x(0), y(0) {}
    vec2(float a_x, float a_y) : x(a_x), y(a_y) {}

    const vec2 operator*(float f) {
        return vec2(x*f,y*f);
    }
    const vec2 operator+(const vec2& v) {
        return vec2(x+v.x,y+v.y);
    }
};

struct triangle {
    vec2 a,b,c;
};


void shaderDisplay(void);
void normalDisplay(void);
void keyDown(unsigned char,int,int);

Shader f_shader(GL_FRAGMENT_SHADER);
triangle tri;

void centerWindow(int w_width, int w_height) {
    // center window in screen
    int win_pos_x = 0;
    int win_pos_y = 0;
    int screen_width  = glutGet(GLUT_SCREEN_WIDTH);
    int screen_height = glutGet(GLUT_SCREEN_HEIGHT);

    if(screen_width > 0 && screen_height > 0) {
        printf("    screen size: %i x %i\n",screen_width,screen_height);
        win_pos_x = screen_width/2  - w_width/2;
        win_pos_y = screen_height/2 - w_height/2;
    }
    
    printf("    placing window at (%i,%i)\n",win_pos_x,win_pos_y);
    glutInitWindowPosition(win_pos_x,win_pos_y);
}

void registerCallbacks() {
    if(f_shader.ok()) {
        printf("Using shader display.\n");
        glutDisplayFunc(shaderDisplay);
    } else {
        printf("Using normal display.\n");
        glutDisplayFunc(normalDisplay);
    }
    glutKeyboardFunc(keyDown);
}

int main(int argc, char** argv) {
    printf("Barycentric Coordinate Demo\n");
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(400,400); 
    centerWindow(400,400);

    glutCreateWindow("Barycentric Coordinates");

    glShadeModel(GL_FLAT);

    f_shader.load("shader.frag");
    registerCallbacks();

    tri.a = vec2(0,0);
    tri.b = vec2(50,50);
    tri.c = vec2(100,0);

    glewInit();
    
    glutMainLoop();
}

void shaderDisplay(void) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    
    glutSwapBuffers();
}

void normalDisplay(void) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    
    glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y) {
    if(key == 27) exit(0);
}
