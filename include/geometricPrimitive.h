#pragma once
#include "primitive.h"

class GeometricPrimitive : public Primitive {
 protected:
  std::shared_ptr<Material> material;

 public:
  GeometricPrimitive(const std::shared_ptr<Material>& _material)
      : material(_material) {}

  virtual const std::shared_ptr<Material> getMaterial(void) const {
    return material;
  }

  virtual bool intersect(const Ray& r, SurfaceInteraction* si) const = 0;

  // Simpler & faster version of intersection that only return true/false.
  // It does not compute the hit point information.
  virtual bool intersect(const Ray& r) const = 0;
};