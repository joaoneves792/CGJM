#version 330 core

in vec4 in_Position;
uniform mat4 MVP;

void main(void){
	gl_Position = MVP * in_Position;
}

