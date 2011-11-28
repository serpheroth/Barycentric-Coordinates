#include "barycentric.h"
#include "model.h"
#include <cstdlib>

int main(int argc, char** argv) {
    if(argc == 1) {
        printf("Please specify a file.\n");
        exit(0);
    } 

    GlutWrapper::initGlut(
        &argc,argv,vec2(800,800),"Barycentric Coordinates Demo");

    Model* b = new Model(argv[1]);

    GlutWrapper::start();
}
