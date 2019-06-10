#pragma once
#include "light.h"
#include "vec3.h"

class AmbientLight : public Light {
 public:
  AmbientLight(const vec3 &_intensity) : Light(_intensity) {}
  vec3 getIntensityForHit(const SurfaceInteraction &isect, vec3 *vectorToLight,
                          VisibilityTester *vt) const {
    return intensity;
  }

  bool isAmbient() const { return true; }
};