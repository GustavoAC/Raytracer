#pragma once
#include "ray.h"
#include "surfaceInteraction.h"

class Scene;

class VisibilityTester {
 private:
  SurfaceInteraction siOrig;
  SurfaceInteraction siDest;
  bool hasTLimit;

  Ray createRay();

 public:
  VisibilityTester(const SurfaceInteraction &_siOrig,
                   const SurfaceInteraction &_siDest,
                   const bool &hasTLimit = true)
      : siOrig(_siOrig), siDest(_siDest), hasTLimit(hasTLimit) {}

  bool isOccluded(const Scene &scene);
};