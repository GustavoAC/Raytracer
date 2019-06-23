#pragma once

#include "ray.h"
#include "vec3.h"

class Bounds3 {
 public:
  vec3 pMin, pMax;

  Bounds3() {
    float minNum = std::numeric_limits<float>::lowest();
    float maxNum = std::numeric_limits<float>::max();
    pMin = point3(maxNum, maxNum, maxNum);
    pMax = point3(minNum, minNum, minNum);
  }

  // Might be unused
  Bounds3(const point3& p) : pMin{p}, pMax{p} {}

  Bounds3(const point3& p1, const point3& p2)
      : pMin(point3(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
                    std::min(p1.z(), p2.z()))),
        pMax(point3(std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()),
                    std::max(p1.z(), p2.z()))) {}

  static Bounds3 getUnion(const Bounds3& b, const point3& p) {
    return Bounds3(
        point3(std::min(b.pMin.x(), p.x()), std::min(b.pMin.y(), p.y()),
               std::min(b.pMin.z(), p.z())),
        point3(std::max(b.pMax.x(), p.x()), std::max(b.pMax.y(), p.y()),
               std::max(b.pMax.z(), p.z())));
  };

  static Bounds3 getUnion(const Bounds3& b1, const Bounds3& b2) {
    return Bounds3(point3(std::min(b1.pMin.x(), b2.pMin.x()),
                          std::min(b1.pMin.y(), b2.pMin.y()),
                          std::min(b1.pMin.z(), b2.pMin.z())),
                   point3(std::max(b1.pMax.x(), b2.pMax.x()),
                          std::max(b1.pMax.y(), b2.pMax.y()),
                          std::max(b1.pMax.z(), b2.pMax.z())));
  };

  inline bool intersect(const Ray& ray) const {
    for (int i = 0; i < 3; ++i) {
      float invD = 1.0f / ray.getDirection()[i];
      float t0 = (pMin[i] - ray.getOrigin()[i]) * invD;
      float t1 = (pMax[i] - ray.getOrigin()[i]) * invD;
      if (invD < 0) std::swap(t0, t1);
      if (t0 > t1) return true;
    }
    return false;
  }
};