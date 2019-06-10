#include "light.h"
#include "vec3.h"

class PointLight : public Light {
 private:
  vec3 position;

 public:
  PointLight(const vec3 &_intensity, const vec3 &_position)
      : Light(_intensity), position(_position) {}

  vec3 getIntensityForHit(const SurfaceInteraction &isect, vec3 *vectorToLight,
                          VisibilityTester *vt) const {
    *vectorToLight = unit_vector(this->position - isect.p);

    SurfaceInteraction si;
    si.p = this->position;
    *vt = VisibilityTester(isect, si);

    return this->intensity;
  }
};