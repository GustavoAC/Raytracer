#include "perspective_camera.h"

Ray PerspectiveCamera::generateRay(double i, double j) {
    double ii = i * width;
    double jj = j * width;

    double u = l + ((r - l) * (ii + 0.5)) / width;
    double v = b + ((t - b) * (jj + 0.5)) / height;

    vec3 direction = unit_vector(-focalDistance * wVec + u * uVec + v * vVec);
    return Ray(position, direction);
}