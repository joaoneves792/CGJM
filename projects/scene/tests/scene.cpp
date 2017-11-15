//
// Created by joao on 11/13/17.
//

#include "CGJengine.h"
#include "scene.h"
#include "animation.h"

#define interpolate(v1, v2, t)  (1 - t) * v1 + t * v2

SceneNode* tangramNodes[TANGRAM_PIECES_COUNT];

extern keyframe original[];
extern keyframe floating[];
extern keyframe floatingBox[];
extern keyframe box[];

/////////////////////////////////////////////////////////////////////// SCENE
SceneGraph* setupScene(){
    ResourceManager* rm = ResourceManager::getInstance();

    auto camera = new SphereCamera(10.0f, Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));
    //auto camera = new FreeCamera(Vec3(0.0f, 0.0f, 5.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));

    auto rootNode = new SceneNode("root");
    auto scene = new SceneGraph(camera, rootNode);

    //Load Models
    auto floor = new OBJMesh("res/floor.obj");
    auto table = new OBJMesh("res/table.obj");
    auto triangle = new OBJMesh("res/triangle_rot.obj");
    auto square = new OBJMesh("res/square_rot.obj");
    auto parallelogram = new OBJMesh("res/parallelogram_rot.obj");

    rm->addMesh("floor", floor);
    rm->addMesh("table", table);
    rm->addMesh("triangle", triangle);
    rm->addMesh("square", square);
    rm->addMesh("parallelogram", parallelogram);


    //Load Shaders
    auto tableShader = new Shader("res/tablev.glsl", "res/tablef.glsl");
    tableShader->setAttribLocation("inPosition", VERTICES__ATTR);
    /*tableShader->setAttribLocation("inTexcoord", TEXCOORDS_ATTR);*/
    tableShader->setAttribLocation("inNormal", NORMALS__ATTR);
    tableShader->link();
    tableShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int MVPLocation = tableShader->getUniformLocation("MVP");
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, (P*V*M).transpose());
        int ModelLocation = tableShader->getUniformLocation("Model");
        glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, M.transpose());
        int ViewLocation = tableShader->getUniformLocation("View");
        glUniformMatrix4fv(ViewLocation, 1, GL_FALSE, V.transpose());
    });

    auto floorShader = new Shader("res/floorv.glsl", "res/floorf.glsl");
    floorShader->setAttribLocation("inPosition", VERTICES__ATTR);
    floorShader->link();
    floorShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int uniformLocation = floorShader->getUniformLocation("MVP");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (P*V*M).transpose());
    });

    auto tangramShader = new Shader("res/tangramOBJv.shader", "res/tangramOBJf.shader");
    tangramShader->setAttribLocation("inPosition", VERTICES__ATTR);
    tangramShader->setAttribLocation("inNormal", NORMALS__ATTR);
    tangramShader->link();
    const int colorUniform = tangramShader->getUniformLocation("color");
    tangramShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int uniformLocation = tangramShader->getUniformLocation("MVP");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (P*V*M).transpose());
    });
    checkOpenGLError("ERROR: Could not create shaders.");


    rm->addShader("table", tableShader);
    rm->addShader("floor", floorShader);
    rm->addShader("tangram", tangramShader);

    //Create Nodes
    auto tableNode = new SceneNode("table", table, tableShader);
    rootNode->addChild(tableNode);
    tableNode->translate(0.0f, 2.0f, 0.0f);

    auto floorNode = new SceneNode("floor", floor, floorShader);
    rootNode->addChild(floorNode);

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

    rm->addScene("main", scene);
    return scene;
}


