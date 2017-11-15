#include <iostream>
#include <sstream>
#include <cmath>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "CGJengine.h"
#include "scene.h"
#include "animation.h"


#define CAPTION "SCENE VIEWER (ESC:quit F:Animate ARROWS:Move WASDQE:Camera)"

int WinX = 1024, WinY = 1024;
int WindowHandle = 0;
unsigned int FrameCount = 0;

const std::string SCENE_NAME = "main";

#define W 0
#define A 1
#define S 2
#define D 3
#define Q 4
#define E 5
#define UP 6
#define DOWN 7
#define RIGHT 8
#define LEFT 9

#define ESCAPE 27
unsigned char keyboardStatus[10];
int mouseX = WinX/2;
int mouseY = WinY/2;

/////////////////////////////////////////////////////////////////////// CALLBACKS
void cleanup()
{
    ResourceManager::getInstance()->destroyEverything();
    ResourceManager::getInstance()->deleteInstance();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE_NAME);
	scene->draw();
	checkOpenGLError("ERROR: Could not draw scene.");
	glutSwapBuffers();
}

void update(){
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);

    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int timeDelta = currentTime-lastTime;
    lastTime = currentTime;

    SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE_NAME);
    /*Update camera movement*/
    float movementRate = 0.005; //magic number
    if(keyboardStatus[W]){
		scene->getCamera()->move(0.0f, 0.0f, -(timeDelta*movementRate));
    }
	if(keyboardStatus[A]){
		scene->getCamera()->move(-timeDelta*movementRate, 0.0f, 0.0f);
	}
    if(keyboardStatus[S]){
		scene->getCamera()->move(0.0f, 0.0f, timeDelta*movementRate);
    }
	if(keyboardStatus[D]){
		scene->getCamera()->move(timeDelta*movementRate, 0.0f, 0.0f);
	}

    std::string movingNodeName = "table";
    SceneNode* movingNode = scene->findNode(movingNodeName);

    if(keyboardStatus[UP] && movingNode != nullptr){
        movingNode->translate(0.0f, 0.0f, -(timeDelta*movementRate));
    }
    if(keyboardStatus[DOWN] && movingNode != nullptr){
        movingNode->translate(0.0f, 0.0f, (timeDelta*movementRate));
    }
    if(keyboardStatus[LEFT] && movingNode != nullptr){
        movingNode->translate(-(timeDelta*movementRate), 0.0f, 0.0f);
    }
    if(keyboardStatus[RIGHT] && movingNode != nullptr){
        movingNode->translate((timeDelta*movementRate), 0.0f, 0.0f);
    }

    /*Update camera roll*/
    if(keyboardStatus[Q] || keyboardStatus[E]) {
		scene->getCamera()->changeOrientation(0.0f, 0.0f, ((keyboardStatus[Q])?-1:1)*(timeDelta*movementRate));
    }

    updateScene(timeDelta);
    
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
    SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE_NAME);
    scene->getCamera()->perspective((float)M_PI/4, (WinX/WinY), 0.1, 100);
	//scene->getCamera()->ortho(-2, 2, -2, 2, 0, 10);
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

    SceneGraph* scene = ResourceManager::getInstance()->getScene(SCENE_NAME);
	scene->getCamera()->changeOrientation(deltaX*cameraRate, deltaY*cameraRate, 0.0f);
    glutTimerFunc(10, mouseTimer, 0);
}

void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'w':
            keyboardStatus[W] = key;
            return;
        case 'a':
            keyboardStatus[A] = key;
            return;
        case 's':
            keyboardStatus[S] = key;
            return;
        case 'd':
            keyboardStatus[D] = key;
            return;
        case 'q':
            keyboardStatus[Q] = key;
            return;
        case 'e':
            keyboardStatus[E] = key;
            return;
        case 'f':
            animate();
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
            keyboardStatus[W] = 0;
            return;
        case 'a':
            keyboardStatus[A] = 0;
            return;
        case 's':
            keyboardStatus[S] = 0;
            return;
        case 'd':
            keyboardStatus[D] = 0;
            return;
        case 'q':
            keyboardStatus[Q] = 0;
            return;
        case 'e':
            keyboardStatus[E] = 0;
            return;
        default:
            return;
    }
}

void specialKeyboard(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            keyboardStatus[UP] = 1;
            break;
        case GLUT_KEY_DOWN:
            keyboardStatus[DOWN] = 1;
            break;
        case GLUT_KEY_LEFT:
            keyboardStatus[LEFT] = 1;
            break;
        case GLUT_KEY_RIGHT:
            keyboardStatus[RIGHT] = 1;
            break;
        default:
            return;
    }
}

void specialKeyboardUp(int key, int x, int y){
    switch(key){
        case GLUT_KEY_UP:
            keyboardStatus[UP] = 0;
            break;
        case GLUT_KEY_DOWN:
            keyboardStatus[DOWN] = 0;
            break;
        case GLUT_KEY_LEFT:
            keyboardStatus[LEFT] = 0;
            break;
        case GLUT_KEY_RIGHT:
            keyboardStatus[RIGHT] = 0;
            break;
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
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboardUp);
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

    setupScene(SCENE_NAME);

    keyboardStatus[W] = 0;
    keyboardStatus[A] = 0;
    keyboardStatus[S] = 0;
    keyboardStatus[D] = 0;
    keyboardStatus[Q] = 0;
    keyboardStatus[E] = 0;
    keyboardStatus[UP] = 0;
    keyboardStatus[DOWN] = 0;
    keyboardStatus[LEFT] = 0;
    keyboardStatus[RIGHT] = 0;
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
