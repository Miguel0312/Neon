#include "scene/triangle.h"
#include "scene/sceneParser.h"
#include "utils/consts.h"
#include "utils/objectFactory.h"
#include "utils/utils.h"
#include <iostream>

namespace Neon {
Triangle::Triangle(const Point3f &p1, const Point3f &p2, const Point3f &p3,
                   BSDF *bsdf)
    : Shape(bsdf), m_p1(p1), m_p2(p2), m_p3(p3) {
  Point3f mini(std::min(p1.x(), std::min(p2.x(), p3.x())),
               std::min(p1.y(), std::min(p2.y(), p3.y())),
               std::min(p1.z(), std::min(p2.z(), p3.z())));

  Point3f maxi(std::max(p1.x(), std::max(p2.x(), p3.x())),
               std::max(p1.y(), std::max(p2.y(), p3.y())),
               std::max(p1.z(), std::max(p2.z(), p3.z())));

  for (int i = 0; i < 3; i++) {
    if (maxi[i] - mini[i] < EPSILON) {
      mini[i] -= 1e-3;
    }
  }

  m_box = BoundingBox(mini, maxi);
}

Triangle::Triangle(const toml::table *table) {
  m_p1 = parsePoint(table->at("p1").as_array());
  m_p2 = parsePoint(table->at("p2").as_array());
  m_p3 = parsePoint(table->at("p3").as_array());

  m_bsdf = SceneParser::getSingleton()->getMaterial(
      table->at("material").as_string()->get());

  Point3f mini(std::min(m_p1.x(), std::min(m_p2.x(), m_p3.x())),
               std::min(m_p1.y(), std::min(m_p2.y(), m_p3.y())),
               std::min(m_p1.z(), std::min(m_p2.z(), m_p3.z())));

  Point3f maxi(std::max(m_p1.x(), std::max(m_p2.x(), m_p3.x())),
               std::max(m_p1.y(), std::max(m_p2.y(), m_p3.y())),
               std::max(m_p1.z(), std::max(m_p2.z(), m_p3.z())));

  for (int i = 0; i < 3; i++) {
    if (maxi[i] - mini[i] < EPSILON) {
      mini[i] -= 1e-3;
    }
  }

  m_box = BoundingBox(mini, maxi);
}

bool Triangle::intersect(const Ray &r, const Intervalf &tInterval,
                         float *t) const {
  Vector3f v1 = m_p2 - m_p1, v2 = m_p3 - m_p1, u = v1.cross(v2);
  float tCandidate = -(r.origin - m_p1).dot(u) / r.dir.dot(u);

  if (!tInterval.contains(tCandidate)) {
    return false;
  }

  Vector3f p = r(tCandidate) - m_p1;
  Vector3f vPrime = u.cross(v1);

  float aPrime = p.dot(v1) / v1.lengthSq(),
        bPrime = p.dot(vPrime) / vPrime.lengthSq();
  float alpha = v2.dot(v1) / v1.lengthSq(),
        beta = v2.dot(vPrime) / vPrime.lengthSq();

  float a = aPrime - bPrime * alpha / beta, b = bPrime / beta;

  if (a >= 0 && b >= 0 && a + b <= 1) {
    if (t) {
      *t = tCandidate;
    }
    return true;
  }

  return false;
}

bool Triangle::intersect(const Ray &r, const Intervalf &tInterval,
                         ShapeIntersectionRecord &rec) const {
  bool hit = intersect(r, tInterval, &rec.t);
  if (!hit) {
    return false;
  }

  Vector3f v1 = m_p2 - m_p1, v2 = m_p3 - m_p1;

  rec.p = r(rec.t);
  rec.n = v1.cross(v2).normalized();

  if (rec.n.dot(r.dir) > 0) {
    rec.n *= -1;
  }

  rec.shape = this;

  rec.frame = Frame(RotateFromTo(rec.n, Vector3f(0, 0, 1)));

  return true;
}

void Triangle::sample(Point3f &p, Vector3f &n, Sampler *sampler) const {
  Point2f sample = sampler->next2D();
  Vector3f v1 = m_p2 - m_p1, v2 = m_p3 - m_p1;

  p = m_p1 + sample.x() * v1 + sample.y() * (1 - sample.x()) * v2;
  n = v1.cross(v2).normalized();
}

float Triangle::area() const {
  Vector3f v1 = m_p2 - m_p1, v2 = m_p3 - m_p1;
  return 0.5f * v1.cross(v2).length();
}

Vector3f Triangle::normalAt(const Point3f &p) const {
  Vector3f v1 = m_p2 - m_p1, v2 = m_p3 - m_p1;
  return v1.cross(v2).normalized();
}

NEON_REGISTER_CLASS(Triangle, "triangle");
} // namespace Neon
