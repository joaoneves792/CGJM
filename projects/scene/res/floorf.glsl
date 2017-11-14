#version 330 core

smooth in vec3 fragmentPosition;
out vec4 out_Color;

void main(void){
    vec2 pos2d = fragmentPosition.xz;
    float color = 1.0f-(length(pos2d)/10.0f);
    if(color > 0.7){
        color = 1.0f;
    }

    out_Color = vec4(color, color, color, 1.0f);
}

