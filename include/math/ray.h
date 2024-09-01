#ifndef NEON_RAY_H
#define NEON_RAY_H

#include "math/point.h"
#include "math/vector.h"
namespace Neon {

/// Creates a ray from a point and direction. The dir vector should be
/// normalized
class Ray {
public:
  Point3f origin;
  Vector3f dir;

  Ray(const Point3f &o, const Vector3f &d) : origin(o), dir(d) {}

  Point3f operator()(float t) { return origin + t * dir; }
};
} // namespace Neon

#endif
