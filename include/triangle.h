#pragma once

#include <memory>
#include "bounds3.h"
#include "material.h"
#include "point2.h"
#include "shape.h"
#include "surfaceInteraction.h"
#include "trianglemesh.h"

#define EPSILON 0.001

class Triangle : public Shape {
 private:
  const int *v;  // Pointer to an index array stored elsewhere (triangle mesh)
                 // in memory.
  // After the proper triangle initialization, we may access v[0], v[1], and
  // v[2]
  std::shared_ptr<const TriangleMesh> mesh;
  bool backface_cull;  // Set it `true` to turn backface culling.

 public:
  Triangle(std::shared_ptr<const TriangleMesh> mesh,
           std::shared_ptr<Material> material, int tri_id, bool bfc = false)
      : Shape(material), mesh(mesh), backface_cull(bfc) {
    v = &mesh->vertexIndices[3 * tri_id];
  }

  virtual bool intersect(const Ray &ray, SurfaceInteraction *isect) const {
    auto v0 = mesh->points[v[0]];
    auto v1 = mesh->points[v[1]];
    auto v2 = mesh->points[v[2]];
    vec3 edge1, edge2, tvec, pvec, qvec;
    float det, invDet, t, u, vFloat;

    // pvec = h
    edge1 = v1 - v0;
    edge2 = v2 - v0;
    pvec = cross(ray.getDirection(), edge2);
    det = dot(edge1, pvec);

    if (backface_cull) {
      if (det < EPSILON)
        return false;  // This ray is parallel to this triangle.

      tvec = ray.getOrigin() - v0;
      u = dot(tvec, pvec);
      if (u < 0.0 || u > 1.0) return false;

      qvec = cross(tvec, edge1);
      vFloat = dot(ray.getDirection(), qvec);
      if (vFloat < 0.0 || u + vFloat > 1.0) return false;

      t = dot(edge2, qvec);
      invDet = 1.0 / det;
      t *= invDet;
      u *= invDet;
      vFloat *= invDet;
    } else {
      if (det > -EPSILON && det < EPSILON)
        return false;  // This ray is parallel to this triangle.
      invDet = 1.0 / det;

      tvec = ray.getOrigin() - v0;
      u = dot(tvec, pvec) * invDet;
      if (u < 0.0 || u > 1.0) return false;

      qvec = cross(tvec, edge1);
      vFloat = dot(ray.getDirection(), qvec) * invDet;
      if (vFloat < 0.0 || u + vFloat > 1.0) return false;

      t = dot(edge2, qvec) * invDet;
    }

    if (t > ray.maxT) return false;
    if (isect != nullptr) {
      isect->p = ray(t);
      isect->uv = Point2(u, vFloat);
      isect->wo = -1.0f * ray.getDirection();
      isect->t = t;
      isect->primitive = this;

      auto n0 = mesh->normals[v[0]];
      auto n1 = mesh->normals[v[1]];
      auto n2 = mesh->normals[v[2]];

      isect->n =
          unit_vector(((1 - u - vFloat) * n0) + (u * n1) + (vFloat * n2));
    }

    return true;
  }

  virtual bool intersect(const Ray &ray) const {
    return this->intersect(ray, nullptr);
  }

  // Return the triangle's bounding box.
  virtual Bounds3 getBounds() const {
    auto p0 = mesh->points[v[0]];
    auto p1 = mesh->points[v[1]];
    auto p2 = mesh->points[v[2]];

    auto bounds = Bounds3(p0, p1);
    return bounds.getUnion(bounds, p2);
  }
};