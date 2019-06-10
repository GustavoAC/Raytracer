#pragma once
#include "integrator.h"

class BlinnPhongIntegrator : public SampleIntegrator {
 public:
  BlinnPhongIntegrator(std::shared_ptr<const Camera> cam,
                       std::shared_ptr<Sampler> sampler)
      : SampleIntegrator(cam, sampler) {}

  void preprocess(const Scene& scene) {}
  Pixel Li(const Ray& ray, const Scene& scene, Sampler& sampler) const;
};