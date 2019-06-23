#include "visibilityTester.h"
#include "scene.h"
#include "surfaceInteraction.h"

#define DELTA 0.001

Ray VisibilityTester::createRay() {
  vec3 direction = siDest.p - siOrig.p;
  // origin + delta of its normal
  vec3 origin = siOrig.p + (unit_vector(siOrig.n) * DELTA);
  Ray r(origin, direction, 0, 0);
  if (hasTLimit) r.maxT = 1.0;
  return r;
}

bool VisibilityTester::isOccluded(const Scene &scene) {
  SurfaceInteraction si;
  return scene.intersect(createRay(), &si);
}