// Author: Tom Fiset

#include "barycentric.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <limits.h>

#include "loader.h"

#define RADIUS 10

Barycentric::Barycentric() {
    m_vertices.push_back(Vertex(vec3(   0, 150,0),vec3(1,0,0)));
    m_vertices.push_back(Vertex(vec3( 150,-150,0),vec3(0,1,0)));
    m_vertices.push_back(Vertex(vec3(-150,-150,0),vec3(0,0,1)));
    setInst(this);
}

Barycentric::Barycentric(const char* f_name) {
    Loader::loadVertices(m_vertices,f_name);
    if(m_vertices.size() < 3) {
        printf("Not enough vertices in file: %s\n",f_name);
        exit(0);
    }
    setInst(this);
}

void Barycentric::display() {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    gluLookAt(100,100,300,
              0,0,0,
              0,1,0);

    if(m_vertices.size() <= 3) {
        glColor3f(0.5,0.5,0.5);
        drawCircle(m_vertices[0].pos.xy(),RADIUS);
        drawCircle(m_vertices[1].pos.xy(),RADIUS);
        drawCircle(m_vertices[2].pos.xy(),RADIUS);
    }
 
    glColor3f(1.0,1.0,1.0); 
    glBegin(GL_TRIANGLES);
    for(int i=0; i<m_vertices.size(); i++) {
        Vertex& v = m_vertices[i];
        glColor3f(v.color.x,v.color.y,v.color.z);
        glVertex3f(v.pos.x,v.pos.y,v.pos.z);
    }
    glEnd();
    
    glColor3f(0.5,0.5,0.5);
    for(int i=0; i<m_vertices.size()/3; i++) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(m_vertices[i*3+0].pos.x,m_vertices[i*3+0].pos.y,m_vertices[i*3+0].pos.z);
        glVertex3f(m_vertices[i*3+1].pos.x,m_vertices[i*3+1].pos.y,m_vertices[i*3+1].pos.z);
        glVertex3f(m_vertices[i*3+2].pos.x,m_vertices[i*3+2].pos.y,m_vertices[i*3+2].pos.z);
        glEnd();
    }

    glutSwapBuffers();
}

float front = 600.0;
float back = -1.0;
void Barycentric::reshape(int width, int height) {
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300,300, -300, 300, front, back);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void Barycentric::keyDown(unsigned char c, int x, int y) {
    if(c==27) exit(0);
    if(c=='q') front += 5;
    if(c=='a') front -= 5;
    if(c=='w') back  += 5;
    if(c=='s') back -= 5;
    printf("front: %f back: %f\n",front,back);
    reshape(windowDim().x,windowDim().y);
}

void Barycentric::mouseAction(int button, int state, int x, int y) {
    if(button != GLUT_LEFT_BUTTON) return;

    vec2 mouse_coord(x*windowDim().x/600-300,300-y*windowDim().y/600);
    
    int distsqrd = INT_MAX;
    int index = -1;
    for(int i=0; i<m_vertices.size(); i++) {
        vec2 vm = m_vertices[i].pos.xy() - mouse_coord;
        int temp = vm*vm;
        if(temp < RADIUS*RADIUS) {
            distsqrd = temp;
            index = i; 
        }
    }

    m_drag_offset = m_vertices[index].pos.xy() - mouse_coord;
    m_drag_index  = index;
}

void Barycentric::mouseDrag(int x, int y) {
    vec2 mouse_coord(x*windowDim().x/600-300,300-y*windowDim().y/600);
    if(m_drag_index >= 0)
        m_vertices[m_drag_index].pos = vec3(mouse_coord + m_drag_offset,0);

    glutPostRedisplay();
}

void Barycentric::drawCircle(vec2 v, float r, float p) {
    glBegin(GL_LINE_LOOP);
    for(float angle=0; angle < 6.28; angle+=p)
        glVertex2f(v.x+sin(angle)*r,v.y+cos(angle)*r);
    glEnd();
}

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("Please specify a file.\n");
        exit(0);
    }
    Barycentric* b = new Barycentric(argv[1]);

    GlutWrapper::initGlut(
        &argc,argv,vec2(600,600),"Barycentric Coordinates Demo");
}
