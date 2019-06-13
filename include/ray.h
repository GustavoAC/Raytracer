#pragma once
#include <limits>
#include "vec3.h"

class Ray {
 private:
  point3 origin;
  vec3 direction;
  double i;
  double j;

 public:
  Ray(const point3 &_origin, const vec3 &_direction, const double &_i,
      const double &_j)
      : origin(_origin), direction(_direction), i(_i), j(_j) {
    /* Empty */
  }

  //! Maximum acceptable t value for the ray
  float maxT = std::numeric_limits<float>::max();

  vec3 operator()(float t) const { return origin + (t * direction); }

  point3 getOrigin() const { return origin; }
  vec3 getDirection() const { return direction; }
  double getI() const { return i; }
  double getJ() const { return j; }

  friend std::ostream &operator<<(std::ostream &os, const Ray &t) {
    os << "[o=" << t.origin << ", d=" << t.direction << "]";
    return os;
  }
};