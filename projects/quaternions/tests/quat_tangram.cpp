#include <iostream>
#include <sstream>
#include <cmath>
#include <quat.h>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "glerrors.h"
#include "Shader.h"


#include "vec.h"
#include "mat.h"

#define CAPTION "QUAT TANGRAM (Press ESC to quit)"

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

int WinX = 512, WinY = 512;
int WindowHandle = 0;
unsigned int FrameCount = 0;


#define VERTICES 0
#define SHADES 1

GLint MVPUniform;
GLint colorUniform;
Shader* shaderProgram;

#define W 0
#define A 1
#define S 2
#define D 3
#define Q 4
#define E 5
#define ESCAPE 27
unsigned char WASD[6];
float cameraDistance = 5.0f;
float cameraYaw = 0.0f;
float cameraRoll = 0.0f;
float cameraPitch = 0.0f;

Vec3 front(0, 0 ,-1);
Vec3 up(0, 1, 0);
Vec3 right(1, 0, 0);

Quat cameraOrientation(0, up);


bool gimbal = false;
bool perspectiveProjection = true;

CGJM::Mat4 P; //Projection Matrix

typedef struct{
	std::vector<Vec3> vertices;
	std::vector<GLuint> indices;
	GLuint VAO;
	GLuint VBOs[2];
}shape;


/////////////////////////////////////////////////////////////////////// CREATE SHAPES

