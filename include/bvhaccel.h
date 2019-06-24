#pragma once
#include <algorithm>
#include <vector>
#include "aggregatePrimitive.h"
#include "bounds3.h"
#include "ray.h"
#include "vec3.h"

class BVHAccel : public AggregatePrimitive {
 private:
  Bounds3 bounds;
  std::shared_ptr<Primitive> left;
  std::shared_ptr<Primitive> right;

 public:
  BVHAccel() {}
  BVHAccel(std::vector<std::shared_ptr<Primitive>>& p, int axis = 0);
  static bool compareLessAtAxis(const std::shared_ptr<Primitive>& p1,
                                const std::shared_ptr<Primitive>& p2,
                                const int axis);
  virtual bool intersect(const Ray& r, SurfaceInteraction* si) const;
  virtual bool intersect(const Ray& r) const;
  virtual Bounds3 getBounds() const { return bounds; }
};

bool BVHAccel::intersect(const Ray& r, SurfaceInteraction* si) const {
  // std::cout << bounds.pMin[0] << " " << bounds.pMax[0] << " | "
  //           << bounds.pMin[1] << " " << bounds.pMax[1] << " | "
  //           << bounds.pMin[2] << " " << bounds.pMax[2] << "\n";
  if (bounds.intersect(r)) {
    SurfaceInteraction siLeft, siRight;
    bool hitLeft = left != nullptr && left->intersect(r, &siLeft);
    bool hitRight = right != nullptr && right->intersect(r, &siRight);
    if (hitLeft && hitRight) {
      *si = (siLeft.t < siRight.t) ? siLeft : siRight;
      return true;
    }

    if (hitLeft) {
      *si = siLeft;
      return true;
    }

    if (hitRight) {
      *si = siRight;
      return true;
    }
    return false;
  }
  return false;
}

bool BVHAccel::compareLessAtAxis(const std::shared_ptr<Primitive>& p1,
                                 const std::shared_ptr<Primitive>& p2,
                                 const int axis) {
  return p1->getBounds().pMin[axis] < p2->getBounds().pMin[axis];
}

BVHAccel::BVHAccel(std::vector<std::shared_ptr<Primitive>>& p, int axis) {
  std::sort(p.begin(), p.end(),
            [axis](const std::shared_ptr<Primitive>& p1,
                   const std::shared_ptr<Primitive>& p2) {
              return compareLessAtAxis(p1, p2, axis);
            });

  if (p.size() == 1) {
    left = p[0];
    bounds = left->getBounds();
    return;
  } else if (p.size() == 2) {
    left = p[0];
    right = p[1];
  } else {
    std::vector<std::shared_ptr<Primitive>> firstHalf(p.begin(),
                                                      p.begin() + p.size() / 2);
    std::vector<std::shared_ptr<Primitive>> secondHalf(p.begin() + p.size() / 2,
                                                       p.end());
    left = std::shared_ptr<Primitive>(new BVHAccel(firstHalf, (axis + 1) % 3));
    right =
        std::shared_ptr<Primitive>(new BVHAccel(secondHalf, (axis + 1) % 3));
  }
  bounds = Bounds3::getUnion(left->getBounds(), right->getBounds());
}

bool BVHAccel::intersect(const Ray& r) const {
  SurfaceInteraction si;
  intersect(r, &si);
}