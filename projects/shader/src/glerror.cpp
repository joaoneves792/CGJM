#include <GL/glew.h>
#include <string>
#include <iostream>
#include "GL/glut.h"
#include "glerrors.h"

bool isOpenGLError() {
        bool isError = false;
        GLenum errCode;
        const GLubyte *errString;
        while ((errCode = glGetError()) != GL_NO_ERROR) {
                isError = true;
                errString = gluErrorString(errCode);
                std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
        }
        return isError;
}

void checkOpenGLError(std::string error)
{
        if(isOpenGLError()) {
                std::cerr << error << std::endl;
                exit(EXIT_FAILURE);
        }
}

