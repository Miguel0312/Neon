#include "math/transform.h"
#include <cmath>
#include <iostream>
#include <scene/sphere.h>

namespace Neon {
bool Sphere::intersect(const Ray &r, const Intervalf &tInterval,
                       float *t) const {
  Vector3f v = r.origin - m_center;
  // It only works because r.dir is normalized
  const float a = 1;
  const float b = 2 * v.dot(r.dir);
  const float c = v.lengthSq() - m_radius * m_radius;

  float delta = b * b - 4 * a * c;
  if (delta < 0) {
    return false;
  }

  delta = std::sqrt(delta);
  float val = (-b - delta) / 2 * a;
  if (tInterval.contains(val)) {
    if (t) {
      *t = val;
    }
    return true;
  }

  val = (-b + delta) / 2 * a;
  if (tInterval.contains(val)) {
    if (t) {
      *t = val;
    }
    return true;
  }

  return false;
}

bool Sphere::intersect(const Ray &r, const Intervalf &tInterval,
                       ShapeIntersectionRecord &rec) const {
  bool hit = intersect(r, tInterval, &rec.t);
  if (!hit) {
    return false;
  }

  rec.p = r(rec.t);
  rec.n = (rec.p - m_center) / m_radius;
  rec.shape = this;
  // If t > -b / 2 * a, we need to flip the normal
  Vector3f v = r.origin - m_center;
  const float a = 1;
  const float b = 2 * v.dot(r.dir);
  if (rec.t > -b / 2 * a) {
    rec.n *= -1;
  }
  
  rec.frame = Frame(RotateFromTo(rec.n, Vector3f(0, 0, 1)));

  return true;
}
} // namespace Neon
