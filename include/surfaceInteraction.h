#pragma once

#include "geometricPrimitive.h"
#include "point2.h"
#include "vec3.h"

struct SurfaceInteraction {
  SurfaceInteraction(const point3 &p, const vec3 &n, const vec3 &wo, float t,
                     const Point2 &uv, const GeometricPrimitive *pri)
      : p{p}, n{n}, wo{wo}, t{t}, uv{uv}, primitive{pri} {};

  SurfaceInteraction(){};

  point3 p;  /** Contact point. */
  vec3 n;    /** The surface normal. */
  vec3 wo;   /** Outgoing direction of light, which is -ray. */
  float t;   /** Time of contact. */
  Point2 uv; /** Parametric coordinate (u,v) of the hit surface. */
  const GeometricPrimitive *primitive =
      nullptr; /** Pointer to the primitive. */
};