#ifndef NEON_SHAPE_H
#define NEON_SHAPE_H

#include "math/ray.h"
namespace Neon {
struct ShapeIntersectionRecord {
  // TODO: create a normal class
  Vector3f n;
  Point3f p;
  float t;

  ShapeIntersectionRecord() : n(0), p(0), t(0) {}
};

class Shape {
public:
  Shape() = default;
  virtual ~Shape() = default;

  // Returns true if and only if the ray colides with the shape with t in the
  // interval [minT, maxT]
  // The optional parameter t allows the caller to retrieve the t value of the
  // intersection, which is often already calculated, needed and comes from an
  // expensive computation
  virtual bool intersect(const Ray &r, const float minT, const float maxT,
                         float *t = nullptr) const = 0;

  // Returns true if and only if the ray colides with the shape with t in the
  // interval [minT, maxT] and fills rec with the necessary information
  virtual bool intersect(const Ray &r, const float minT, const float maxT,
                         ShapeIntersectionRecord &rec) const = 0;
};
} // namespace Neon

#endif