shape triangle = {
			{//Vertices 	//Small triangle (others are obtained through scale)
			Vec3(-0.25f, -0.25f, 0.00f), Vec3(0.5f, 0.5f, 0.5f), //0
			Vec3( 0.25f, -0.25f, 0.00f), Vec3(0.5f, 0.5f, 0.5f), //1
			Vec3( 0.25f,  0.25f, 0.00f), Vec3(0.5f, 0.5f, 0.5f), //2

            Vec3(-0.25f, -0.25f, 0.25f), Vec3(0.0f, 0.0f, 0.0f), //3
            Vec3( 0.25f, -0.25f, 0.25f), Vec3(0.0f, 0.0f, 0.0f), //4
            Vec3( 0.25f,  0.25f, 0.25f), Vec3(0.0f, 0.0f, 0.0f), //5

            Vec3(-0.25f, -0.25f, 0.00f), Vec3(0.1f, 0.1f, 0.1f), //6
            Vec3( 0.25f, -0.25f, 0.00f), Vec3(0.1f, 0.1f, 0.1f), //7
            Vec3( 0.25f, -0.25f, 0.25f), Vec3(0.1f, 0.1f, 0.1f), //8
            Vec3(-0.25f, -0.25f, 0.25f), Vec3(0.1f, 0.1f, 0.1f), //9 678896

            Vec3(-0.25f, -0.25f, 0.00f), Vec3(0.2f, 0.2f, 0.2f), //10
            Vec3(-0.25f, -0.25f, 0.25f), Vec3(0.2f, 0.2f, 0.2f), //11
            Vec3( 0.25f,  0.25f, 0.25f), Vec3(0.2f, 0.2f, 0.2f), //12
            Vec3( 0.25f,  0.25f, 0.00f), Vec3(0.2f, 0.2f, 0.2f), //13 //10 11 12 12 13 10

            Vec3( 0.25f, -0.25f, 0.00f), Vec3(0.3f, 0.3f, 0.3f), //14
            Vec3( 0.25f,  0.25f, 0.00f), Vec3(0.3f, 0.3f, 0.3f), //15
            Vec3( 0.25f,  0.25f, 0.25f), Vec3(0.3f, 0.3f, 0.3f), //16
            Vec3( 0.25f, -0.25f, 0.25f), Vec3(0.3f, 0.3f, 0.3f), //17 //14 15 16 16 17 14
            },
      		//Indices
			{2, 1, 0,
             3, 4, 5,
             6, 7, 8,
             8, 9, 6,
             10, 11, 12,
             12, 13, 10,
             14, 15, 16,
             16, 17, 14},
		    0, //VAO
		    {0, 0}//VBOs
};
shape square = {
			{//Vertices
			Vec3(-0.25f, -0.25f, 0.00f), Vec3(0.5f, 0.5f, 0.5f), //0
			Vec3( 0.25f, -0.25f, 0.00f), Vec3(0.5f, 0.5f, 0.5f), //1
			Vec3( 0.25f,  0.25f, 0.00f), Vec3(0.5f, 0.5f, 0.5f), //2
			Vec3(-0.25f,  0.25f, 0.00f), Vec3(0.5f, 0.5f, 0.5f), //3

            Vec3(-0.25f, -0.25f, 0.75f), Vec3(0.0f, 0.0f, 0.0f), //4
            Vec3( 0.25f, -0.25f, 0.75f), Vec3(0.0f, 0.0f, 0.0f), //5
            Vec3( 0.25f,  0.25f, 0.75f), Vec3(0.0f, 0.0f, 0.0f), //6
            Vec3(-0.25f,  0.25f, 0.75f), Vec3(0.0f, 0.0f, 0.0f), //7

            Vec3(-0.25f, -0.25f, 0.00f), Vec3(0.1f, 0.1f, 0.1f), //8
            Vec3( 0.25f, -0.25f, 0.00f), Vec3(0.1f, 0.1f, 0.1f), //9
            Vec3( 0.25f, -0.25f, 0.75f), Vec3(0.1f, 0.1f, 0.1f), //10
            Vec3(-0.25f, -0.25f, 0.75f), Vec3(0.1f, 0.1f, 0.1f), //11

            Vec3(-0.25f,  0.25f, 0.00f), Vec3(0.2f, 0.2f, 0.2f), //12
            Vec3( 0.25f,  0.25f, 0.00f), Vec3(0.2f, 0.2f, 0.2f), //13
            Vec3( 0.25f,  0.25f, 0.75f), Vec3(0.2f, 0.2f, 0.2f), //14
            Vec3(-0.25f,  0.25f, 0.75f), Vec3(0.2f, 0.2f, 0.2f), //15

            Vec3(-0.25f, -0.25f, 0.00f), Vec3(0.3f, 0.3f, 0.3f), //16
            Vec3(-0.25f, -0.25f, 0.75f), Vec3(0.3f, 0.3f, 0.3f), //17
            Vec3(-0.25f,  0.25f, 0.75f), Vec3(0.3f, 0.3f, 0.3f), //18
            Vec3(-0.25f,  0.25f, 0.00f), Vec3(0.3f, 0.3f, 0.3f), //19

            Vec3( 0.25f, -0.25f, 0.00f), Vec3(0.4f, 0.4f, 0.4f), //20
            Vec3( 0.25f, -0.25f, 0.75f), Vec3(0.4f, 0.4f, 0.4f), //21
            Vec3( 0.25f,  0.25f, 0.75f), Vec3(0.4f, 0.4f, 0.4f), //22
            Vec3( 0.25f,  0.25f, 0.00f), Vec3(0.4f, 0.4f, 0.4f), //23

            },
			//indices
			{2, 1, 0,
			 0, 3, 2,
             4, 5, 6,
             6, 7, 4,
             8, 9, 10,
             10, 11, 8,
             14, 13, 12,
             12, 15, 14,
             16, 17, 18,
             18, 19, 16,
             22, 21, 20,
             20, 23, 22
             },
		0, //VAO
		{0, 0} //VBOs
};
shape parallelogram = {
						{//Vertices
                        	Vec3( 0.00f, -0.25f, 0.0f), Vec3(0.5f, 0.5f, 0.5f), //0
	                        Vec3( 0.50f, -0.25f, 0.0f), Vec3(0.5f, 0.5f, 0.5f), //1
	                        Vec3( 0.00f,  0.25f, 0.0f), Vec3(0.5f, 0.5f, 0.5f), //2
        	                Vec3(-0.50f,  0.25f, 0.0f), Vec3(0.5f, 0.5f, 0.5f), //3

                            Vec3( 0.00f, -0.25f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), //4
                            Vec3( 0.50f, -0.25f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), //5
                            Vec3( 0.00f,  0.25f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), //6
                            Vec3(-0.50f,  0.25f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), //7

                            Vec3( 0.00f, -0.25f, 0.0f), Vec3(0.1f, 0.1f, 0.1f), //8
                            Vec3( 0.50f, -0.25f, 0.0f), Vec3(0.1f, 0.1f, 0.1f), //9
                            Vec3( 0.00f, -0.25f, 1.0f), Vec3(0.1f, 0.1f, 0.1f), //10
                            Vec3( 0.50f, -0.25f, 1.0f), Vec3(0.1f, 0.1f, 0.1f), //11

                            Vec3( 0.00f,  0.25f, 0.0f), Vec3(0.2f, 0.2f, 0.2f), //12
                            Vec3(-0.50f,  0.25f, 0.0f), Vec3(0.2f, 0.2f, 0.2f), //13
                            Vec3( 0.00f,  0.25f, 1.0f), Vec3(0.2f, 0.2f, 0.2f), //14
                            Vec3(-0.50f,  0.25f, 1.0f), Vec3(0.2f, 0.2f, 0.2f), //15

                            Vec3( 0.00f, -0.25f, 1.0f), Vec3(0.3f, 0.3f, 0.3f), //16
                            Vec3(-0.50f,  0.25f, 0.0f), Vec3(0.3f, 0.3f, 0.3f), //17
                            Vec3( 0.00f, -0.25f, 0.0f), Vec3(0.3f, 0.3f, 0.3f), //18
                            Vec3(-0.50f,  0.25f, 1.0f), Vec3(0.3f, 0.3f, 0.3f), //19

                            Vec3( 0.50f, -0.25f, 1.0f), Vec3(0.4f, 0.4f, 0.4f), //20
                            Vec3( 0.50f, -0.25f, 0.0f), Vec3(0.4f, 0.4f, 0.4f), //21
                            Vec3( 0.00f,  0.25f, 0.0f), Vec3(0.4f, 0.4f, 0.4f), //22
                            Vec3( 0.00f,  0.25f, 1.0f), Vec3(0.4f, 0.4f, 0.4f), //23

                        },
      					//indices
						{2, 1, 0,
						2, 0, 3,
                        4, 5, 6,
                        6, 7, 4,
                        8, 9, 10,
                        10, 9, 11,
                        12, 13, 14,
                        14, 13, 15,
                        16, 17, 18,
                        16, 19, 17,
                        20, 21, 22,
                        22, 23, 20
                        },
			0, //VAO
			{0, 0} //VBOs
};


