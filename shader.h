#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>

class Shader {
    public:
        Shader(int shader_type);
        Shader(const char* f_name, int shader_type);
       ~Shader();

        void load(const char* f_name);
        void clear();

        void bind();
        void unbind();

        bool   ok();
        GLenum type();
        GLuint id();
    private:
        bool        m_successful;
        GLenum      m_shader_type;
        GLuint      m_shader_id;
        GLhandleARB m_shader;
};

#endif
