//
// Created by joao on 11/13/17.
//

#include "CGJengine.h"
#include "scene.h"
#include "animation.h"

#define interpolate(v1, v2, t)  (1 - t) * v1 + t * v2

std::vector<OBJMesh*> meshes;
std::vector<Shader*> shaders;

SceneNode* tangramNodes[TANGRAM_PIECES_COUNT];

extern keyframe original[];
extern keyframe floating[];
extern keyframe floatingBox[];
extern keyframe box[];

/////////////////////////////////////////////////////////////////////// SCENE
SceneGraph* setupScene(){
    auto camera = new SphereCamera(5.0f, Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));
    //auto camera = new FreeCamera(Vec3(0.0f, 0.0f, 5.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));

    auto rootNode = new SceneNode("root");
    auto scene = new SceneGraph(camera, rootNode);

    //Load Models
    auto floor = new OBJMesh("res/floor.obj");
    meshes.push_back(floor);
    auto table = new OBJMesh("res/table.obj");
    meshes.push_back(table);
    auto triangle = new OBJMesh("res/triangle_rot.obj");
    meshes.push_back(triangle);
    auto square = new OBJMesh("res/square_rot.obj");
    meshes.push_back(square);
    auto parallelogram = new OBJMesh("res/parallelogram_rot.obj");
    meshes.push_back(parallelogram);

    //Load Shaders
    auto tableShader = new Shader("res/tablev.glsl", "res/tablef.glsl");
    shaders.push_back(tableShader);
    tableShader->setAttribLocation("inPosition", VERTICES__ATTR);
    /*tableShader->setAttribLocation("inTexcoord", TEXCOORDS_ATTR);*/
    tableShader->setAttribLocation("inNormal", NORMALS__ATTR);
    tableShader->link();
    tableShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int uniformLocation = tableShader->getUniformLocation("MVP");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (P*V*M).transpose());
    });

    auto floorShader = new Shader("res/floorv.glsl", "res/floorf.glsl");
    shaders.push_back(floorShader);
    floorShader->setAttribLocation("inPosition", VERTICES__ATTR);
    floorShader->link();
    floorShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int uniformLocation = floorShader->getUniformLocation("MVP");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (P*V*M).transpose());
    });

    auto tangramShader = new Shader("res/tangramOBJv.shader", "res/tangramOBJf.shader");
    shaders.push_back(tangramShader);
    tangramShader->setAttribLocation("inPosition", VERTICES__ATTR);
    tangramShader->setAttribLocation("inNormal", NORMALS__ATTR);
    tangramShader->link();
    const int colorUniform = tangramShader->getUniformLocation("color");
    tangramShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int uniformLocation = tangramShader->getUniformLocation("MVP");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (P*V*M).transpose());
    });
    checkOpenGLError("ERROR: Could not create shaders.");


    //Create Nodes
    auto tableNode = new SceneNode("table", table, tableShader);
    rootNode->addChild(tableNode);

    auto floorNode = new SceneNode("floor", floor, floorShader);
    rootNode->addChild(floorNode);
    floorNode->hidden(true);

    auto squareNode = new SceneNode("square", square, tangramShader);
    squareNode->setPreDraw([=](){
        glUniform4f(colorUniform, 0.0f, 0.5f, 0.0f, 1.0f);
    });
    tableNode->addChild(squareNode);
    tangramNodes[tan_square] = squareNode;

    auto parallelogramNode = new SceneNode("parallelogram", parallelogram, tangramShader);
    parallelogramNode->setPreDraw([=](){
            glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
    });
    tableNode->addChild(parallelogramNode);
    tangramNodes[tan_parallelogram] = parallelogramNode;

    auto smallTriangle1Node = new SceneNode("smallTriangle1", triangle, tangramShader);
    smallTriangle1Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.5f, 0.0f, 0.0f, 1.0f);
    });
    tableNode->addChild(smallTriangle1Node);
    tangramNodes[tan_s_triangle1] = smallTriangle1Node;

    auto smallTriangle2Node = new SceneNode("smallTriangle2", triangle, tangramShader);
    smallTriangle2Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
    });
    tableNode->addChild(smallTriangle2Node);
    tangramNodes[tan_s_triangle2] = smallTriangle2Node;

    auto mediumTriangleNode = new SceneNode("mediumTriangle", triangle, tangramShader);
    mediumTriangleNode->scale(1.414f, 1.414f, 1.414f);
    mediumTriangleNode->setPreDraw([=](){
        glUniform4f(colorUniform, 0.0f, 0.2f, 0.5f, 1.0f);
    });
    tableNode->addChild(mediumTriangleNode);
    tangramNodes[tan_m_triangle] = mediumTriangleNode;

    auto largeNode = new SceneNode("largeScaling");
    largeNode->scale(2.0f, 2.0f, 2.0f);
    tableNode->addChild(largeNode);

    auto largeTriangle1Node = new SceneNode("largeTriangle1", triangle, tangramShader);
    largeTriangle1Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.0f, 0.5f, 0.5f, 1.0f);
    });
    largeNode->addChild(largeTriangle1Node);
    tangramNodes[tan_l_triangle1] = largeTriangle1Node;


    auto largeTriangle2Node = new SceneNode("largeTriangle2", triangle, tangramShader);
    largeTriangle2Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.5f, 0.5f, 0.0f, 1.0f);
    });
    largeNode->addChild(largeTriangle2Node);
    tangramNodes[tan_l_triangle2] = largeTriangle2Node;

    //Place all parts according to "original" keyframe
    applyAnimation( original, original, 1.0f);

    return scene;
}


void destroyScene(SceneGraph* scene) {
    //Destroy meshes
    for (OBJMesh *m : meshes) {
        m->unload();
        delete m;
    }
    meshes.clear();

    //Destroy shaders
    glUseProgram(0);
    for (Shader *s: shaders) {
        delete s;
    }
    checkOpenGLError("ERROR: Could not destroy shaders.");

    //Destroy Scene
    delete scene->getCamera();
    scene->destroy();
    delete scene;
}