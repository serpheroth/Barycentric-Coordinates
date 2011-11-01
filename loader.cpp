// Author: Tom Fiset

#include "loader.h"

#include <fstream>

void Loader::loadVertices(vector<Vertex>& vertices, const char* f_name) {
    ifstream ifs(f_name,ifstream::in);
    
    string s = "";
    while(ifs.good()) {
        getline(ifs,s);
        
        size_t found = 0;
        found = s.find_first_not_of(' ');

        if(s[found] == '#') continue;
        if(s.size() <= 1) continue;

        Vertex a,b,c;
        sscanf(
            s.c_str(),
            "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f",
            &a.pos.x,&a.pos.y,&a.pos.z,
            &b.pos.x,&b.pos.y,&b.pos.z,
            &c.pos.x,&c.pos.y,&c.pos.z,
            &a.color.x,&a.color.y,&a.color.z,
            &b.color.z,&b.color.y,&b.color.z,
            &c.color.x,&c.color.y,&c.color.z);

        vertices.push_back(a);
        vertices.push_back(b);
        vertices.push_back(c);
    }
}

string Loader::readShaderFile(const char* f_name) {
    ifstream ifs(f_name,ifstream::in);
    string s;

    while(ifs.good()) s += ifs.get();

    ifs.close();
    s[s.size()-1] = '\0';

    return s;
}
