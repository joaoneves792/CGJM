//
// Created by joao on 11/12/17.
//

#ifndef CGJM_SPHERECAMERA_H
#define CGJM_SPHERECAMERA_H

#include "Camera.h"

class SphereCamera : public Camera{
    float distance;
public:
    SphereCamera(float distance, Vec3 center, Quat originalOrientation);
    virtual Mat4 getMatrix();
    virtual void move(float x, float y, float z);
    virtual void changeOrientation(float yaw, float pitch, float roll);
};
#endif //CGJM_SPHERECAMERA_H
