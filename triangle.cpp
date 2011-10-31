#include <GL/glew.h>
#include <GL/glut.h>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include "shader.h"
#include "vec.h"

using namespace std;

#define ESCAPE 27
#define RADIUS 20
#define WIDTH  400
#define HEIGHT 400



// a triangle vertex
struct vertex {
    vec2 p; // position
    vec3 c; // color
};


void reshape(int w, int h);
void shaderDisplay(void);
void normalDisplay(void);
void keyDown(unsigned char,int,int);
void mousePress(int button, int state, int x, int y);
void mouseDrag(int x, int y);

Shader* f_shader;
static vertex a,b,c;
static bool DRAG_A         = false;
static bool DRAG_B         = false;
static bool DRAG_C         = false;
static bool SHADER_SUPPORT = false;
static int  W_WIDTH        = 400;
static int  W_HEIGHT       = 400;

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
    if(SHADER_SUPPORT) {
        printf("Using shader display.\n");
        glutDisplayFunc(shaderDisplay);
    } else {
        printf("Using normal display.\n");
        glutDisplayFunc(normalDisplay);
    }
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown);
    glutMouseFunc(mousePress);
    glutMotionFunc(mouseDrag);
}

int main(int argc, char** argv) {
    printf("Barycentric Coordinate Demo\n");
    glutInit(&argc,argv);
 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(WIDTH,HEIGHT); 
    
    centerWindow(WIDTH,HEIGHT);

    glutCreateWindow("Barycentric Coordinates");

    glShadeModel(GL_FLAT);
    //glDisable(GL_DEPTH_TEST); 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glewInit();

    // make sure GLSL is supported
    if(!GLEW_ARB_vertex_shader ||
       !GLEW_ARB_fragment_shader) {
        printf("ERROR: GLSL not supported.\n");
        SHADER_SUPPORT = false;
        glShadeModel(GL_SMOOTH);
    } else {
        SHADER_SUPPORT = true;

        f_shader = new Shader();

        f_shader->load("shader.vert",GL_VERTEX_SHADER);
        f_shader->load("shader.frag",GL_FRAGMENT_SHADER);
        f_shader->link();
    }

    a.p = vec2(200,100);
    b.p = vec2(100,300);
    c.p = vec2(300,300);

    a.c = vec3(1,0,0);
    b.c = vec3(0,1,0);
    c.c = vec3(0,0,1);

    registerCallbacks();
            
    glutMainLoop();
}

void reshape(int w, int h) {
    printf("Reshape (%i,%i)\n",w,h);
    W_WIDTH = w;
    W_HEIGHT= h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,WIDTH, HEIGHT, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void drawCircle(float x, float y, float r) {
    glBegin(GL_LINE_LOOP);
    for(float angle=0; angle < 6.28; angle+=.1)
        glVertex2f(x+sin(angle)*r,y+cos(angle)*r);
    glEnd();
}

void shaderDisplay(void) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    f_shader->bind();
    
    // set uniforms
    f_shader->uniform2f("avert", a.p.x, a.p.y);
    f_shader->uniform2f("bvert", b.p.x, b.p.y);
    f_shader->uniform2f("cvert", c.p.x, c.p.y);
    f_shader->uniform3f("acol", a.c.x, a.c.y, a.c.z);
    f_shader->uniform3f("bcol", b.c.x, b.c.y, b.c.z);
    f_shader->uniform3f("ccol", c.c.x, c.c.y, c.c.z);

    glBegin(GL_TRIANGLES);
    glVertex2f(a.p.x,a.p.y);
    glVertex2f(b.p.x,b.p.y);
    glVertex2f(c.p.x,c.p.y);
    glEnd();
    f_shader->unbind();

    glutSwapBuffers();
}

void normalDisplay(void) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1.0,1.0,1.0);
    drawCircle(a.p.x,a.p.y,RADIUS);
    drawCircle(b.p.x,b.p.y,RADIUS);
    drawCircle(c.p.x,c.p.y,RADIUS);
 
    glColor3f(1.0,1.0,1.0); 
    glBegin(GL_TRIANGLES);
    glColor3f(a.c.x,a.c.y,a.c.z);
    glVertex2f(a.p.x,a.p.y);
    glColor3f(b.c.x,b.c.y,b.c.z);
    glVertex2f(b.p.x,b.p.y);
    glColor3f(c.c.x,c.c.y,c.c.z);
    glVertex2f(c.p.x,c.p.y);
    glEnd();

    glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y) {
    if(key == 27) exit(0);
}


vec2 offset;
void mousePress(int button, int state, int x, int y) {
    // translate mouse_coord into world
    // coordinates
    vec2 mouse_coord(x*WIDTH/W_WIDTH,
                     y*HEIGHT/W_HEIGHT);
    
    vec2 am = a.p - mouse_coord;
    vec2 bm = b.p - mouse_coord;
    vec2 cm = c.p - mouse_coord;

    float dista = sqrt(am*am);
    float distb = sqrt(bm*bm);
    float distc = sqrt(cm*cm);

    if(button == GLUT_LEFT_BUTTON)
        if(state  == GLUT_DOWN) {
            if(dista < RADIUS) {
                offset = am;
                DRAG_A = true;
                return;
            }
            if(distb < RADIUS) {
                offset = bm;
                DRAG_B = true;
                return;
            }
            if(distc < RADIUS) {
                offset = cm;
                DRAG_C = true;
                return;
            }
        } else {
            DRAG_A = false;
            DRAG_B = false;
            DRAG_C = false;
        }
}

void mouseDrag(int x, int y) {
    vec2 drag_coord(x*WIDTH/W_WIDTH,
                    y*HEIGHT/W_HEIGHT);
    if(DRAG_A) a.p = drag_coord + offset;
    if(DRAG_B) b.p = drag_coord + offset;
    if(DRAG_C) c.p = drag_coord + offset;
    glutPostRedisplay();
}
