// Author: Tom Fiset

#include "model.h"

#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <limits.h>
#include <cstdlib>

#include "loader.h"

#define RADIUS 10

Model::Model() {
    init(0);
}
Model::Model(const char* f_name) {
    init(f_name);
}
void Model::init(const char* f_name) {
    m_show_wireframe = false;
    m_show_normals   = false;

    m_rot_last = Quaternion(0,0,0,1);
    m_rot = Quaternion(0,0,0,1);
    if(f_name) {
        Loader::loadVertices(m_vertices,f_name);
        if(m_vertices.size() == 0 ||
           m_vertices.size() % 3 != 0) {
            printf("Invalid file.\n");
            exit(0);
        }
        // calculate normals
        for(int i=0; i<m_vertices.size(); i+=3) {
            // (V1-V0)x(V2-V0)
            m_normals.push_back(
                (m_vertices[i+1].pos-m_vertices[i+0].pos)^
                (m_vertices[i+2].pos-m_vertices[i+0].pos));
            m_normals.back().normalize();
        }
    }
    setInst(this);
}

inline void glVec3(const vec3& v) {
    glVertex3f(v.x,v.y,v.z);
}

void Model::display() {
    glClearColor(0.0,0.0,0.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat lightPosition[] = { 0.0f, 3.0f, 300.0f, 1.0f };
    glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);

    Quaternion q = m_rot_last*m_rot;
    float x = q.y;
    float y = -q.x;
    float z = q.z;
    float w = q.w;
    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;
    float xy = x * y;
    float xz = x * z;
    float yz = y * z;
    float wx = w * x;
    float wy = w * y;
    float wz = w * z;
 
    float f[] = { 1.0f - 2.0f * (y2 + z2), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
            2.0f * (xy + wz), 1.0f - 2.0f * (x2 + z2), 2.0f * (yz - wx), 0.0f,
            2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (x2 + y2), 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f};

    glMatrixMode(GL_MODELVIEW);
    glMultMatrixf(f);

    if(!m_show_wireframe) {
        glEnable(GL_LIGHTING);
        glColor3f(0.5,0.5,0.5); 
        for(int i=0; i<m_vertices.size(); i+=3) {
            vec3& a = m_vertices[i+0].pos;
            vec3& b = m_vertices[i+1].pos;
            vec3& c = m_vertices[i+2].pos;
            vec3& n = m_normals[(int)(i/3)];
            glNormal3f(n.x,n.y,n.z);
            glBegin(GL_POLYGON);
            glVec3(a);
            glVec3(b);
            glVec3(c);
            glEnd();
        }
    }

    if(m_show_wireframe) {
        glDisable(GL_LIGHTING);
        for(int i=0; i<m_vertices.size(); i+=3) {
            glColor3f(0.3,0.3,0.3);
            glBegin(GL_LINE_LOOP);
            glVec3(m_vertices[i+0].pos);
            glVec3(m_vertices[i+1].pos);
            glVec3(m_vertices[i+2].pos);
            glEnd();
        }
    }

    if(m_show_normals) {
        glDisable(GL_LIGHTING);
        for(int i=0; i<m_vertices.size(); i+=3) {
            glColor3f(0.5,0.5,0.5);
            glBegin(GL_LINE_LOOP);
            vec3 n = m_normals[(int)(i/3)];
            vec3 pointA = (m_vertices[i+0].pos + m_vertices[i+1].pos + m_vertices[i+2].pos)/3;
            vec3 pointB = pointA + (n*15);
            glVec3(pointA);
            glVec3(pointB);
            glEnd();
        }
    }
    
    glutSwapBuffers();
    glutPostRedisplay();
}

void Model::reshape(int width, int height) {
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-300,300, -300, 300, -600, 600);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void Model::keyDown(unsigned char c, int x, int y) {
    if(c == 27) exit(0);
    if(c == 'w') {
        m_show_wireframe = !m_show_wireframe;
        glutPostRedisplay();
    }
    if(c == 'n') {
        m_show_normals = !m_show_normals;
        glutPostRedisplay();
    }
    if(c == 'a') {
        setAntialiasing(!isAntialiased());
        glutPostRedisplay();
    }
    if(c == 's') {
        setSpecular(!isSpecular());
        glutPostRedisplay();
    }
}

void Model::mouseAction(int button, int state, int x, int y) {
    if(button != GLUT_LEFT_BUTTON) return;
    if(state == GLUT_DOWN)
        m_mouse_click = vec2(x*600/windowDim().x-300,300-y*600/windowDim().y);
    if(state == GLUT_UP) {
        m_rot_last = m_rot_last*m_rot;
        m_rot = Quaternion(0,0,0,1);
    }
}

void Model::mouseDrag(int x, int y) {
    vec2 mouse_coord(x*600/windowDim().x-300,300-y*600/windowDim().y);
   
    vec3 diff = vec3(mouse_coord - m_mouse_click,0);
    //if(fabs(diff.x) > fabs(diff.y)) diff.y = 0;
    //else diff.x = 0;
    float m = sqrt(diff*diff); 
    m_rot = Quaternion::fromAxisAngle(diff.x/m,diff.y/m,0,m/360.0);
    //m_rot.w *= 20;
    //printf("m_rot: %f %f %f %f\n",m_rot.x,m_rot.y,m_rot.z,m_rot.w);

    glutPostRedisplay();
}

void Model::drawCircle(vec2 v, float r, float p) {
    glBegin(GL_LINE_LOOP);
    for(float angle=0; angle < 6.28; angle+=p)
        glVertex2f(v.x+sin(angle)*r,v.y+cos(angle)*r);
    glEnd();
}

