#pragma once
#include "integrator.h"
#include "pixel.h"

class NormalMapIntegrator : public SampleIntegrator {
 public:
  NormalMapIntegrator(std::shared_ptr<const Camera> cam,
                      std::shared_ptr<Sampler> sampler)
      : SampleIntegrator(cam, sampler) {}

  void preprocess(const Scene& scene) {}
  Pixel Li(const Ray& ray, const Scene& scene, Sampler& sampler) const;
};
