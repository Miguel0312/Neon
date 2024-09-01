#include <cmath>
#include <iostream>
#include <scene/sphere.h>

namespace Neon {
bool Sphere::intersect(const Ray &r, const float minT, const float maxT,
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
  if (val >= minT && val <= maxT) {
    if (t) {
      *t = val;
    }
    return true;
  }

  val = (-b + delta) / 2 * a;
  std::cout << val << std::endl;
  if (val >= minT && val <= maxT) {
    if (t) {
      *t = val;
    }
    return true;
  }

  return false;
}

bool Sphere::intersect(const Ray &r, const float minT, const float maxT,
                       ShapeIntersectionRecord &rec) const {
  bool hit = intersect(r, minT, maxT, &rec.t);
  if (!hit) {
    return false;
  }

  rec.p = r(rec.t);
  rec.n = (rec.p - m_center) / m_radius;

  return true;
}
} // namespace Neon
