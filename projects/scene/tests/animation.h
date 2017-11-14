//
// Created by joao on 11/13/17.
//

#ifndef CGJM_ANIMATION_H
#define CGJM_ANIMATION_H

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

#define KEYFRAME_DURATION 2000.0f; //in milliseconds
#define ANIMATION_KEYFRAME_COUNT 4

typedef struct {
    float position[3];
    float angle;
}keyframe;

void applyAnimation(keyframe* last, keyframe* current, float k);
void animate();
void updateScene(int dt);

#endif //CGJM_ANIMATION_H
