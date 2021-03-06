// Author: Tom Fiset

#ifndef BARYCENTRIC_H
#define BARYCENTRIC_H

#include <vector>

#include "glutwrapper.h"
#include "vec.h"
#include "vertex.h"
#include "shader.h"

using namespace std;

class Barycentric : public GlutWrapper {
    public:
        Barycentric();
        Barycentric(const char* f_name);

    protected:
        void display();
        void reshape(int width, int height);
        void keyDown(unsigned char c, int x, int y);
        void mouseAction(int button, int state, int x, int y);
        void mouseDrag(int x, int y);

        void drawCircle(vec2 v, float r, float p = 0.1);
        
    private:
        vector<Vertex> m_vertices;
        int            m_drag_index;
        vec2           m_drag_offset;
        Shader*        m_shader;
        bool           m_wireframe;
        vec2           m_rot_last;
        vec2           m_rot;
};

#endif
