#ifndef NEON_SPHERE_H
#define NEON_SPHERE_H

#include "shape.h"

namespace Neon {
class Sphere final : Shape {
public:
  Sphere() : m_center(0), m_radius(1){};
  Sphere(const Point3f &center, float radius)
      : m_center(center), m_radius(radius) {}

  virtual ~Sphere() = default;

  bool intersect(const Ray &r, const float minT, const float maxT,
                 float *t = nullptr) const override;

  bool intersect(const Ray &r, const float minT, const float maxT,
                 ShapeIntersectionRecord &rec) const override;

private:
  Point3f m_center;
  float m_radius;
};
} // namespace Neon

#endif
