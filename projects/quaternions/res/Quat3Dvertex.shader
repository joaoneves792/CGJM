#version 330 core

in vec4 in_Position;
in vec4 in_Shade;

out vec4 shade;

uniform mat4 MVP;

void main(void){
    shade = in_Shade;
	gl_Position = MVP * in_Position;
}

