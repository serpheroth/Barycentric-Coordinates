// Author: Tom Fiset

#include <GL/glew.h>
#include <GL/glut.h>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#include "shader.h"
#include "vec.h"
#include "vertex.h"
#include "loader.h"

using namespace std;

#define ESCAPE 27
#define RADIUS 10
#define WIDTH  400
#define HEIGHT 400

void reshape(int w, int h);
void shaderDisplay(void);
void normalDisplay(void);
void keyDown(unsigned char,int,int);
void mousePress(int button, int state, int x, int y);
void mouseDrag(int x, int y);

Shader* f_shader;
static bool DRAG_A         = false;
static bool DRAG_B         = false;
static bool DRAG_C         = false;
static bool SHADER_SUPPORT = false;
static int  W_WIDTH        = 400;
static int  W_HEIGHT       = 400;
static vector<Vertex> vertices;
static Vertex a,b,c;

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

    if(argc < 2) {
        printf("Indicate a file please.\n");
        return 0;
    } 

    glutInit(&argc,argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH,HEIGHT); 
    centerWindow(WIDTH,HEIGHT);

    glutCreateWindow("Barycentric Coordinates");

    glShadeModel(GL_FLAT);
    glDisable(GL_DEPTH_TEST); 
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#ifdef GLEW_SUPPORT
    printf("GLEW is supported.\n");
    glewInit();

    // make sure GLSL is supported
    if(GLEW_ARB_vertex_shader &&
       GLEW_ARB_fragment_shader) {
        SHADER_SUPPORT = true;

        f_shader = new Shader();

        f_shader->load("shader.frag",GL_FRAGMENT_SHADER);
        f_shader->link(); 
    }
    else
#endif
    {
        printf("ERROR: GLSL not supported.\n");
        SHADER_SUPPORT = false;
        glShadeModel(GL_SMOOTH); 
    }

    printf("Loading vertex data...\n");
    Loader::loadVertices(vertices,argv[1]);
    //for(int i=0; i<vertices.size(); i++)
    //    vertices[i].print();

    if(vertices.size() >= 3) {
        a = vertices[0];
        b = vertices[1];
        c = vertices[2];
    } else {
        printf("Using preset values for a,b,c!\n");
        a.pos = vec3(10,10,0);
        b.pos = vec3(20,20,0);
        c.pos = vec3(20,10,0);

        a.color = vec3(1,0,0);
        b.color = vec3(0,1,0);
        c.color = vec3(0,0,1);
    }

    registerCallbacks();
            
    glutMainLoop();
}

void reshape(int w, int h) {
    W_WIDTH = w;
    W_HEIGHT= h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,WIDTH, HEIGHT, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void drawCircle(vec2 v, float r) {
    glBegin(GL_LINE_LOOP);
    for(float angle=0; angle < 6.28; angle+=.1)
        glVertex2f(v.x+sin(angle)*r,v.y+cos(angle)*r);
    glEnd();
}

void shaderDisplay(void) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
#ifdef GLEW_SUPPORT
    f_shader->bind();

    glColor3f(0.5,0.5,0.5);
    drawCircle(a.pos.xy(),RADIUS);
    drawCircle(b.pos.xy(),RADIUS);
    drawCircle(c.pos.xy(),RADIUS);
   
    // set uniforms
    f_shader->uniform2f("v[0].p",a.pos.x,a.pos.y);
    f_shader->uniform2f("v[1].p",b.pos.x,b.pos.y);
    f_shader->uniform2f("v[2].p",c.pos.x,c.pos.y);
    f_shader->uniform3f("v[0].c",a.color.x,a.color.y,a.color.z);
    f_shader->uniform3f("v[1].c",b.color.x,b.color.y,b.color.z);
    f_shader->uniform3f("v[2].c",c.color.x,c.color.y,c.color.z);

    f_shader->uniform2f("windim",W_WIDTH,W_HEIGHT);
    f_shader->uniform2f("realdim",WIDTH,HEIGHT);

#endif
    glBegin(GL_TRIANGLES);
    for(int i=0; i<vertices.size(); i++)
        glVertex3f(vertices[i].pos.x,vertices[i].pos.y,vertices[i].pos.z);
    glEnd();

#ifdef GLEW_SUPPORT
    f_shader->unbind();
#endif

    glutSwapBuffers();
}

void normalDisplay(void) {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(0.5,0.5,0.5);
    drawCircle(a.pos.xy(),RADIUS);
    drawCircle(b.pos.xy(),RADIUS);
    drawCircle(c.pos.xy(),RADIUS);
 
    glColor3f(1.0,1.0,1.0); 
    glBegin(GL_TRIANGLES);
    glColor3f(a.color.x,a.color.y,a.color.z);
    glVertex2f(a.pos.x,a.pos.y);
    glColor3f(b.color.x,b.color.y,b.color.z);
    glVertex2f(b.pos.x,b.pos.y);
    glColor3f(c.color.x,c.color.y,c.color.z);
    glVertex2f(c.pos.x,c.pos.y);
    glEnd();

    glColor3f(1.0,1.0,1.0);
    glBegin(GL_LINE_LOOP);
    for(int i=0; i<vertices.size(); i++)
        glVertex3f(vertices[i].pos.x,vertices[i].pos.y,vertices[i].pos.z);
    glEnd();

    glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y) {
    if(key == 27) exit(0);
}


vec3 offset;
void mousePress(int button, int state, int x, int y) {
    // translate mouse_coord into world
    // coordinates
    vec3 mouse_coord(x*WIDTH/W_WIDTH,
                     y*HEIGHT/W_HEIGHT,0);
    
    vec3 am = a.pos - mouse_coord;
    vec3 bm = b.pos - mouse_coord;
    vec3 cm = c.pos - mouse_coord;

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
    vec3 drag_coord(x*WIDTH/W_WIDTH,
                    y*HEIGHT/W_HEIGHT,0);
    if(DRAG_A) a.pos = drag_coord + offset;
    if(DRAG_B) b.pos = drag_coord + offset;
    if(DRAG_C) c.pos = drag_coord + offset;
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
    glutPostRedisplay();
}
