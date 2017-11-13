//
// Created by joao on 11/12/17.
//

#ifndef CGJM_SCENEGRAPH_H
#define CGJM_SCENEGRAPH_H

#include <functional>
#include <list>
#include "CGJengine.h"

class SceneGraph;

class SceneNode{
private:
    OBJMesh* mesh;
    Shader* shader;
    Vec3 position;
    Quat orientation;
    Vec3 size;

    bool visible;

    SceneNode* parent;
    std::list<SceneNode*> childs = {};

    std::function<void()> pre_draw;
    std::function<void()> post_draw;

public:
    SceneNode();
    SceneNode(OBJMesh* mesh);
    SceneNode(OBJMesh* mesh, Shader* shader);

    void setMesh(OBJMesh* mesh);
    void setShader(Shader* shader);

    void setPosition(float x, float y, float z);
    void translate(float x, float y, float z);

    void setOrientation(float x, float y, float z, float angle);
    void setOrientation(Quat quat);
    void rotate(float x, float y, float z, float angle);

    void lerpPosition(float x, float y, float z, float c);
    void lerpOrientation(float x, float y, float z, float angle, float c);
    void lerpOrientation(Quat q, float c);

    void scale(float x, float y, float z);

    /*void setPreDraw(void(*callback)());
    void setPostDraw(void(*callback)());*/

    void setPreDraw(std::function<void()> callback);
    void setPostDraw(std::function<void()> callback);

    void addChild(SceneNode* child);
    void destroy();

    void update(int dt);
    void draw(SceneGraph* scene);

    Mat4 getModelMatrix();

private:
    Mat4 getTranslation();
    Quat getOrientation();
    Mat4 getScale();
};

class SceneGraph{
private:
    Camera* camera;
    SceneNode* root;
public:
    SceneGraph();
    SceneGraph(Camera* cam);
    SceneGraph(Camera* cam, SceneNode* rootNode);

    void destroy();

    Mat4 getViewMatrix();
    Mat4 getProjectionMatrix();

    Camera* getCamera();
    SceneNode* getRoot();

    void update(int dt);
    void draw();
};

#endif //CGJM_SCENEGRAPH_H
