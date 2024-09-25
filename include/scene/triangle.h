#ifndef NEON_TRIANGLE_H
#define NEON_TRIANGLE_H

#include "math/point.h"
#include "reflection/bsdf.h"
#include "shape.h"

namespace Neon {
class Triangle final : public Shape {
public:
  Triangle(const Point3f &p1, const Point3f &p2, const Point3f &p3, BSDF *bsdf);

  Triangle(const toml::table *table);

  ~Triangle() = default;

  bool intersect(const Ray &r, const Intervalf &tInterval,
                 float *t = nullptr) const override;

  bool intersect(const Ray &r, const Intervalf &tInterval,
                 ShapeIntersectionRecord &rec) const override;

  void sample(Point3f &p, Vector3f &n, Sampler *sampler) const override;

  float area() const override;

  Vector3f normalAt(const Point3f &p) const override;

private:
  Point3f m_p1, m_p2, m_p3;
};
} // namespace Neon

#endif
