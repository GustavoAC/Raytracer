#include "flatIntegrator.h"
#include "flatMaterial.h"
#include "ray.h"
#include "surfaceInteraction.h"

// This method must be overriden by all classes derived from SamplerIntegrator.
/// Determines a color for the incoming ray.
Pixel FlatIntegrator::Li(const Ray& ray, const Scene& scene, Sampler& sampler) {
  Pixel L(0, 0, 0);  // The radiance
  // Find closest ray intersection or return background radiance.
  SurfaceInteraction isect;
  if (!scene.intersect(ray, &isect)) {
    // This might be just:
    L = scene.background->getColor(ray.getI(), ray.getJ());
  } else {
    // Some form of determining the incoming radiance at the ray's origin.
    // For this integrator, it might just be:
    // Polymorphism in action.
    FlatMaterial* fm =
        dynamic_cast<FlatMaterial*>(isect.primitive->getMaterial().get());
    // Assign diffuse color to L.
    L = fm->getColor();
  }

  return L;
}