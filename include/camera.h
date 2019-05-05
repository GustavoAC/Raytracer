#pragma once

#include <memory>
#include "buffer.h"
#include "ray.h"
#include "vec3.h"

class Camera {
 protected:
  int width;
  int height;

  point3 position;
  point3 target;
  vec3 vUp;

  // camera borders
  double l, r, b, t;

  // camera vectors
  vec3 wVec, uVec, vVec;

  std::shared_ptr<Buffer> film;

 public:
  Camera(int _width, int _height, const point3 &_position,
         const point3 &_target, const vec3 &_vUp)
      : width(_width),
        height(_height),
        position(_position),
        target(_target),
        vUp(_vUp) {
    calcCameraVectors();
    film = std::make_shared<Buffer>(Buffer(_width, _height));
  }

  void calcCameraVectors() {
    vec3 gaze = target - position;

    wVec = unit_vector(-gaze);
    uVec = unit_vector(cross(vUp, wVec));
    vVec = unit_vector(cross(uVec, wVec));
  }

  /**
   * Generates ray for i, j in [0, 1]
   */
  virtual Ray generateRay(double i, double j) const = 0;
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  std::shared_ptr<Buffer> getFilm() const { return film; }
};