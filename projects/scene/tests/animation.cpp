//
// Created by joao on 11/13/17.
//

#include "CGJengine.h"
#include "scene.h"
#include "animation.h"

#define interpolate(v1, v2, t)  (1 - t) * v1 + t * v2

keyframe original[TANGRAM_PIECES_COUNT] = {
        {-0.75f, 1.0f, -0.75f, 0.0f}, //square
        {0.0f,   1.0f, 0.75f,  0.0f}, //parallelogram
        {-0.75f, 1.0f, -0.25f, M_PI}, //small tri 1
        {-0.25f, 1.0f, 0.25f,  -M_PI / 2.0f}, //small tri 2
        {-0.50f, 1.0f, 0.0f,   5.0f * M_PI / 4.0f}, //medium tri
        {0.0f,   1.0f, -0.50f, -M_PI / 2.0f}, //large tri 1
        {0.0f,   1.0f, 0.50f,  M_PI / 2.0f} //large tri 2
};
keyframe floating[TANGRAM_PIECES_COUNT] = {
        {-0.75f, 1.00f, -0.75f, 0.0f}, //square
        {0.0f,   1.50f, 0.75f,  0.0f}, //parallelogram
        {-0.75f, 2.00f, -0.25f, M_PI}, //small tri 1
        {-0.25f, 2.50f, 0.25f,  -M_PI / 2.0f}, //small tri 2
        {-0.50f, 3.00f, 0.0f,   5.0f * M_PI / 4.0f}, //medium tri
        {0.0f,   3.50f, -0.50f, -M_PI / 2.0f}, //large tri 1
        {0.0f,   4.00f, 0.50f,  M_PI / 2.0f} //large tri 2
};
keyframe floatingBox[TANGRAM_PIECES_COUNT] = {
        {0.00f,   1.00f, -0.06f, M_PI / 4.0f}, //square
        {0.53f,   1.50f, 0.47f,   -M_PI / 4.0f}, //parallelogram
        {0.35f,   2.00f, 0.30f,   5.0f * M_PI / 4.0f}, //small tri 1
        {-0.355f, 2.50f, -0.415f, -M_PI / 4.0f}, //small tri 2
        {0.35f,   3.00f, -0.06f, M_PI / 2.0f}, //medium tri
        {0.0f,    3.50f, 1.00f,   3.0f * M_PI / 4.0f}, //large tri 1
        {-0.71f,  4.00f, 0.29f,   9.0f * M_PI / 4.0f} //large tri 2
};
keyframe box[TANGRAM_PIECES_COUNT] = {
        {0.00f,   1.00f, -0.06f, M_PI / 4.0f}, //square
        {0.53f,   1.00f, 0.47f,   -M_PI / 4.0f}, //parallelogram
        {0.35f,   1.00f, 0.30f,   5.0f * M_PI / 4.0f}, //small tri 1
        {-0.355f, 1.00f, -0.415f, -M_PI / 4.0f}, //small tri 2
        {0.35f,   1.00f, -0.06f, M_PI / 2.0f}, //medium tri
        {0.0f,    1.00f, 1.00f,   3.0f * M_PI / 4.0f}, //large tri 1
        {-0.71f,  1.00f, 0.29f,   9.0f * M_PI / 4.0f} //large tri 2
};



keyframe* animations[2][ANIMATION_KEYFRAME_COUNT]= { {original, floating, floatingBox, box},
                                                     {box, floatingBox, floating, original} };
bool animationFinished = true;

extern SceneNode* tangramNodes[];

/////////////////////////////////////////////////////////////////////// Animation
void applyAnimation(keyframe *last, keyframe *current, float k) {
    for(int i=0; i < TANGRAM_PIECES_COUNT; i++){
        SceneNode* node = tangramNodes[i];
        node->setPosition(
                interpolate(last[i].position[0], current[i].position[0], k),
                interpolate(last[i].position[1], current[i].position[1], k),
                interpolate(last[i].position[2], current[i].position[2], k)
        );
        Quat prevQuat = Quat(last[i].angle, Vec3(0.0f, 1.0f, 0.0f));
        Quat nextQuat = Quat(current[i].angle, Vec3(0.0f, 1.0f, 0.0f));
        node->setOrientation(slerp(prevQuat, nextQuat, k));
    }
}

void animate(){
    if(animationFinished){
        animationFinished = false;
    }
}


void updateScene(int dt){
    static int k = 0;
    static float keyframeStatus = 1.0f;
    static keyframe* currentKeyframe = original;
    static keyframe* lastKeyframe = original;
    static int currentAnimation = 0;
    static int lastAnimation = 1;

    if(animationFinished)
        return;

    //If current keyframe is complete
    if(keyframeStatus >= 1.0) {
        //If we reached the end of the animation
        if (k+1 >= ANIMATION_KEYFRAME_COUNT) {
            k = 0;
            int tmp = currentAnimation; //Flip the animations
            currentAnimation = lastAnimation;
            lastAnimation = tmp;
            animationFinished = true;
            keyframeStatus = 1.0f;
        } else {
            lastKeyframe = currentKeyframe;
            currentKeyframe = animations[currentAnimation][++k];
            keyframeStatus = 0.0f;
        }
    }

    keyframeStatus += (float)dt / (float)KEYFRAME_DURATION;
    if(keyframeStatus > 1.0f)
        keyframeStatus = 1.0f;

    applyAnimation(lastKeyframe, currentKeyframe, keyframeStatus);
}