std::vector<shape> shapes = {triangle, square, parallelogram};

/////////////////////////////////////////////////////////////////////// ERRORS



void createShaderProgram()
{
	shaderProgram = new Shader("res/Quat3Dvertex.shader", "res/Quat3Dfragment.shader");

	shaderProgram->setAttribLocation("in_Position", VERTICES);
    shaderProgram->setAttribLocation("in_Shade", SHADES);

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
    GLfloat* shades;
	size_t vertex_i = 0;
	vertices = new GLfloat[s.indices.size()*4];
    shades = new GLfloat[s.indices.size()*4];
	for(size_t i=0; i<s.vertices.size();/*empty*/){
        Vec3 vertex = s.vertices[i++];
        Vec3 shade = s.vertices[i++];
		vertices[vertex_i] = vertex[0];
        shades[vertex_i++] = shade[0];

        vertices[vertex_i] = vertex[1];
        shades[vertex_i++] = shade[1];

        vertices[vertex_i] = vertex[2];
        shades[vertex_i++] = shade[2];

        vertices[vertex_i] = 1.0f;
        shades[vertex_i++] = 1.0f;
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
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex_i*2, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertex_i, vertices);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertex_i, sizeof(GLfloat)*vertex_i, shades);

		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, 0);

        glEnableVertexAttribArray(SHADES);
        glVertexAttribPointer(SHADES, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (GLvoid *)(sizeof(GLfloat)*vertex_i));

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
        glDisableVertexAttribArray(SHADES);
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

	CGJM::Mat4 R;
    CGJM::Mat4 V;
	if(gimbal){
		//order XYZ
		Vec3 rotatedUp = Vec3(0,1,0).rotate(Vec3(1, 0, 0), cameraPitch);
		Vec3 rotatedFront = Vec3(0, 0, -1).rotate(Vec3(1, 0, 0), cameraPitch).rotate(rotatedUp, cameraYaw);
		R = CGJM::rotate(rotatedFront, cameraRoll) *
				 CGJM::rotate(rotatedUp, cameraYaw) *
				 CGJM::rotate(Vec3(1, 0, 0), cameraPitch);

	}else{
		R = cameraOrientation.GLMatrix().transpose();
	}

	up = (R * Vec3(0, 1, 0)).normalize();
	front = (R * Vec3(0, 0, -1)).normalize();
	right = (R * Vec3(1, 0, 0)).normalize();

	V = CGJM::translate(0.0f, 0.0f, -cameraDistance) * R;

	CGJM::Mat4 VP = P*V; //VP part of MVP
    CGJM::Mat4 M;

    /*1 square*/
	glBindVertexArray(square.VAO);
	glUniform4f(colorUniform, 0.0f, 0.5f, 0.0f, 1.0f);
    M = CGJM::translate(-0.75f, -0.75f, 0.0f);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	glDrawElements(GL_TRIANGLES, (GLsizei)square.indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
	

	/*2 Small triangles, 2 big triangles and 1 medium triangle*/
	glBindVertexArray(triangle.VAO);
	glUniform4f(colorUniform, 0.5f, 0.0f, 0.0f, 1.0f);
	M = CGJM::translate(-0.25f, -0.75f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), 3*M_PI/2); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	glDrawElements(GL_TRIANGLES, (GLsizei)triangle.indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);


	glUniform4f(colorUniform, 0.0f, 0.0f, 0.5f, 1.0f);
	M = CGJM::translate(0.00f, -0.50f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), -M_PI/4)*CGJM::scale(1.414f, 1.414f, 1.414f); //Medium
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	glDrawElements(GL_TRIANGLES, (GLsizei)triangle.indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
	
	glUniform4f(colorUniform, 0.0f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(-0.50f, 0.00f, 0.0f)*CGJM::scale(2.0f, 2.0f, 2.0f);//Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	glDrawElements(GL_TRIANGLES, (GLsizei)triangle.indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
	
	glUniform4f(colorUniform, 0.5f, 0.5f, 0.0f, 1.0f);
	M = CGJM::translate(0.50f, 0.00f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), M_PI)*CGJM::scale(2.0f, 2.0f, 2.0f); //Big
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	glDrawElements(GL_TRIANGLES, (GLsizei)triangle.indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);

	glUniform4f(colorUniform, 0.5f, 0.5f, 0.5f, 1.0f);
	M = CGJM::translate(0.25f, -0.25f, 0.0f); //Small
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	glDrawElements(GL_TRIANGLES, (GLsizei)triangle.indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);


	/*1 parallelogram*/	
	glBindVertexArray(parallelogram.VAO);
	glUniform4f(colorUniform, 1.0f, 1.0f, 0.0f, 1.0f);
	M = CGJM::translate(0.75f, 0.0f, 0.0f)*CGJM::rotate(Vec3(0.0f, 0.0f, 1.0f), -M_PI/2.0f);
	glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, (VP*M).transpose());
	glDrawElements(GL_TRIANGLES, (GLsizei)parallelogram.indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);


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

void update(){
    glutWarpPointer(WinX/2, WinY/2);
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
    float cameraRate = (float)-1.0f*(M_PI / (WinX*10.0f));
    int deltaX = (WinX/2) - x;
    int deltaY = (WinY/2) - y;

	cameraYaw += deltaX*cameraRate;
	cameraPitch += deltaY*cameraRate;

	Quat pitchQuat = Quat(deltaY*cameraRate, right);
	Quat yawQuat = Quat(deltaX*cameraRate, up);

	cameraOrientation = pitchQuat * yawQuat * cameraOrientation;

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
	createShaderProgram();
	createBufferObjects(triangle);
	createBufferObjects(square);
	createBufferObjects(parallelogram);
	setupCallbacks();
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
