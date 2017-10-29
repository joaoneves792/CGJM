#version 330 core

in vec4 in_Position;
in float in_Shade;

out float shade;

uniform mat4 MVP;

void main(void){
    shade = in_Shade;
	gl_Position = MVP * in_Position;
}

