#version 330 core

in vec4 shade;
out vec4 out_Color;
uniform vec4 color;

void main(void){
       out_Color = color+shade;
}

