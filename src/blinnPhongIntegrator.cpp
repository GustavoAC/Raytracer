#include "blinnPhongIntegrator.h"
#include <cmath>
#include "blinnMaterial.h"
#include "surfaceInteraction.h"
#include "visibilityTester.h"

Pixel BlinnPhongIntegrator::Li(const Ray& ray, const Scene& scene,
                               Sampler& sampler) const {
  SurfaceInteraction isect;
  if (!scene.intersect(ray, &isect)) {
    // This might be just:
    return scene.background->getColor(ray.getI(), ray.getJ());
  }

  // Get material
  BlinnMaterial* bm = (BlinnMaterial*)(isect.primitive->getMaterial().get());
  auto ka = bm->getKa();
  auto kd = bm->getKd();
  auto ks = bm->getKs();
  auto glossiness = bm->getGlossiness();

  vec3 lightIntensity(0, 0, 0);

  vec3 vecToLight;
  SurfaceInteraction s1;
  SurfaceInteraction s2;
  VisibilityTester vt((SurfaceInteraction()), (SurfaceInteraction()));

  for (auto& l : scene.lights) {
    if (l->isAmbient()) {
      lightIntensity += ka * l->getIntensity();
    } else {
      auto currLightIntensity = l->getIntensityForHit(isect, &vecToLight, &vt);

      auto n = unit_vector(isect.n);
      auto h = unit_vector(isect.wo + vecToLight);

      if (!vt.isOccluded(scene)) {
        lightIntensity +=
            kd * currLightIntensity *
            fmax(0.0, dot(unit_vector(n), unit_vector(vecToLight)));
        lightIntensity +=
            ks * currLightIntensity * pow(fmax(0.0, dot(n, h)), glossiness);
      }
    }
  }

  Pixel p(fmin(255.0, lightIntensity.r() * 255),
          fmin(255.0, lightIntensity.g() * 255),
          fmin(255.0, lightIntensity.b() * 255));

  return p;
}