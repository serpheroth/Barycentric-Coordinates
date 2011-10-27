#include <GL/glew.h>
#include <GL/glut.h>

#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string readFile(const char* file_name);
GLhandleARB loadShader(const char* file_name, GLenum shader_type);

int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(400,400);
    glutInitWindowPosition(400,400);
    glutCreateWindow("Barycentric Coordinates");

    glShadeModel(GL_FLAT);

    // ...

    glewInit();
    
    if(!GLEW_ARB_vertex_shader ||
       !GLEW_ARB_fragment_shader) {
        printf("ERROR: GLSL not supported.\n");
        exit(0);
    }

    GLhandleARB h_fshader = loadShader("shader.frag",GL_FRAGMENT_SHADER);
        
    glutMainLoop();
}

string readFile(const char* file_name) {
    fstream in(file_name,ios::in);
    string result;
    if(in.is_open()) {
        stringstream buffer;
        buffer << in.rdbuf();
        result = buffer.str();
    } else {
        printf("ERROR: Could not open file!\n");
        exit(0);
    }
    return result;
}

GLhandleARB loadShader(const char* file_name, GLenum shader_type) {
    printf("Loading shader from file: %s\n",file_name);

    GLhandleARB h_shader;
    h_shader = glCreateShaderObjectARB(shader_type);
    if(!h_shader) {
        printf("ERROR: Failed to create shader object!\n");
        exit(0);
    }

    printf("Compiling shader...\n");
    const char* s[1];
    s[0] = readFile(file_name).c_str();
    glShaderSourceARB(h_shader,1,s,NULL); 
    glCompileShaderARB(h_shader);

    int compiled;
    glGetObjectParameterivARB(
        h_shader,
        GL_OBJECT_COMPILE_STATUS_ARB,
        &compiled);
    
    if(!compiled) {
        printf("ERROR: Shader failed to compile!\n");
    
        int length;
        glGetObjectParameterivARB(
            h_shader,
            GL_OBJECT_INFO_LOG_LENGTH_ARB,
            &length);
        vector<char> buffer;
        buffer.resize(length);

        int final_length;
        glGetInfoLogARB(h_shader, length, &final_length, &buffer[0]);
        string message(&buffer[0],length);
        printf("%s",message.c_str());
        if(final_length > length) {
            printf("Shader log contained more information!");
        }
        exit(0);
    }

    printf("Done.\n");

    return h_shader;
}

