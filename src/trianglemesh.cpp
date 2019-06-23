#include "trianglemesh.h"
#include "triangle.h"

std::vector<std::shared_ptr<Primitive>> TriangleMesh::getTriangles(
    const TriangleMesh &mesh) {
  std::shared_ptr<const TriangleMesh> meshPointer =
      std::make_shared<const TriangleMesh>(mesh);
  std::vector<std::shared_ptr<Primitive>> triangles;
  for (int i = 0; i < mesh.nTriangles; ++i) {
    triangles.push_back(std::shared_ptr<Primitive>(
        new Triangle(meshPointer, mesh.material, i)));
  }

  return triangles;
}