#pragma once
#include <memory>
#include <vector>
#include "primitive.h"

class AggregatePrimitive : public Primitive {
 public:
  std::vector<std::shared_ptr<Primitive>> primitives;

  virtual bool intersect(const Ray& r, SurfaceInteraction* si) const {
    std::shared_ptr<SurfaceInteraction> bestSi = nullptr;
    SurfaceInteraction currentSi;
    for (auto& p : primitives) {
      if (p->intersect(r, &currentSi)) {
        if (bestSi == nullptr)
          bestSi = std::make_shared<SurfaceInteraction>(currentSi);
        else if (currentSi.t < bestSi->t) {
          *bestSi = currentSi;
        }
      }
    }

    if (bestSi != nullptr) {
      *si = *bestSi;
      return true;
    }

    return false;
  }

  // Simpler & faster version of intersection that only return true/false.
  // It does not compute the hit point information.
  virtual bool intersect(const Ray& r) const {
    for (auto& p : primitives) {
      if (p->intersect(r)) return true;
    }
    return false;
  }
}