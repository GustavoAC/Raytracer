#pragma once
#include "integrator.h"
#include "pixel.h"

class DepthMapIntegrator : public SampleIntegrator {
 private:
  double smallestT = 10000000000;
  double biggestT = -10000000000;

  Pixel colorClose;
  Pixel colorFar;

 public:
  DepthMapIntegrator(std::shared_ptr<const Camera> cam,
                     std::shared_ptr<Sampler> sampler, const Pixel& _colorClose,
                     const Pixel& _colorFar)
      : SampleIntegrator(cam, sampler),
        colorClose(_colorClose),
        colorFar(_colorFar) {}

  void preprocess(const Scene& scene);
  Pixel Li(const Ray& ray, const Scene& scene, Sampler& sampler) const;
};
