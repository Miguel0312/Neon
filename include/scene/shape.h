#ifndef NEON_SHAPE_H
#define NEON_SHAPE_H

#include "math/boundingBox.h"
#include "math/interval.h"
#include "math/ray.h"
#include "reflection/bsdf.h"
#include <memory>
namespace Neon {
class Shape;

struct ShapeIntersectionRecord {
  // TODO: create a normal class
  Vector3f n;
  Point3f p;
  float t;
  const Shape *shape;
  Frame frame;

  ShapeIntersectionRecord() : n(0), p(0), t(0) {}
};

class Shape {
public:
  Shape() = delete;

  Shape(BSDF *bsdf) : m_bsdf(bsdf) {}

  virtual ~Shape() = default;

  const BSDF *getBSDF() const { return m_bsdf; }

  // Returns true if and only if the ray colides with the shape with t in the
  // interval [minT, maxT]
  // The optional parameter t allows the caller to retrieve the t value of the
  // intersection, which is often already calculated, needed and comes from an
  // expensive computation
  virtual bool intersect(const Ray &r, const Intervalf &tInterval,
                         float *t = nullptr) const = 0;

  // Returns true if and only if the ray colides with the shape with t in the
  // interval [minT, maxT] and fills rec with the necessary information
  virtual bool intersect(const Ray &r, const Intervalf &tInterval,
                         ShapeIntersectionRecord &rec) const = 0;

  const BoundingBox &getBoundingBox() const { return m_box; }

protected:
  // TODO: maybe create a mesh class with this info
  BSDF *m_bsdf;

  BoundingBox m_box;
};
} // namespace Neon

#endif
