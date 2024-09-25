#ifndef NEON_SPHERE_H
#define NEON_SPHERE_H

#include "math/point.h"
#include "reflection/bsdf.h"
#include "shape.h"

namespace Neon {
class Sphere final : public Shape {
public:
  Sphere(BSDF *bsdf) : Shape(bsdf), m_center(0), m_radius(1) {
    m_box = BoundingBox(m_center - Vector3f(m_radius),
                        m_center + Point3f(m_radius));
  }

  Sphere(const Point3f &center, float radius, BSDF *bsdf)
      : Shape(bsdf), m_center(center), m_radius(radius) {
    m_box = BoundingBox(m_center - Vector3f(m_radius),
                        m_center + Point3f(m_radius));
  }

  Sphere(const toml::table *table);

  ~Sphere() = default;

  bool intersect(const Ray &r, const Intervalf &tInterval,
                 float *t = nullptr) const override;

  bool intersect(const Ray &r, const Intervalf &tInterval,
                 ShapeIntersectionRecord &rec) const override;

  void sample(Point3f &p, Vector3f &n, Sampler *sampler) const override;

  float area() const override;

  Vector3f normalAt(const Point3f &p) const override;

private:
  Point3f m_center;
  float m_radius;
};
} // namespace Neon

#endif
