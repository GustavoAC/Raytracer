#pragma once
#include "geometricPrimitive.h"

class Shape : public GeometricPrimitive {
 public:
  Shape(const std::shared_ptr<Material>& _material)
      : GeometricPrimitive(_material) {}

  virtual bool intersect(const Ray& r, SurfaceInteraction* si) const = 0;

  // Simpler & faster version of intersection that only return true/false.
  // It does not compute the hit point information.
  virtual bool intersect(const Ray& r) const = 0;
};