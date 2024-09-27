#include "math/transform.h"
#include "scene/sceneParser.h"
#include "utils/objectFactory.h"
#include "utils/sampling/warp.h"
#include "utils/utils.h"
#include <cmath>
#include <scene/sphere.h>

namespace Neon {
Sphere::Sphere(const toml::table *table) {
  m_radius = table->at("radius").as_floating_point()->get();
  m_center = parsePoint(table->at("center").as_array());
  m_bsdf = SceneParser::getSingleton()->getMaterial(
      table->at("material").as_string()->get());
  m_box =
      BoundingBox(m_center - Vector3f(m_radius), m_center + Point3f(m_radius));
}

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
  float val = (-b - delta) / (2 * a);
  if (tInterval.contains(val)) {
    if (t) {
      *t = val;
    }
    return true;
  }

  val = (-b + delta) / (2 * a);
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
  if (rec.t > -b / (2 * a)) {
    rec.n *= -1;
  }

  rec.frame = Frame(RotateFromTo(rec.n, Vector3f(0, 0, 1)));

  return true;
}

void Sphere::sample(Point3f &p, Vector3f &n, Sampler *sampler) const {
  Vector3f unitVector = Warp::squareToUniformSphere(sampler->next2D());

  p = m_center + m_radius * unitVector;
  n = unitVector;
}

float Sphere::area() const { return 4 * M_PI * m_radius * m_radius; }

Vector3f Sphere::normalAt(const Point3f &p) const {
  return (p - m_center) / m_radius;
}

NEON_REGISTER_CLASS(Sphere, "sphere");
} // namespace Neon
