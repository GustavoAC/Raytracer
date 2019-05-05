#pragma once

#include <memory>
#include "material.h"
#include "ray.h"

class SurfaceInteraction;

class Primitive {
 public:
  virtual bool intersect(const Ray& r, SurfaceInteraction* si) const = 0;

  // Simpler & faster version of intersection that only return true/false.
  // It does not compute the hit point information.
  virtual bool intersect(const Ray& r) const = 0;
};