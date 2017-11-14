#version 330 core

in vec3 fragmentPosition;
out vec4 out_Color;

void main(void){
    vec2 pos2d = fragmentPosition.xz;
    float color = 1.0f-(length(pos2d)/10.0f);
    if(color > 0.8){
        color = 1.0f;
    }
    if(color < 0.1f){
        color = 0.1f;
    }

    if(length(pos2d)<0.25f){
        color = 0.0f;
    }
    out_Color = vec4(color, color, color, 1.0f);
}

