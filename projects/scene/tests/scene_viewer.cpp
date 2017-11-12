#include <iostream>
#include <sstream>
#include <cmath>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glerrors.h"
#include "Shader.h"


#include "vec.h"
#include "mat.h"
#include "quat.h"

#include "OBJMesh.h"
#include "Camera.h"
#include "SphereCamera.h"
#include "FreeCamera.h"
#include "SceneGraph.h"

#define CAPTION "SCENE VIEWER (Press ESC to quit)"

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

int WinX = 1024, WinY = 1024;
int WindowHandle = 0;
unsigned int FrameCount = 0;


#define VERTICES 0
#define SHADES 1

#define W 0
#define A 1
#define S 2
#define D 3
#define Q 4
#define E 5
#define ESCAPE 27
unsigned char WASD[6];
int mouseX = WinX/2;
int mouseY = WinY/2;

Camera* camera;
SceneGraph* scene;

std::vector<OBJMesh*> meshes;
std::vector<Shader*> shaders;

/////////////////////////////////////////////////////////////////////// SCENE
void setupScene(){
    camera = new SphereCamera(5.0f, Vec3(0.0f, 0.0f, 0.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));
    //camera = new FreeCamera(Vec3(0.0f, 0.0f, 5.0f), Quat(0.0f, Vec3(0.0f, 1.0f, 0.0f)));

    auto rootNode = new SceneNode();
    scene = new SceneGraph(camera, rootNode);

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
        GLint uniformLocation = cubeShader->getUniformLocation("MVP");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (P*V*M).transpose());
    });

    auto tangramShader = new Shader("res/tangramOBJv.shader", "res/tangramOBJf.shader");
    shaders.push_back(tangramShader);
    tangramShader->setAttribLocation("inPosition", VERTICES__ATTR);
    tangramShader->setAttribLocation("inNormal", NORMALS__ATTR);
    tangramShader->link();
    const GLint colorUniform = tangramShader->getUniformLocation("color");
    tangramShader->setMVPFunction([=](Mat4 M, Mat4 V, Mat4 P){
        GLint uniformLocation = tangramShader->getUniformLocation("MVP");
        glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, (P*V*M).transpose());
    });
    checkOpenGLError("ERROR: Could not create shaders.");


    //Create Nodes
    auto cubeNode = new SceneNode(cube, cubeShader);
    rootNode->addChild(cubeNode);
    cubeNode->translate(0.0f, -1.0f, 0.0f);

    auto squareNode = new SceneNode(square, tangramShader);
    squareNode->translate(-0.75f, 1.0f, -0.75f);
    squareNode->setPreDraw([=](){
        glUniform4f(colorUniform, 0.0f, 0.5f, 0.0f, 1.0f);
    });
    cubeNode->addChild(squareNode);

    auto parallelogramNode = new SceneNode(parallelogram, tangramShader);
    parallelogramNode->translate(0.0f, 1.0f, 0.75f);
    parallelogramNode->setPreDraw([=](){
            glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
    });
    cubeNode->addChild(parallelogramNode);




    }


    void drawScene()
    {
        scene->draw();
        checkOpenGLError("ERROR: Could not draw scene.");
        /*
        tangramShader->use();
        */
    /*1 square*/
    /*M = CGJM::translate(-0.75f, 0.0f, -0.75f);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
    square->draw();*/

	/*2 Small triangles, 2 big triangles and 1 medium triangle*/
	/*glUniform4f(colorUniform, 0.5f, 0.0f, 0.0f, 1.0f);
	M = CGJM::translate(-0.75f, 0.0f, -0.25f)*CGJM::rotate(Vec3(0.0f, 1.0f, 0.0f), M_PI); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.0f, 0.2f, 0.5f, 1.0f);
	M = CGJM::translate(-0.50f, 0.0f, 0.0f)*CGJM::rotate(Vec3(0.0f, 1.0f, 0.0f), 5*M_PI/4)*CGJM::scale(1.414f, 1.414f, 1.414f); //Medium
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.0f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(0.0f, 0.00f, -0.5f)*CGJM::rotate(Vec3(0.0f,1.0f, 0.0f), -M_PI/2)*CGJM::scale(2.0f, 2.0f, 2.0f);//Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.5f, 0.5f, 0.0f, 1.0f);
	M = CGJM::translate(0.0f, 0.00f, 0.5f)*CGJM::rotate(Vec3(0.0f, 1.0f, 0.0f), M_PI/2)*CGJM::scale(2.0f, 2.0f, 2.0f); //Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(-0.25f, 0.0f, 0.25f)*CGJM::rotate(Vec3(0.0f, 1.0f, 0.0f), -M_PI/2.0f); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();*/

	/*1 parallelogram*/	
	/*glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	M = CGJM::translate(0.0f, 0.0f, 0.75f);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
    parallelogram->draw();*/

}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
    //Destroy meshes
    for(OBJMesh* m : meshes){
        m->unload();
        delete m;
    }
    meshes.clear();

    //Destroy shaders
    glUseProgram(0);
    for(Shader* s: shaders){
        delete s;
    }
    checkOpenGLError("ERROR: Could not destroy shaders.");

    scene->destroy();

    //Destroy the camera
    delete camera;
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void update(){
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeDelta = currentTime-lastTime;
    lastTime = currentTime;

    /*Update camera movement*/
    float movementRate = 0.005; //magic number
    if(WASD[W]){
		camera->move(0.0f, 0.0f, -(timeDelta*movementRate));
    }
	if(WASD[A]){
		camera->move(-timeDelta*movementRate, 0.0f, 0.0f);
	}
    if(WASD[S]){
		camera->move(0.0f, 0.0f, timeDelta*movementRate);
    }
	if(WASD[D]){
		camera->move(timeDelta*movementRate, 0.0f, 0.0f);
	}


    /*Update camera roll*/
    if(WASD[Q] || WASD[E]) {
		camera->changeOrientation(0.0f, 0.0f, ((WASD[Q])?-1:1)*(timeDelta*movementRate));
    }
    
}

