//
// Created by joao on 11/13/17.
//

#ifndef CGJM_SCENE_H
#define CGJM_SCENE_H

#ifndef M_PI
#define M_PI 3.14159265359f
#endif

#define TANGRAM_PIECES_COUNT 7

enum tangram{
    tan_square = 0,
    tan_parallelogram = 1,
    tan_s_triangle1 = 2,
    tan_s_triangle2 = 3,
    tan_m_triangle = 4,
    tan_l_triangle1 = 5,
    tan_l_triangle2 = 6
};

typedef struct {
    float position[3];
    float angle;
}keyframe;



SceneGraph* setupScene();
void switchKeyframe(keyframe* kf);
void updateScene(int dt);
void destroyScene(SceneGraph* scene);

#endif //CGJM_SCENE_H
