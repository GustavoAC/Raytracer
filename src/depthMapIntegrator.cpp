#include "depthMapIntegrator.h"
#include "ray.h"
#include "surfaceInteraction.h"

void DepthMapIntegrator::preprocess(const Scene& scene) {
  auto w = camera->getWidth();
  auto h = camera->getHeight();

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      SurfaceInteraction si;
      Ray r1 = camera->generateRay(float(i) / float(w), float(j) / float(h));
      if (scene.intersect(r1, &si)) {
        if (si.t > biggestT) biggestT = si.t;
        if (si.t < smallestT) smallestT = si.t;
      }
    }
  }
}

Pixel DepthMapIntegrator::Li(const Ray& ray, const Scene& scene,
                             Sampler& sampler) const {
  SurfaceInteraction isect;
  if (!scene.intersect(ray, &isect)) {
    return colorFar;
  }

  auto colorMultiplier = (isect.t - smallestT) / (biggestT - smallestT);
  auto finalR = colorClose.getR() +
                (colorFar.getR() - colorClose.getR()) * colorMultiplier;
  auto finalG = colorClose.getG() +
                (colorFar.getG() - colorClose.getG()) * colorMultiplier;
  auto finalB = colorClose.getB() +
                (colorFar.getB() - colorClose.getB()) * colorMultiplier;

  return Pixel(finalR, finalG, finalB);
}