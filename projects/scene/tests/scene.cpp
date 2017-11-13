//
// Created by joao on 11/13/17.
//

#include "CGJengine.h"
#include "scene.h"



std::vector<OBJMesh*> meshes;
std::vector<Shader*> shaders;

keyframe original[TANGRAM_PIECES_COUNT];
keyframe floating[TANGRAM_PIECES_COUNT];
keyframe box[TANGRAM_PIECES_COUNT];

keyframe* currentKeyframe;
keyframe* lastKeyframe;
float keyframeStatus = 1.0f;
const int KEYFRAME_DURATION = 2000; //in milliseconds


SceneNode* tangramNodes[TANGRAM_PIECES_COUNT];

/////////////////////////////////////////////////////////////////////// SCENE

void defineKeyframes(){
    /*original[] = {
            {-0.75f, 1.0f, -0.75f, 0.0f}, //square
            {0.0f, 1.0f, 0.75f, 0.0f}, //parallelogram
            {-0.75f, 1.0f, -0.25f, M_PI}, //small tri 1
            {-0.25f, 1.0f, 0.25f, -M_PI/2.0f}, //small tri 2
            {-0.50f, 1.0f, 0.0f, 5.0f*M_PI/4.0f}, //medium tri
            {0.0f, 1.0f, -0.50f, -M_PI/2.0f}, //large tri 1
            {0.0f, 1.0f, 0.50f, M_PI/2.0f} //large tri 2
    };*/
}

