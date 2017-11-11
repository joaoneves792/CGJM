#version 330 core

in vec3 inPosition;
in vec3 inNormal;

out float shade;

uniform mat4 MVP;

void main(void){
    shade = inNormal[0];
	gl_Position = MVP * vec4(inPosition, 1.0);
}

