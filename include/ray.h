#pragma once

#include "vec3.h"

class Ray {
   private:
    point3 origin;
    vec3 direction;

   public:
    Ray(const point3 &_origin, const vec3 &_direction) : origin(_origin), direction(_direction) {
        /* Empty */
    }

    vec3 operator()(float t) const { return origin + (t * direction); }

    point3 getOrigin() const { return origin; }
    vec3 getDirection() const { return direction; }

    friend std::ostream &operator<<(std::ostream &os, const Ray &t) {
        os << "[o=" << t.origin << ", d=" << t.direction << "]";
        return os;
    }
};