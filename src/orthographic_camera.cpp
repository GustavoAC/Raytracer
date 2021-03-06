#include "orthographic_camera.h"
#include "vec3.h"

Ray OrthographicCamera::generateRay(double i, double j) const {
  double ii = i * width;
  double jj = j * height;

  double u = l + ((r - l) * (ii + 0.5)) / width;
  double v = b + ((t - b) * (jj + 0.5)) / height;

  point3 origin = position + u * uVec + v * vVec;
  vec3 direction = -wVec;
  return Ray(origin, direction, i, j);
}