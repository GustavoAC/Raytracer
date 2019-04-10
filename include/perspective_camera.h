#pragma once

#include "camera.h"
#include <cmath>

class PerspectiveCamera : public Camera {
   private:
        float focalDistance;
        float verticalAOV;
        float aspectRatio;

   public:
    PerspectiveCamera(int _width, int _height, const point3 &_position, const point3 &_target,
                       const vec3 &_vUp, const float &_verticalAOV, const float &_aspectRatio, const float &_focalDistance = 1)
        : Camera(_width, _height, _position, _target, _vUp), focalDistance(_focalDistance) {
        float halfHeight = std::tan(M_PI * _verticalAOV / 180.0 / 2.0) * _focalDistance;
        float halfWidth = halfHeight * _aspectRatio;
        l = -halfWidth;
        r = halfWidth;
        b = -halfHeight;
        t = halfHeight;
    }

    Ray generateRay(double i, double j);
};