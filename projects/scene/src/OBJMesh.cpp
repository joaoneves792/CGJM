//
// Created by joao on 11/11/17.
//

#include "OBJMesh.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <OBJMesh.h>

OBJMesh::OBJMesh() =default;

OBJMesh::OBJMesh(const std::string &filename) {
    loadFromFile(filename);
    prepare();
    freeMeshData();
}

void parseVertex(std::stringstream& sin, objGroup* group)
{
    auto v = new objVertex;
    sin >> v->vertex[0] >> v->vertex[1] >> v->vertex[2];
    group->vertices.push_back(v);
}

void parseTexcoord(std::stringstream& sin, objGroup* group)
{
    auto t = new objTexCoord;
    sin >> t->uv[0] >> t->uv[1];
    group->texCoords.push_back(t);
}

void parseNormal(std::stringstream& sin, objGroup* group)
{
    auto n = new objNormal;
    sin >> n->normal[0] >> n->normal[1] >> n->normal[2];
    group->normals.push_back(n);
}

void parseFace(std::stringstream& sin, objGroup* group)
{
    std::string token;
    auto f = new objFace;
    group->faces.push_back(f);
    for(int i = 0; i < 3; i++) {
        std::getline(sin, token, '/');
        if(!token.empty()) f->vertices[i] = (unsigned int)std::stoi(token);
        std::getline(sin, token, '/');
        if(!token.empty()) f->texCoords[i] = (unsigned int)std::stoi(token);
        std::getline(sin, token, ' ');
        if(!token.empty()) f->normals[i] = (unsigned int)std::stoi(token);

    }
}
void parseLine(std::stringstream& sin, objGroup* group){
    std::string s;
    sin >> s;
    if(s.compare("v" ) == 0) parseVertex(sin, group);
    else if(s.compare("vt") == 0) parseTexcoord(sin, group);
    else if(s.compare("vn") == 0) parseNormal(sin, group);
    else if(s.compare("f" ) == 0) parseFace(sin, group);
}

void OBJMesh::loadFromFile(const std::string& filename) {
    std::ifstream ifile(filename);
    std::string line;

    auto group = new objGroup;
    groups.push_back(group);

    while(std::getline(ifile, line)) {
        std::stringstream sin = std::stringstream(line);
        parseLine(sin, group);
    }
    group->faceCount = group->faces.size();
}

void OBJMesh::freeMeshData() {
    for(objGroup* g : groups){
        for(objVertex* v : g->vertices)
            delete v;
        g->vertices.clear();

        for(objNormal* n : g->normals)
            delete n;
        g->normals.clear();

        for(objTexCoord* t : g->texCoords)
            delete t;
        g->texCoords.clear();

        for(objFace* f : g->faces)
            delete f;
        g->faces.clear();

    }
}


void prepareGroup(objGroup* group){
    glGenVertexArrays(1, &(group->vao));
    glBindVertexArray(group->vao);
    {

        /*TODO Dumb code -> make proper use of indexes to avoid duplicate vertexes*/

        auto vertices  = new GLfloat[group->faces.size()*3*3];
        auto normals   = new GLfloat[group->faces.size()*3*3];
        auto texCoords = new GLfloat[group->faces.size()*2*3];

        auto indices  = new GLushort[group->faces.size()*3];

        GLushort vi=0;
        GLushort ni=0;
        GLushort ti=0;
        GLushort ii=0;

        for(objFace* f : group->faces){
            for(int i=0; i<3; i++){
                objVertex* v = group->vertices[f->vertices[i]-1];
                vertices[vi++] = v->vertex[0];
                vertices[vi++] = v->vertex[1];
                vertices[vi++] = v->vertex[2];

                if(!group->normals.empty()) {
                    objNormal *n = group->normals[f->normals[i]-1];
                    normals[ni++] = n->normal[0];
                    normals[ni++] = n->normal[1];
                    normals[ni++] = n->normal[2];
                }
                if(!group->texCoords.empty()){
                    objTexCoord* t = group->texCoords[f->texCoords[i]-1];
                    texCoords[ti++] = t->uv[0];
                    texCoords[ti++] = t->uv[1];
                }
            }
            indices[ii] = ii;
            ii++;
            indices[ii] = ii;
            ii++;
            indices[ii] = ii;
            ii++;
        }

        glGenBuffers(1, &group->vbos[VERTICES_VBO]);
        glBindBuffer(GL_ARRAY_BUFFER, group->vbos[VERTICES_VBO]);
        glBufferData(GL_ARRAY_BUFFER, vi * sizeof(float), vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTICES__ATTR);
        glVertexAttribPointer(VERTICES__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);

        if (ti > 0)
        {
            glGenBuffers(1, &group->vbos[TEXCOORDS_VBO]);
            glBindBuffer(GL_ARRAY_BUFFER, group->vbos[TEXCOORDS_VBO]);
            glBufferData(GL_ARRAY_BUFFER, ti * sizeof(float), texCoords, GL_STATIC_DRAW);
            glEnableVertexAttribArray(TEXCOORDS_ATTR);
            glVertexAttribPointer(TEXCOORDS_ATTR, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
        }
        if (ni > 0)
        {
            glGenBuffers(1, &group->vbos[NORMALS_VBO]);
            glBindBuffer(GL_ARRAY_BUFFER, group->vbos[NORMALS_VBO]);
            glBufferData(GL_ARRAY_BUFFER, ni * sizeof(float), normals, GL_STATIC_DRAW);
            glEnableVertexAttribArray(NORMALS__ATTR);
            glVertexAttribPointer(NORMALS__ATTR, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, 0);
        }

        //Set up the indices (Not very usefull right now see TODO)
        glGenBuffers(1, &(group->eab));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, group->eab);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*ii , indices, GL_STATIC_DRAW);

        //Free all the temporary memory
        delete[] vertices;
        delete[] indices;
        delete[] normals;
        delete[] texCoords;
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OBJMesh::prepare() {
    for(objGroup* group : groups)
        prepareGroup(group);
}

void unloadGroup(objGroup* group){
    glBindVertexArray(group->vao);
    glDisableVertexAttribArray(VERTICES__ATTR);
    glDisableVertexAttribArray(TEXCOORDS_ATTR);
    glDisableVertexAttribArray(NORMALS__ATTR);
    glDeleteBuffers(3, group->vbos);
    glDeleteBuffers(1, &group->eab);
    glDeleteVertexArrays(1, &group->vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void OBJMesh::unload() {
    for(objGroup* group : groups)
        unloadGroup(group);
}

void drawGroup(objGroup* group){
    glBindVertexArray(group->vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)group->faceCount*3, GL_UNSIGNED_SHORT, 0);
}

void OBJMesh::draw(){
    for(objGroup* group : groups)
        drawGroup(group);
}
