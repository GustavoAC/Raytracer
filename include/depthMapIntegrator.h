#pragma once
#include "integrator.h"
#include "pixel.h"

class DepthMapIntegrator : public SampleIntegrator {
 private:
  double smallestT = 10000000000;
  double biggestT = -10000000000;

  Pixel colorClose = Pixel(150, 150, 150);
  Pixel colorFar = Pixel(255, 255, 255);

 public:
  DepthMapIntegrator(std::shared_ptr<const Camera> cam,
                     std::shared_ptr<Sampler> sampler)
      : SampleIntegrator(cam, sampler) {}

  void preprocess(const Scene& scene) {}
  Pixel Li(const Ray& ray, const Scene& scene, Sampler& sampler) const;
};
