#pragma once
#include <memory>
#include "camera.h"
#include "sampler.h"
#include "scene.h"

class Integrator {
 public:
  virtual void render(const Scene& scene) = 0;
};

class SampleIntegrator : public Integrator {
 public:
  SampleIntegrator(std::shared_ptr<const Camera> cam,
                   std::shared_ptr<Sampler> sampler)
      : camera{cam}, sampler{sampler} {};

  virtual Pixel Li(const Ray& ray, const Scene& scene,
                   Sampler& sampler) const = 0;
  virtual void preprocess(const Scene& scene) = 0;
  virtual void render(const Scene& scene);

 protected:
  std::shared_ptr<const Camera> camera;

 private:
  std::shared_ptr<Sampler> sampler;
};