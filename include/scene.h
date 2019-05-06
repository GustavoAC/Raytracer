#pragma once

#include <memory>
#include <vector>
#include "background.h"
#include "light.h"
#include "primitive.h"

class Scene {
  //=== Public data
 public:
  std::vector<std::shared_ptr<Light>> lights;  // list of lights
  std::shared_ptr<Background> background;      // The background object.
 private:
  std::shared_ptr<Primitive>
      aggregate;  // The scene graph of objects, acceleration structure.

  //=== Public interface
 public:
  Scene(std::shared_ptr<Primitive> ag,
        const std::vector<std::shared_ptr<Light>>& ls,
        const std::shared_ptr<Background>& _background)
      : lights{ls}, background{_background}, aggregate{ag} { /* empty */
  }
  /// Determines the intersection info; return true if there is an intersection.
  bool intersect(const Ray& r, SurfaceInteraction* isect) const {
    return aggregate->intersect(r, isect);
  }
  /*! A faster version that only determines whether there is an intersection or
   * not; it doesn't calculate the intersection info.
   */
  bool intersect_p(const Ray& r) const { return aggregate->intersect(r); }
};