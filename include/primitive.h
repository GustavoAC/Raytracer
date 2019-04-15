#pragma once

#include <memory>
#include "ray.h"
#include "material.h"

class SurfaceInteraction;

class Primitive {
   private:
    std::shared_ptr<Material> material;

   public:
    virtual bool intersect(const Ray& r, SurfaceInteraction* si) const = 0;

    // Simpler & faster version of intersection that only return true/false.
    // It does not compute the hit point information.
    virtual bool intersect(const Ray& r) const = 0;

    virtual const std::shared_ptr<Material> get_material(void) const { return material; }
};