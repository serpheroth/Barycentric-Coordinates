// Author: Tom Fiset

#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include "glutwrapper.h"
#include "vec.h"
#include "vertex.h"
#include "shader.h"
#include "quaternion.h"

using namespace std;

class Model : public GlutWrapper {
    public:
        Model();
        Model(const char* f_name);
    private:
        void init(const char* f_name);

    protected:
        void display();
        void reshape(int width, int height);
        void keyDown(unsigned char c, int x, int y);
        void mouseAction(int button, int state, int x, int y);
        void mouseDrag(int x, int y);

        void drawCircle(vec2 v, float r, float p = 0.1);
        
    private:
        vector<Vertex> m_vertices;
        vector<vec3>   m_normals;

        
        bool           m_show_wireframe;
        bool           m_show_normals;
        vec2           m_mouse_click;
        Quaternion     m_rot_last;
        Quaternion     m_rot;
};

#endif
