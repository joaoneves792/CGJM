#version 330 core

in vec3 inPosition;
in vec3 inNormal;
out vec3 fragmentPosition;

uniform mat4 MVP;

void main(void){
	fragmentPosition = inPosition;
	gl_Position = MVP * vec4(inPosition, 1.0);
}

