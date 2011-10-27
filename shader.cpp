#include "shader.h"

#include <cstdio>
#include <vector>
#include <string>

using namespace std;

Shader::Shader(int shader_type) {
    m_successful  = false;
    m_shader_type = shader_type;
}

Shader::Shader(const char* f_name, int shader_type) {
    m_successful = false;
    m_shader_type = shader_type;
    load(f_name);
}

Shader::~Shader() {
    clear();
}

static char* readFile(const char* f_name) {
    FILE* file = fopen(f_name,"r");
    if(!file) return NULL;

    fseek(file,0,SEEK_END);
    int size = ftell(file);
    rewind(file);

    char* text;
    text = (char*)malloc(sizeof(char)*size + 1);
    fread(text,1,size,file); 
    text[size] = '\0';
     
    fclose(file);

    return text;
}

void Shader::load(const char* f_name) {
    // make sure GLSL is supported
    if(!GLEW_ARB_vertex_shader ||
       !GLEW_ARB_fragment_shader) {
        printf("ERROR: GLSL not supported.\n");
        return;
    }

    // clear previous data
    clear();

    printf("Loading shader from: %s\n",f_name);

    m_shader = glCreateShaderObjectARB(m_shader_type);
    if(!m_shader) {
        printf("ERROR: glCreateShaderObjectARB failed!");
        return;
    }

    printf("    Reading shader file...\n");
    const char* shader_text = readFile(f_name);
    if(!shader_text) {
        printf("ERROR: could not read file!\n");
        return;
    }
    
    printf("    Compiling shader...\n");
    glShaderSourceARB(m_shader,1,(const GLcharARB**)shader_text,NULL);
    glCompileShaderARB(m_shader);

    int compiled;
    glGetObjectParameterivARB(
        m_shader,GL_OBJECT_COMPILE_STATUS_ARB,&compiled);
    if(!compiled) {
        printf("ERROR: shader failed to compile!\n");

        int log_length;
        glGetObjectParameterivARB(
            m_shader,GL_OBJECT_INFO_LOG_LENGTH_ARB,&log_length);

        vector<char> buffer;
        buffer.resize(log_length);

        int final_length;
        glGetInfoLogARB(
            m_shader,log_length,&final_length,&buffer[0]);
        string message(&buffer[0],log_length);
        printf("%s",message.c_str());
        if(final_length > log_length)
            printf("Compile log contained more information!\n");
    
        return;
    }

    printf("    Creating program...\n");
    m_shader_id = glCreateProgram();
    printf("    Attaching shader...\n");
    glAttachShader(m_shader_id,m_shader);
    printf("    Linking program...\n");
    glLinkProgram(m_shader_id);    

    m_successful  = true;
}

void Shader::clear() {
    if(m_successful) {
        glDetachShader(m_shader_id,m_shader);
        glDeleteShader(m_shader);
        glDeleteProgram(m_shader_id);
    }
    m_successful = false;
}

void Shader::bind() {
    glUseProgram(m_shader_id);
}

void Shader::unbind() {
    glUseProgram(0);
}

bool Shader::ok() {
    return m_successful;
}

GLenum Shader::type() {
    return m_shader_type;
}

GLuint Shader::id() {
    return m_shader_id;
}
