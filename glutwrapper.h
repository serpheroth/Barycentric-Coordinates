// Author: Tom Fiset

#ifndef GLUTWRAPPER_H
#define GLUTWRAPPER_H

#include "vec.h"

// This is a wrapper class for glut functionality
class GlutWrapper {
    protected:
        GlutWrapper();
        static GlutWrapper* m_instance;
        static bool m_glut_running;
        static vec2 m_window_dim;

    private:
        static void centerWindow();
        static void registerCallbacks();

    public:
        static bool running();
        static GlutWrapper* inst();
        static void setInst(GlutWrapper* inst);
        static bool isAntialiased();
        static void setAntialiasing(bool b);
        static bool isSpecular();
        static void setSpecular(bool b);

        static void initGlut(int* argc, char** argv, vec2 w_dim, const char* w_title);
        static void start();

        vec2 windowDim();
        void setWindowDim(vec2 dim);

        virtual void reshape(int width, int height);
        virtual void display();
        virtual void keyDown(unsigned char c, int x, int y);
        virtual void keyUp(unsigned char c, int x, int y);
        virtual void mouseAction(int button, int state, int x, int y);
        virtual void mouseDrag(int x, int y);

    private:
        static bool m_antialiased;
        static bool m_specular;
};

#endif
