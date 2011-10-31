#include "shader.h"

#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

#ifdef GLEW_SUPPORT

Shader::Shader() {
    m_program_id = glCreateProgram();
}

Shader::Shader(const char* f_name, GLenum type) {
    m_program_id = glCreateProgram();
    load(f_name,type);
}

Shader::~Shader() {
    for(int i=0; i<m_shaders.size(); i++) {
        glDetachShader(m_program_id,m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program_id);
}

static inline string readFile(const char* f_name) {
    ifstream ifs(f_name,ifstream::in);
    string s;

    while(ifs.good()) s += ifs.get();

    ifs.close();
    s[s.size()-1] = '\0';

    return s;
}

void Shader::load(const char* f_name, GLenum type) {
    printf("Loading shader from: %s\n",f_name);
    GLhandleARB shader = glCreateShaderObjectARB(type);

    if(!shader) {
        printf("ERROR: glCreateShaderObjectARB failed!");
        return;
    }

    m_shaders.push_back(shader);

    printf("    Reading shader file...\n");
    string shader_text = readFile(f_name);
    if(shader_text.empty()) {
        printf("ERROR: could not read file!\n");
        return;
    }

    printf("    Compiling shader...\n");

    const char* array[1];
    array[0] = shader_text.c_str();

    glShaderSourceARB(shader,1,(const GLcharARB**)array,NULL);
    glCompileShaderARB(shader);

    int compiled;
    glGetObjectParameterivARB(
        shader,GL_OBJECT_COMPILE_STATUS_ARB,&compiled);
    if(!compiled) {
        printf("ERROR: shader failed to compile!\n");

        int log_length;
        glGetObjectParameterivARB(
            shader,GL_OBJECT_INFO_LOG_LENGTH_ARB,&log_length);

        vector<char> buffer;
        buffer.resize(log_length);

        int final_length;
        glGetInfoLogARB(
            shader,log_length,&final_length,&buffer[0]);
        string message(&buffer[0],log_length);
        printf("%s",message.c_str());
        if(final_length > log_length)
            printf("Compile log contained more information!\n");
    
        return;
    }

    printf("    Attaching shader...\n");
    glAttachShader(m_program_id,shader);
}

void Shader::link() {
    glLinkProgram(m_program_id);
}

void Shader::bind() {
    glUseProgram(m_program_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::uniform2f(const char* name, float a, float b) {
    GLint loc = glGetUniformLocation(m_program_id,name);
    glUniform2f(loc,a,b);
}

void Shader::uniform3f(const char* name, float a, float b, float c) {
    GLint loc = glGetUniformLocation(m_program_id,name);
    glUniform3f(loc,a,b,c);
}

GLuint Shader::id() {
    return m_program_id;
}

#endif //GLEW_SUPPORT
