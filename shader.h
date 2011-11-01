// Author: Tom Fiset

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <vector>

using namespace std;

class Shader {
    public:
        Shader();
        Shader(const char* f_name, GLenum type);
       ~Shader();

        void load(const char* f_name, GLenum type);
        void link();

        void bind();
        void unbind();

        void uniform2f(const char* name, float a, float b);
        void uniform3f(const char* name, float a, float b, float c);

        GLuint id();
    private:
        GLuint m_program_id;
        vector<GLhandleARB> m_shaders;
};

#endif
