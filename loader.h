// Author: Tom Fiset

#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include <string>

#include "vertex.h"

using namespace std;

namespace Loader {
    // a function for loading vertex data from a text file
    void loadVertices(vector<Vertex>& vertices, const char* f_name);
    string readShaderFile(const char* f_name);
}

#endif
