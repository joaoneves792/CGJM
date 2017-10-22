#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glerrors.h"
#include "Shader.h"


#include "vec.h"
#include "mat.h"

#define CAPTION "3D TANGRAM"

#define M_PI 3.14159265359

int WinX = 512, WinY = 512;
int WindowHandle = 0;
unsigned int FrameCount = 0;


#define VERTICES 0

GLuint VaoId, VboId[2];
GLint MVPUniform;
GLint colorUniform;
Shader* shaderProgram;

CGJM::Vec3 CameraPosition(0, 0, 1);
CGJM::Vec3 CameraFront(0, 0, -1);
float CameraYaw = 0;
float CameraPitch = 0;

CGJM::Mat4 P(1);
CGJM::Mat4 V = CGJM::translate(-CameraPosition[0], -CameraPosition[1], -CameraPosition[2]);


typedef struct{
	std::vector<Vec3> vertices;
	std::vector<GLuint> indices;
	GLuint VAO;
	GLuint VBOs[2];
}shape;


/////////////////////////////////////////////////////////////////////// CREATE SHAPES

shape triangle = {
			{//Vertices 	//Small triangle (others are obtained through scale)
			Vec3(-0.25f, -0.25f, 0.0f),
			Vec3( 0.25f, -0.25f, 0.0f),
			Vec3( 0.25f,  0.25f, 0.0f)},
      		//Indices
			{0, 1, 2},
		    0, //VAO
		    {0, 0}//VBOs
};
shape square = {
			{//Vertices
			Vec3(-0.25f, -0.25f, 0.0f),
			Vec3( 0.25f, -0.25f, 0.0f),
			Vec3( 0.25f,  0.25f, 0.0f),
			Vec3(-0.25f,  0.25f, 0.0f)},
			//indices
			{0, 1, 2, 
			  2, 3, 0},
		0, //VAO
		{0, 0} //VBOs
};
shape parallelogram = {
						{//Vertices
                        	Vec3( 0.00f, -0.25f, 0.0f),
	                        Vec3( 0.50f, -0.25f, 0.0f),
	                        Vec3( 0.00f,  0.25f, 0.0f),
        	                Vec3(-0.50f,  0.25f, 0.0f)},
      					//indices
						{0, 1, 2, 
						3, 0, 2},
			0, //VAO
			{0, 0} //VBOs
};


std::vector<shape> shapes = {triangle, square, parallelogram};

/////////////////////////////////////////////////////////////////////// ERRORS



