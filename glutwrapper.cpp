// Author: Tom Fiset

#include "glutwrapper.h"

#include <GL/glut.h>

static void reshape(int width, int height);
static void display();
static void keyDown(unsigned char c, int x, int y);
static void keyUp(unsigned char c, int x, int y);
static void mouseAction(int button, int state, int x, int y);
static void mouseDrag(int x, int y);

GlutWrapper* GlutWrapper::m_instance = 0;
bool GlutWrapper::m_glut_running = false;
vec2 GlutWrapper::m_window_dim = vec2();

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

#ifdef DEBUG
    printf("    Screen size: %i x %i\n",(int)screen_dim.x,(int)screen_dim.y);
    printf("    Placing window at (%i,%i)\n",(int)w_pos.x,(int)w_pos.y);
#endif

    glutInitWindowPosition((int)w_pos.x,(int)w_pos.y);    
}

void GlutWrapper::initGlut(int* argc, char** argv, vec2 w_dim, const char* w_title) {
    m_glut_running = true;
    m_window_dim = w_dim;
    
    glutInit(argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(m_window_dim.x,m_window_dim.y);

    centerWindow();
    glutCreateWindow(w_title);

    glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH,GL_NICEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    registerCallbacks();
    glutMainLoop();
}

void GlutWrapper::registerCallbacks() {
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
