#include "visibilityTester.h"
#include "scene.h"

Ray VisibilityTester::createRay() {
  vec3 direction = siDest.p - siOrig.p;
  return Ray(siDest.p, direction, 0, 0);
}

bool VisibilityTester::isOccluded(const Scene &scene) {
  return scene.intersect_p(createRay());
}