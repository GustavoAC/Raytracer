#pragma once

#include "vec3.h"

class Ray {
 private:
  point3 origin;
  vec3 direction;
  float i;
  float j;

 public:
  Ray(const point3 &_origin, const vec3 &_direction, const int &_i,
      const int &_j)
      : origin(_origin), direction(_direction), i(_i), j(_j) {
    /* Empty */
  }

  vec3 operator()(float t) const { return origin + (t * direction); }

  point3 getOrigin() const { return origin; }
  vec3 getDirection() const { return direction; }
  int getI() const { return i; }
  int getJ() const { return j; }

  friend std::ostream &operator<<(std::ostream &os, const Ray &t) {
    os << "[o=" << t.origin << ", d=" << t.direction << "]";
    return os;
  }
};