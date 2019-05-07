#include "normalMapIntegrator.h"
#include "surfaceInteraction.h"
#include "vec3.h"

Pixel NormalMapIntegrator::Li(const Ray& ray, const Scene& scene,
                              Sampler& sampler) const {
  SurfaceInteraction isect;
  if (!scene.intersect(ray, &isect)) {
    // This might be just:
    return scene.background->getColor(ray.getI(), ray.getJ());

    isect.n = unit_vector(isect.n);
    vec3 modifiedN =
        0.5 * vec3(isect.n.x() + 1, isect.n.y() + 1, isect.n.z() + 1);
    return Pixel(255 * modifiedN.x(), 255 * modifiedN.y(), 255 * modifiedN.z());
  }