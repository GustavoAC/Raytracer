#pragma once

#include "camera.h"
#include "iostream"

class OrthographicCamera : public Camera {
 public:
  OrthographicCamera(int _width, int _height, const point3 &_position,
                     const point3 &_target, const vec3 &_vUp, double _l,
                     double _r, double _b, double _t)
      : Camera(_width, _height, _position, _target, _vUp) {
    l = _l;
    r = _r;
    b = _b;
    t = _t;
  }

  Ray generateRay(double i, double j) const;
};