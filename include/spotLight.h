#include <cmath>
#include "light.h"
#include "vec3.h"

#define RADIAN 0.01745329252  // single degree as radian

class SpotLight : public Light {
 private:
  point3 position;
  point3 point_at;
  int cutoff;
  int falloff;

 public:
  SpotLight(point3 intensity, point3 position, point3 point_at, int cutoff,
            int falloff)
      : Light(intensity),
        position(position),
        point_at(point_at),
        cutoff(cutoff),
        falloff(falloff) {}

  point3 getPosition() const { return position; }
  point3 getPointAt() const { return point_at; }
  int getCutoff() const { return cutoff; }
  int getFalloff() const { return falloff; }

  vec3 getIntensityForHit(const SurfaceInteraction &isect, vec3 *vectorToLight,
                          VisibilityTester *vt) const {
    *vectorToLight = unit_vector(this->position - isect.p);
    SurfaceInteraction si;
    si.p = this->position;
    *vt = VisibilityTester(isect, si);

    auto w = -*vectorToLight;

    auto cos_cutoff = cos(cutoff * RADIAN);
    auto cos_falloff = cos(falloff * RADIAN);

    vec3 light_vector = point_at - position;
    float theta = dot(light_vector, w) / (light_vector.length() * w.length());

    if ((theta) < cos_cutoff) return vec3(0, 0, 0);
    if ((theta) > cos_falloff) return intensity;

    float delta = (theta - cos_cutoff) / (cos_falloff - cos_cutoff);
    return pow(delta, 4) * intensity;
  }
};