void idle()
{
    update();
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
    camera->perspective((float)M_PI/4, (WinX/WinY), 0.1, 10);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void mouseTimer(int value)
{
	//Wthis is executed every 10ms, necessary since the values vary a alot between Win and linux

	float cameraRate = (float)-1.0f*(M_PI / (WinX*2));
    int deltaX = (WinX/2) - mouseX;
    int deltaY = (WinY/2) - mouseY;
	glutWarpPointer(WinX/2, WinY/2);
	mouseX = WinX / 2;
	mouseY = WinY / 2;
	
	camera->changeOrientation(deltaX*cameraRate, deltaY*cameraRate, 0.0f);
    glutTimerFunc(10, mouseTimer, 0);
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'w':
            WASD[W] = key;
            return;
        case 'a':
            WASD[A] = key;
            return;
        case 's':
            WASD[S] = key;
            return;
        case 'd':
            WASD[D] = key;
            return;
        case 'q':
            WASD[Q] = key;
            return;
        case 'e':
            WASD[E] = key;
            return;
        case ESCAPE:
            glutLeaveMainLoop();
            return;
        default:
            return;
    }
}

void keyboardUp(unsigned char key, int x, int y){
    switch(key){
        case 'w':
            WASD[W] = 0;
            return;
        case 'a':
            WASD[A] = 0;
            return;
        case 's':
            WASD[S] = 0;
            return;
        case 'd':
            WASD[D] = 0;
            return;
        case 'q':
            WASD[Q] = 0;
            return;
        case 'e':
            WASD[E] = 0;
            return;
        default:
            return;
    }
}

void mouse(int x, int y) {
	mouseX = x;
	mouseY = y;
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	glutTimerFunc(0,mouseTimer,0);
	glutPassiveMotionFunc(mouse);
	glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
}

void checkOpenGLInfo()
{
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << std::endl;
	std::cerr << "OpenGL version " << version << std::endl;
	std::cerr << "GLSL version " << glslVersion << std::endl;
}

void setupOpenGL()
{
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() 
{
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	//GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
    glutSetCursor(GLUT_CURSOR_NONE);
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	setupCallbacks();
    setupScene();

    WASD[W] = 0;
    WASD[A] = 0;
    WASD[S] = 0;
    WASD[D] = 0;
    WASD[Q] = 0;
    WASD[E] = 0;
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