SceneGraph* setupScene(){
    currentKeyframe = original;
    lastKeyframe = original;
    keyframeStatus = 1.0f;

    auto camera = new SphereCamera(5.0f, Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));
    //camera = new FreeCamera(Vec3(0.0f, 0.0f, 5.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));

    auto rootNode = new SceneNode();
    auto scene = new SceneGraph(camera, rootNode);

    //Load Models
    auto cube = new OBJMesh("res/cube_vn.obj");
    meshes.push_back(cube);
    auto triangle = new OBJMesh("res/triangle_rot.obj");
    meshes.push_back(triangle);
    auto square = new OBJMesh("res/square_rot.obj");
    meshes.push_back(square);
    auto parallelogram = new OBJMesh("res/parallelogram_rot.obj");
    meshes.push_back(parallelogram);

    //Load Shaders
    auto cubeShader = new Shader("res/cube_vs.glsl", "res/cube_fs.glsl");
    shaders.push_back(cubeShader);
    cubeShader->setAttribLocation("inPosition", VERTICES__ATTR);
    cubeShader->setAttribLocation("inTexcoord", TEXCOORDS_ATTR);
    cubeShader->setAttribLocation("inNormal", NORMALS__ATTR);
    cubeShader->link();
    cubeShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        int uniformLocation = cubeShader->getUniformLocation("MVP");
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
    auto cubeNode = new SceneNode(cube, cubeShader);
    rootNode->addChild(cubeNode);
    cubeNode->translate(0.0f, -1.0f, -1.0f);

    float* position;
    auto squareNode = new SceneNode(square, tangramShader);
    position = currentKeyframe[tan_square].position;
    squareNode->translate(position[0], position[1], position[2]);
    squareNode->setPreDraw([=](){
        glUniform4f(colorUniform, 0.0f, 0.5f, 0.0f, 1.0f);
    });
    cubeNode->addChild(squareNode);
    tangramNodes[tan_square] = squareNode;

    auto parallelogramNode = new SceneNode(parallelogram, tangramShader);
    position = currentKeyframe[tan_parallelogram].position;
    parallelogramNode->translate(position[0], position[1], position[2]);
    parallelogramNode->setPreDraw([=](){
            glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
    });
    cubeNode->addChild(parallelogramNode);
    tangramNodes[tan_parallelogram] = parallelogramNode;

    auto smallTriangle1Node = new SceneNode(triangle, tangramShader);
    position = currentKeyframe[tan_s_triangle1].position;
    smallTriangle1Node->translate(position[0], position[1], position[2]);
    smallTriangle1Node->rotate(0.0f, 1.0f, 0.0f, currentKeyframe[tan_s_triangle1].angle);
    smallTriangle1Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.5f, 0.0f, 0.0f, 1.0f);
    });
    cubeNode->addChild(smallTriangle1Node);
    tangramNodes[tan_s_triangle1] = smallTriangle1Node;

    auto smallTriangle2Node = new SceneNode(triangle, tangramShader);
    position = currentKeyframe[tan_s_trangle2].position;
    smallTriangle2Node->translate(position[0], position[1], position[2]);
    smallTriangle2Node->rotate(0.0f, 1.0f, 0.0f, currentKeyframe[tan_s_trangle2].angle);
    smallTriangle2Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
    });
    cubeNode->addChild(smallTriangle2Node);
    tangramNodes[tan_s_trangle2] = smallTriangle2Node;

    auto mediumTriangleNode = new SceneNode(triangle, tangramShader);
    position = currentKeyframe[tan_m_triangle].position;
    mediumTriangleNode->translate(position[0], position[1], position[2]);
    mediumTriangleNode->rotate(0.0f, 1.0f, 0.0f, currentKeyframe[tan_m_triangle].angle);
    mediumTriangleNode->scale(1.414f, 1.414f, 1.414f);
    mediumTriangleNode->setPreDraw([=](){
        glUniform4f(colorUniform, 0.0f, 0.2f, 0.5f, 1.0f);
    });
    cubeNode->addChild(mediumTriangleNode);
    tangramNodes[tan_m_triangle] = mediumTriangleNode;

    auto largeNode = new SceneNode();
    largeNode->scale(2.0f, 2.0f, 2.0f);
    cubeNode->addChild(largeNode);

    auto largeTriangle1Node = new SceneNode(triangle, tangramShader);
    position = currentKeyframe[tan_l_triangle1].position;
    largeTriangle1Node->translate(position[0], position[1], position[2]);
    largeTriangle1Node->rotate(0.0f, 1.0f, 0.0f, currentKeyframe[tan_l_triangle1].angle);
    largeTriangle1Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.0f, 0.5f, 0.5f, 1.0f);
    });
    largeNode->addChild(largeTriangle1Node);
    tangramNodes[tan_l_triangle1] = largeTriangle1Node;


    auto largeTriangle2Node = new SceneNode(triangle, tangramShader);
    position = currentKeyframe[tan_l_triangle2].position;
    largeTriangle2Node->translate(position[0], position[1], position[2]);
    largeTriangle2Node->rotate(0.0f, 1.0f, 0.0f, currentKeyframe[tan_l_triangle2].angle);
    largeTriangle2Node->setPreDraw([=](){
        glUniform4f(colorUniform, 0.5f, 0.5f, 0.0f, 1.0f);
    });
    largeNode->addChild(largeTriangle2Node);
    tangramNodes[tan_l_triangle2] = largeTriangle2Node;

    return scene;
}

void switchKeyframe(keyframe* kf){
    lastKeyframe = currentKeyframe;
    currentKeyframe = kf;
    keyframeStatus = 0.0f;
}

void updateScene(int dt){
    if(keyframeStatus >= 1.0f)
        return;

    keyframeStatus += dt / KEYFRAME_DURATION;
    if(keyframeStatus > 1.0f)
        keyframeStatus = 1.0f;

    for(int i=0; i<TANGRAM_PIECES_COUNT; i++){
        SceneNode* node = tangramNodes[i];
        node->setPosition(
                (currentKeyframe[i].position[0]-lastKeyframe[i].position[0])*keyframeStatus,
                (currentKeyframe[i].position[1]-lastKeyframe[i].position[1])*keyframeStatus,
                (currentKeyframe[i].position[2]-lastKeyframe[i].position[2])*keyframeStatus
        );
    }
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
