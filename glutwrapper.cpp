// Author: Tom Fiset

#include "glutwrapper.h"

#include <GL/glut.h>
#include <cstdio>

static void reshape(int width, int height);
static void display();
static void keyDown(unsigned char c, int x, int y);
static void keyUp(unsigned char c, int x, int y);
static void mouseAction(int button, int state, int x, int y);
static void mouseDrag(int x, int y);

GlutWrapper* GlutWrapper::m_instance = 0;
bool GlutWrapper::m_glut_running = false;
vec2 GlutWrapper::m_window_dim = vec2();
bool GlutWrapper::m_antialiased = false;
bool GlutWrapper::m_specular    = false;

GlutWrapper::GlutWrapper() {}

bool GlutWrapper::running() {
    return m_glut_running;
}

GlutWrapper* GlutWrapper::inst() {
    return m_instance;
}

void GlutWrapper::setInst(GlutWrapper* inst) {
    m_instance = inst;
}

void GlutWrapper::centerWindow() {
    vec2 w_pos;
    vec2 screen_dim(glutGet(GLUT_SCREEN_WIDTH),
                    glutGet(GLUT_SCREEN_HEIGHT));

    w_pos = (screen_dim - m_window_dim)*0.5;

    printf("    Screen size: %i x %i\n",(int)screen_dim.x,(int)screen_dim.y);
    printf("    Placing window at (%i,%i)\n",(int)w_pos.x,(int)w_pos.y);

    glutInitWindowPosition((int)w_pos.x,(int)w_pos.y);    
}

void GlutWrapper::initGlut(int* argc, char** argv, vec2 w_dim, const char* w_title) {
    printf("Initializing GLUT\n");
    m_glut_running = true;
    m_window_dim = w_dim;
    
    glutInit(argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(m_window_dim.x,m_window_dim.y);

    centerWindow();
    glutCreateWindow(w_title);

    // depth testing / culling
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // lighting
    GLfloat lightAmbient[]  = { 0.4f, 0.4f, 0.4f, 1.0f };
    GLfloat lightDiffuse[]  = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat lightSpecular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
    //GLfloat lightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0,GL_AMBIENT,lightAmbient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,lightDiffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular);
    //glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
    
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);

    registerCallbacks();
}

bool GlutWrapper::isAntialiased() {
    return m_antialiased;
}
void GlutWrapper::setAntialiasing(bool b) {
    m_antialiased = b;
    if(b) {
        glShadeModel(GL_SMOOTH);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH,GL_NICEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH,GL_FASTEST);
        glDisable(GL_BLEND);
        glShadeModel(GL_FLAT);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    }
}

bool GlutWrapper::isSpecular() {
    return m_specular;
}
void GlutWrapper::setSpecular(bool b) {
    m_specular = b;
    if(b) {
        GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat mat_shininess[] = { 500.0 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    } else {
        GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 0.0 };
        GLfloat mat_shininess[] = { 500.0 };
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    }
}

void GlutWrapper::start() {
    glutMainLoop();
}

void GlutWrapper::registerCallbacks() {
    printf("Registering callbacks\n");
    glutDisplayFunc(::display);
    glutReshapeFunc(::reshape);
    glutKeyboardFunc(::keyDown);
    glutKeyboardUpFunc(::keyUp);
    glutMouseFunc(::mouseAction);
    glutMotionFunc(::mouseDrag);
}

vec2 GlutWrapper::windowDim() {
    return m_window_dim;
}

void GlutWrapper::setWindowDim(vec2 dim) {
    m_window_dim = dim;
}

void GlutWrapper::reshape(int width, int height) {}
void GlutWrapper::display() {}
void GlutWrapper::keyDown(unsigned char c, int x, int y) {}
void GlutWrapper::keyUp(unsigned char c, int x, int y) {}
void GlutWrapper::mouseAction(int button, int state, int x, int y) {}
void GlutWrapper::mouseDrag(int x, int y) {}

static void reshape(int width, int height) {
    GlutWrapper::inst()->setWindowDim(vec2(width,height));
    GlutWrapper::inst()->reshape(width,height);
}
static void display() {
    GlutWrapper::inst()->display();
}
static void keyDown(unsigned char c, int x, int y) {
    GlutWrapper::inst()->keyDown(c,x,y);
}
static void keyUp(unsigned char c, int x, int y) {
    GlutWrapper::inst()->keyUp(c,x,y);
}
static void mouseAction(int button, int state, int x, int y) {
    GlutWrapper::inst()->mouseAction(button,state,x,y);
}
static void mouseDrag(int x, int y) {
    GlutWrapper::inst()->mouseDrag(x,y);
}
