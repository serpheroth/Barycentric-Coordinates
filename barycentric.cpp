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
#ifdef GLEW_SUPPORT
    glewInit();
    if(GLEW_ARB_fragment_shader) {
        printf("Using pixel shader!\n");
        m_shader = new Shader();
        m_shader->load("baryshader.frag",GL_FRAGMENT_SHADER);
        m_shader->link();
    } else {
        printf("Shaders not supported!\n");
    }
#endif
}

Barycentric::Barycentric(const char* f_name) {
    Loader::loadVertices(m_vertices,f_name);
    if(m_vertices.size() < 3) {
        printf("Not enough vertices in file: %s\n",f_name);
        exit(0);
    }
    setInst(this);
#ifdef GLEW_SUPPORT
    glewInit();
    if(GLEW_ARB_fragment_shader) {
        printf("Using pixel shader!\n");
        m_shader = new Shader();
        m_shader->load("baryshader.frag",GL_FRAGMENT_SHADER);
        m_shader->link();
    } else {
        printf("Shaders not supported!\n");
    }
#endif
}

void Barycentric::display() {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    //gluLookAt(100,100,300,
    //          0,0,0,
    //          0,1,0);

    Vertex& a = m_vertices[0];
    Vertex& b = m_vertices[1];
    Vertex& c = m_vertices[2];

    if(m_vertices.size() <= 3) {
        glColor3f(0.5,0.5,0.5);
        drawCircle(m_vertices[0].pos.xy(),RADIUS);
        drawCircle(m_vertices[1].pos.xy(),RADIUS);
        drawCircle(m_vertices[2].pos.xy(),RADIUS);
    }

#ifdef GLEW_SUPPORT
    if(GLEW_ARB_fragment_shader) {
        m_shader->bind();
        m_shader->uniform2f("v[0].p",a.pos.x,a.pos.y);
        m_shader->uniform2f("v[1].p",b.pos.x,b.pos.y);
        m_shader->uniform2f("v[2].p",c.pos.x,c.pos.y);
        m_shader->uniform3f("v[0].c",a.color.x,a.color.y,a.color.z);
        m_shader->uniform3f("v[1].c",b.color.x,b.color.y,b.color.z);
        m_shader->uniform3f("v[2].c",c.color.x,c.color.y,c.color.z);

        m_shader->uniform2f("windim",windowDim().x,windowDim().y);
        m_shader->uniform2f("realdim",600,600);
    }
#endif
 
    glColor3f(1.0,1.0,1.0); 
    glBegin(GL_TRIANGLES);
    for(int i=0; i<m_vertices.size(); i++) {
        Vertex& v = m_vertices[i];
        if(!m_shader)
            glColor3f(v.color.x,v.color.y,v.color.z);
        glVertex3f(v.pos.x,v.pos.y,v.pos.z);
    }
    glEnd();

#ifdef GLEW_SUPPORT
    if(GLEW_ARB_fragment_shader)
        m_shader->unbind();
#endif
   
    if(m_wireframe) { 
        glColor3f(0.5,0.5,0.5);
        for(int i=0; i<m_vertices.size()/3; i++) {
            glBegin(GL_LINE_LOOP);
            glVertex3f(m_vertices[i*3+0].pos.x,m_vertices[i*3+0].pos.y,m_vertices[i*3+0].pos.z);
            glVertex3f(m_vertices[i*3+1].pos.x,m_vertices[i*3+1].pos.y,m_vertices[i*3+1].pos.z);
            glVertex3f(m_vertices[i*3+2].pos.x,m_vertices[i*3+2].pos.y,m_vertices[i*3+2].pos.z);
            glEnd();
        }
    }

    glutSwapBuffers();
}

void Barycentric::reshape(int width, int height) {
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300,300, -300, 300, -600, 600);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void Barycentric::keyDown(unsigned char c, int x, int y) {
    if(c==27) exit(0);
    if(c=='w') {
        m_wireframe = !m_wireframe;
        glutPostRedisplay();
    }
}

void Barycentric::mouseAction(int button, int state, int x, int y) {
    if(button != GLUT_LEFT_BUTTON) return;
    if(state == GLUT_UP) return;

    vec2 mouse_coord(x*600/windowDim().x-300,300-y*600/windowDim().y);
 
    int distsqrd = INT_MAX;
    int index = -1;
    for(int i=0; i<m_vertices.size(); i++) {
        vec2 vm = m_vertices[i].pos.xy() - mouse_coord;
        float temp = vm*vm;
        if(temp < RADIUS*RADIUS) {
            distsqrd = temp;
            index = i; 
        }
    }

    m_drag_index = index;

    if(index == -1) return;

    m_drag_offset = m_vertices[index].pos.xy() - mouse_coord;
}

void Barycentric::mouseDrag(int x, int y) {
    vec2 mouse_coord(x*600/windowDim().x-300,300-y*600/windowDim().y);
    if(m_drag_index >= 0) {
        Vertex& v = m_vertices[m_drag_index];
        v.pos = vec3(mouse_coord+m_drag_offset,v.pos.z);
    }

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

    GlutWrapper::initGlut(
        &argc,argv,vec2(600,600),"Barycentric Coordinates Demo");

    Barycentric* b = new Barycentric(argv[1]);

    GlutWrapper::start();
}
