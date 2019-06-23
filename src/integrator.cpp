#include "integrator.h"

void SampleIntegrator::render(const Scene& scene) {
  // Always call the preprocess() before doing any rendering.
  // This might be just an empty method, or not, depending on the integrator's
  // needs.
  preprocess(scene);

  auto w = camera->getWidth();
  auto h = camera->getHeight();

  Sampler sampler;
  auto film = camera->getFilm();

  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      std::cout << "i " << i << " j " << j << "\n";
      Ray r1 = camera->generateRay(float(i) / float(w), float(j) / float(h));
      Pixel p = Li(r1, scene, sampler);
      film->setPixel(i, j, p);
    }
  }
}