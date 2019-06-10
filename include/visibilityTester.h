#pragma once
#include "ray.h"
#include "surfaceInteraction.h"

class Scene;

class VisibilityTester {
 private:
  SurfaceInteraction siOrig;
  SurfaceInteraction siDest;

  Ray createRay();

 public:
  VisibilityTester(const SurfaceInteraction &_siOrig,
                   const SurfaceInteraction &_siDest)
      : siOrig(_siOrig), siDest(_siDest) {}

  bool isOccluded(const Scene &scene);
};