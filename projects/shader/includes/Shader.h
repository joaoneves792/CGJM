#ifndef SHADER_H_
#define SHADER_H_

#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>

class Shader{
private:
	GLuint _shaderProgram;
	GLuint _vertexShader;
	GLuint _fragmentShader;
public:
	Shader(const char* path_vert_shader, const char* path_frag_shader);
	virtual ~Shader();
	
	void link();
	void use();
	void clean();
	GLuint getShader();
	GLint getAttribLocation(const char* name);
	GLint getUniformLocation(const char* name);
	void setAttribLocation(const char* name, GLuint position);
	void create_program(const char *path_vert_shader, const char *path_frag_shader);
private:
	GLuint load_and_compile_shader(const char *fname, GLenum shaderType);
	void read_shader_src(const char *fname, std::vector<char> &buffer);
};


#endif /* SHADER_H_*/

