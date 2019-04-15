#pragma once

#include "primitive.h"
#include "vec3.h"
#include "surfaceInteraction.h"

class Sphere : public Primitive {
   private:
    float m_radius;
    point3 m_center;

   public:
    Sphere(float radius, const point3& center) : m_radius(radius), m_center(center) { /* Empty */
    }

    /**
     * returns true if ray intersects sphere and false otherwise.
     * Also stores hit record on Surface Interaction object passed as parameter.
     *
     * Solving for intersection:
     * f (r(t)) = (r(t) − c) . (r(t) − c) − r^2 = 0
     * (dt + o - c) . (dt + o - c) - r^2 = 0
     * (t*t) (d . d) + 2t (d . (o - c)) + ((o - c) . (o - c)) = 0
     * Solving for t:
     * a = d . d
     * b = d . (o - c)
     * c = (o - c) . (o - c)
     * */
    bool intersect(const Ray& ray, SurfaceInteraction* si) const {
        auto origin = ray.getOrigin();
        auto direction = ray.getDirection();

        auto a = dot(direction, direction);
        auto b = 2.0 * dot((origin - m_center), direction);
        auto c = dot(origin - m_center, origin - m_center) - (m_radius * m_radius);

        auto delta = b * b - 4.0 * a * c;

        if (delta < 0.0) return false;

        float t1 = (-b + std::sqrt(delta)) / (2 * a);
        float t2 = (-b - std::sqrt(delta)) / (2 * a);

        si->p = ray(std::min(t1, t2));

        return true;
    }

    /**
     * returns true if ray intersects sphere and false otherwise.
     *
     * Solving for intersection:
     * f (r(t)) = (r(t) − c) . (r(t) − c) − r^2 = 0
     * (dt + o - c) . (dt + o - c) - r^2 = 0
     * (t*t) (d . d) + 2t (d . (o - c)) + ((o - c) . (o - c)) = 0
     * Solving for t:
     * a = d . d
     * b = d . (o - c)
     * c = (o - c) . (o - c)
     * */

    bool intersect(const Ray& ray) const {
        auto origin = ray.getOrigin();
        auto direction = ray.getDirection();

        // solving with bhaskara:
        auto a = dot(direction, direction);
        auto b = 2.0 * dot((origin - m_center), direction);
        auto c = dot(origin - m_center, origin - m_center) - (m_radius * m_radius);

        auto delta = b * b - 4.0 * a * c;

        return (delta >= 0.0);
    }
};
