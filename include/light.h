#pragma once
#include "surfaceInteraction.h"
#include "vec3.h"
#include "visibilityTester.h"

class Light {
 protected:
  vec3 intensity;

 public:
  Light(const vec3 &_intensity) : intensity(_intensity) {}

  virtual vec3 getIntensity() const { return intensity; }

  /**
   * @brief Returns the contribution of the light to the hit surface
   * @param isect SurfaceInteraction of the object hit
   * @param vectorToLight this param is set to the vector from the surface to
   * the light, if any
   * @param vt this param is sets the visibilityTester for the current light
   * @return the intensity of the light for the point
   */
  virtual vec3 getIntensityForHit(const SurfaceInteraction &isect,
                                  vec3 *vectorToLight,
                                  VisibilityTester *vt) const = 0;

  virtual bool isAmbient() const { return false; };
};
