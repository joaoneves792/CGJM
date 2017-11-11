#include <iostream>
#include <sstream>
#include <cmath>
#include <OBJMesh.h>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glerrors.h"
#include "Shader.h"


#include "vec.h"
#include "mat.h"
#include "quat.h"

#define CAPTION "SCENE VIEWER (Press ESC to quit)"

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

int WinX = 1024, WinY = 1024;
int WindowHandle = 0;
unsigned int FrameCount = 0;


#define VERTICES 0
#define SHADES 1

GLint MVPUniform;
GLint colorUniform;

Shader* cubeShader;
Shader* tangramShader;

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
float cameraDistance = 5.0f;
float cameraYaw = 0.0f;
float cameraRoll = 0.0f;
float cameraPitch = 0.0f;

const Vec3 front(0, 0 ,-1);
const Vec3 up(0, 1, 0);
const Vec3 right(1, 0, 0);

Quat cameraOrientation(0, up);


bool gimbal = false;
bool perspectiveProjection = true;

CGJM::Mat4 P; //Projection Matrix


OBJMesh* cube;
OBJMesh* triangle;
OBJMesh* square;
OBJMesh* parallelogram;


void createCubeShaderProgram(){
	cubeShader = new Shader("res/cube_vs.glsl", "res/cube_fs.glsl");

    cubeShader->setAttribLocation("inPosition", VERTICES__ATTR);
    cubeShader->setAttribLocation("inTexcoord", TEXCOORDS_ATTR);
    cubeShader->setAttribLocation("inNormal", NORMALS__ATTR);

	cubeShader->link();

	MVPUniform = cubeShader->getUniformLocation("MVP");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void createTangramShaderProgram(){
	tangramShader = new Shader("res/tangramOBJv.shader", "res/tangramOBJf.shader");

	tangramShader->setAttribLocation("inPosition", VERTICES__ATTR);
	tangramShader->setAttribLocation("inNormal", NORMALS__ATTR);

	tangramShader->link();

	MVPUniform = tangramShader->getUniformLocation("MVP");
	colorUniform = tangramShader->getUniformLocation("color");

	checkOpenGLError("ERROR: Could not create shaders.");
}
void destroyShaderPrograms()
{
	glUseProgram(0);
	delete cubeShader;
	delete tangramShader;
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{
	CGJM::Mat4 R;
    CGJM::Mat4 V;
	if(gimbal){
		//order XYZ
		R = CGJM::rotate(front, cameraRoll) *
				 CGJM::rotate(up, cameraYaw) *
				 CGJM::rotate(right, cameraPitch);

	}else{
		R = cameraOrientation.GLMatrix().transpose();
	}

	V = CGJM::translate(0.0f, 0.0f, -cameraDistance) * R;

	CGJM::Mat4 VP = P*V; //VP part of MVP
    CGJM::Mat4 M(1);

	cubeShader->use();
    M = CGJM::translate(0.0f, -2.0f, 0.0f);
    glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
    cube->draw();

	tangramShader->use();

    /*1 square*/
	glUniform4f(colorUniform, 0.0f, 0.5f, 0.0f, 1.0f);
    M = CGJM::translate(-0.75f, -0.75f, 0.0f);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
    square->draw();

	/*2 Small triangles, 2 big triangles and 1 medium triangle*/
	glUniform4f(colorUniform, 0.5f, 0.0f, 0.0f, 1.0f);
	M = CGJM::translate(-0.25f, -0.75f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), 3*M_PI/2); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.0f, 0.2f, 0.5f, 1.0f);
	M = CGJM::translate(0.00f, -0.50f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), -M_PI/4)*CGJM::scale(1.414f, 1.414f, 1.414f); //Medium
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.0f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(-0.50f, 0.00f, 0.0f)*CGJM::scale(2.0f, 2.0f, 2.0f);//Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.5f, 0.5f, 0.0f, 1.0f);
	M = CGJM::translate(0.50f, 0.00f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), M_PI)*CGJM::scale(2.0f, 2.0f, 2.0f); //Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(0.25f, -0.25f, 0.0f); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	triangle->draw();

	/*1 parallelogram*/	
	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	M = CGJM::translate(0.75f, 0.0f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), -M_PI/2.0f);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
    parallelogram->draw();

	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
    cube->unload();
    triangle->unload();
    square->unload();
    parallelogram->unload();
    delete cube;
    delete triangle;
    delete square;
    delete parallelogram;
	//destroyBufferObjects();
	destroyShaderPrograms();
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
		cameraDistance -= (timeDelta*movementRate);
    }
    if(WASD[S]){
		cameraDistance += (timeDelta*movementRate);
    }


    /*Update camera roll*/
    if(WASD[Q] || WASD[E]) {
        cameraRoll += ((WASD[Q])?-1:1)*(timeDelta*movementRate);
		cameraOrientation = Quat(((WASD[Q])?-1:1)*(timeDelta*movementRate), front) * cameraOrientation;
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
	P = (perspectiveProjection)?CGJM::perspective((float)M_PI/4, (WinX/WinY), 0.1, 10):CGJM::ortho(-5, 5, 5, -5, 10, -10);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")" << ((perspectiveProjection)?" Persp ":" Ortho ") << "gimbal " << ((gimbal)?"ON":"OFF");
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
	
	cameraYaw += deltaX*cameraRate;
	cameraPitch += deltaY*cameraRate;

	Quat pitchQuat = Quat(deltaY*cameraRate, right);
	Quat yawQuat = Quat(deltaX*cameraRate, up);

	cameraOrientation = pitchQuat * yawQuat * cameraOrientation;
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
		case 'p':
			perspectiveProjection = !perspectiveProjection;
			P = (perspectiveProjection)?CGJM::perspective((float)M_PI/4, (WinX/WinY), 0.1, 10):CGJM::ortho(-5, 5, 5, -5, 10, -10);
			return;
		case 'g':
			gimbal = !gimbal;
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
	createCubeShaderProgram();
	createTangramShaderProgram();
	/*createBufferObjects(triangle);
	createBufferObjects(square);
	createBufferObjects(parallelogram);*/
	setupCallbacks();
    WASD[W] = 0;
    WASD[A] = 0;
    WASD[S] = 0;
    WASD[D] = 0;
    WASD[Q] = 0;
    WASD[E] = 0;
	cube = new OBJMesh("res/cube_vn.obj");
	triangle = new OBJMesh("res/triangle.obj");
    square = new OBJMesh("res/square.obj");
    parallelogram = new OBJMesh("res/parallelogram.obj");
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
