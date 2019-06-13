#include "light.h"
#include "vec3.h"

class DirectionalLight : public Light {
 private:
  vec3 direction;

 public:
  DirectionalLight(const vec3 &intensity, const vec3 &direction)
      : Light(intensity), direction(direction) {}

  vec3 getDirection() const { return direction; }

  virtual vec3 getIntensityForHit(const SurfaceInteraction &isect,
                                  vec3 *vectorToLight,
                                  VisibilityTester *vt) const {
    *vectorToLight = unit_vector(direction);
    point3 position = isect.p + *vectorToLight;

    SurfaceInteraction si;
    si.p = position;
    *vt = VisibilityTester(isect, si, false);

    return intensity;
  }
};