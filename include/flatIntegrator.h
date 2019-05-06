#pragma once
#include "integrator.h"

class FlatIntegrator : public SampleIntegrator {
 public:
  FlatIntegrator(std::shared_ptr<const Camera> cam,
                 std::shared_ptr<Sampler> sampler)
      : SampleIntegrator(cam, sampler) {}

  void preprocess(const Scene& scene) {}
  Pixel Li(const Ray& ray, const Scene& scene, Sampler& sampler) const;
};
