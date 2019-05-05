#pragma once
#include "integrator.h"

class FlatIntegrator : public SampleIntegrator {
 public:
  Pixel Li(const Ray& ray, const Scene& scene, Sampler& sampler);
};
