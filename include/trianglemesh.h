#pragma once

#include <memory>
#include <vector>
#include "material.h"
#include "point2.h"
#include "primitive.h"
#include "vec3.h"

class Triangle;

struct TriangleMesh {
  const int nTriangles;  //!< # of triangles in the mesh.
  // The size of the list below should be 3 * nTriangles. Every 3 values we have
  // a triangle.
  const int nVertices;
  std::vector<int> vertexIndices;  //!< The list of indices that make up each
                                   //!< individual triangle.

  std::shared_ptr<Material> material;
  // He comes the data associated with each vertex.
  std::shared_ptr<point3[]> points;  //!< The 3D geometric coordinates
  std::shared_ptr<vec3[]> normals;   //!< The 3D normals.
  std::shared_ptr<Point2[]>
      uv;  //!< The 2D texture coordinages (might be 3D, if necessary).

  // Regular constructor
  TriangleMesh(int nTri, const int *indices, int nVertices,
               const point3 *_points, const vec3 *_normals,
               const std::shared_ptr<Material> &material)
      : nTriangles(nTri),
        nVertices(nVertices),
        vertexIndices(indices, indices + 3 * nTri),
        material(material) {
    points.reset(new point3[nVertices]);
    for (int i = 0; i < nVertices; ++i) points[i] = _points[i];

    normals.reset(new vec3[nVertices]);
    for (int i = 0; i < nVertices; ++i) normals[i] = _normals[i];
  }

  static std::vector<std::shared_ptr<Primitive>> getTriangles(
      const TriangleMesh &mesh);
};