void createShaderProgram()
{
	shaderProgram = new Shader("res/3DTvertex.shader", "res/3DTfragment.shader");

	shaderProgram->setAttribLocation("in_Position", VERTICES);

	shaderProgram->link();

	MVPUniform = shaderProgram->getUniformLocation("MVP");
	colorUniform = shaderProgram->getUniformLocation("color");
	
	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	glUseProgram(0);
	delete shaderProgram;
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs


void createBufferObjects(shape& s){
	
	
	//Prepare data
	GLfloat* vertices;
	size_t vertex_i = 0;
	vertices = new GLfloat[s.vertices.size()*4];
	for(Vec3 vertex : s.vertices){
		vertices[vertex_i++] = vertex[0];
		vertices[vertex_i++] = vertex[1];
		vertices[vertex_i++] = vertex[2];
		vertices[vertex_i++] = 1.0f;
	}

	GLuint* indices;
	size_t index_i = 0;
	indices = new GLuint[s.indices.size()];
	for(GLuint index : s.indices){
		indices[index_i++] = index;
	}

	//Upload buffers
	glGenVertexArrays(1, &s.VAO);

	glBindVertexArray(s.VAO);
	{
		glGenBuffers(2, s.VBOs);

		glBindBuffer(GL_ARRAY_BUFFER, s.VBOs[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex_i, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertex_i, vertices);
		
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, 0);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.VBOs[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*index_i, indices, GL_STATIC_DRAW);
	}
	
	delete[] vertices;	
	delete[] indices;
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	for(shape s : shapes){
		glBindVertexArray(s.VAO);
		glDisableVertexAttribArray(VERTICES);
		glDeleteBuffers(2, s.VBOs);
		glDeleteVertexArrays(1, &s.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
	}
}

/////////////////////////////////////////////////////////////////////// SCENE

void drawScene()
{
	shaderProgram->use();


	CGJM::Mat4 MVP = V*P;

	/*1 square*/
	glBindVertexArray(square.VAO);
	glUniform4f(colorUniform, 0.0f, 0.5f, 0.0f, 1.0f);
	CGJM::Mat4 M = CGJM::translate(-0.75f, -0.75f, 0.0f);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, M.transpose()*MVP);
	glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, (GLvoid*)0);
	
	
	/*2 Small triangles, 2 big triangles and 1 medium triangle*/
	glBindVertexArray(triangle.VAO);
	glUniform4f(colorUniform, 0.5f, 0.0f, 0.0f, 1.0f);
	M = CGJM::translate(-0.25f, -0.75f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), 3*M_PI/2); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, M.transpose()*MVP);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

	glUniform4f(colorUniform, 0.0f, 0.0f, 0.5f, 1.0f);
	M = CGJM::translate(0.00f, -0.50f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), -M_PI/4)*CGJM::scale(1.414f, 1.414f, 1.0f); //Medium
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, M.transpose()*MVP);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);
	
	glUniform4f(colorUniform, 0.0f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(-0.50f, 0.00f, 0.0f)*CGJM::scale(2.0f, 2.0f, 1.0f);//Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, M.transpose()*MVP);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);
	
	glUniform4f(colorUniform, 0.5f, 0.5f, 0.0f, 1.0f);
	M = CGJM::translate(0.50f, 0.00f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), M_PI)*CGJM::scale(2.0f, 2.0f, 1.0f); //Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, M.transpose()*MVP);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);

	glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(0.25f, -0.25f, 0.0f); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, M.transpose()*MVP);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)0);
	

	/*1 parallelogram*/	
	glBindVertexArray(parallelogram.VAO);
	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	M = CGJM::translate(0.75f, 0.0f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), -M_PI/2);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, M.transpose()*MVP);
	glDrawElements(GL_TRIANGLES, 3*2, GL_UNSIGNED_INT, (GLvoid*)0);


	glUseProgram(0);
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyBufferObjects();
	destroyShaderProgram();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
	P = CGJM::perspective(M_PI/4, WinX / WinY, 0.1, 10);
	//P = CGJM::ortho(-1, 1, 1, -1, -1, 1);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")" << value;
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void mouse(int x, int y) {
	static int oldX = 0;
	static int oldY = 0;

	int deltaX = x - oldX;
	int deltaY = y - oldY;

	oldX = x;
	oldY = y;

	float magicNumber = M_PI / (WinX);

	CameraYaw += deltaX*magicNumber;
	CameraPitch += deltaY*magicNumber;


	CameraFront[0] = std::cos(CameraYaw) * std::cos(CameraPitch);
	CameraFront[1] = std::sin(CameraPitch);
	CameraFront[2] = std::sin(CameraYaw) * std::cos(CameraPitch);

	/*CGJM::Mat4 R = CGJM::rotate(Vec3(0, 1, 0), deltaX*magicNumber)*CGJM::rotate(Vec3(1, 0, 0), deltaY*magicNumber);
	CGJM::Mat4 invPos = CGJM::translate(-CameraPosition[0], -CameraPosition[1], -CameraPosition[2]);
	CGJM::Mat4 pos = CGJM::translate(CameraPosition[0], CameraPosition[1], CameraPosition[2]);

	V = V*invPos*R*pos;*/

	CameraFront = CameraFront.normalize();
	V = CGJM::lookAt(CameraPosition, CameraPosition + CameraFront, Vec3(0, 1, 0));

}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
	glutPassiveMotionFunc(mouse);
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
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	createBufferObjects(triangle);
	createBufferObjects(square);
	createBufferObjects(parallelogram);
	setupCallbacks();
	//V = CGJM::lookAt(CameraPosition, Vec3(0, 0, 0), Vec3(0, 1, 0));
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////
