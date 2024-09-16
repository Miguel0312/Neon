#include "math/interval.h"
#include "utils/consts.h"
#include <math/boundingBox.h>

namespace Neon {
BoundingBox::BoundingBox(const Point3f p1, const Point3f p2) {
  m_min = Point3f(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
                  std::min(p1.z(), p2.z()));
  m_max = Point3f(std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()),
                  std::max(p1.z(), p2.z()));
}

BoundingBox BoundingBox::merge(const BoundingBox &box) const {
  Point3f mini = Point3f(std::min(m_min.x(), box.m_min.x()),
                         std::min(m_min.y(), box.m_min.y()),
                         std::min(m_min.z(), box.m_min.z()));
  Point3f maxi = Point3f(std::max(m_max.x(), box.m_max.x()),
                         std::max(m_max.y(), box.m_max.y()),
                         std::max(m_max.z(), box.m_max.z()));

  return BoundingBox(mini, maxi);
}

BoundingBox BoundingBox::intersection(const BoundingBox &box) const {
  Point3f mini = Point3f(std::max(m_min.x(), box.m_min.x()),
                         std::max(m_min.y(), box.m_min.y()),
                         std::max(m_min.z(), box.m_min.z()));
  Point3f maxi = Point3f(std::min(m_max.x(), box.m_max.x()),
                         std::min(m_max.y(), box.m_max.y()),
                         std::min(m_max.z(), box.m_max.z()));

  return BoundingBox(mini, maxi);
}

bool BoundingBox::contains(const Point3f &p, bool strict) const {
  if (strict) {
    return p.x() > m_min.x() && p.x() < m_max.x() && p.y() > m_min.y() &&
           p.y() < m_max.y() && p.z() > m_min.z() && p.z() < m_max.z();
  }
  return p.x() >= m_min.x() && p.x() <= m_max.x() && p.y() >= m_min.y() &&
         p.y() <= m_max.y() && p.z() >= m_min.z() && p.z() <= m_max.z();
}

bool BoundingBox::intersects(const BoundingBox &box) const {
  return box.m_min.x() < m_max.x() && box.m_max.x() > m_min.x() &&
         box.m_min.y() < m_max.y() && box.m_max.y() > m_min.y() &&
         box.m_min.z() < m_max.z() && box.m_max.z() > m_min.z();
}

bool BoundingBox::intersects(const Ray &ray, const Intervalf &tInterval) const {
  Intervalf xInterval, yInterval, zInterval;
  Intervalf curInterval = tInterval;

  if (std::abs(ray.dir.x()) < EPSILON) {
    xInterval = ray.origin.x() >= m_min.x() && ray.origin.x() <= m_max.x()
                    ? tInterval
                    : Intervalf();
  } else {
    float mini = (m_min.x() - ray.origin.x()) / ray.dir.x();
    float maxi = (m_max.x() - ray.origin.x()) / ray.dir.x();
    if (ray.dir.x() < 0) {
      std::swap(mini, maxi);
    }
    xInterval = Intervalf(mini, maxi);
  }

  curInterval = curInterval.intersection(xInterval);
  if (curInterval.empty()) {
    return false;
  }

  if (std::abs(ray.dir.y()) < EPSILON) {
    yInterval = ray.origin.y() >= m_min.y() && ray.origin.y() <= m_max.y()
                    ? tInterval
                    : Intervalf();
  } else {
    float mini = (m_min.y() - ray.origin.y()) / ray.dir.y();
    float maxi = (m_max.y() - ray.origin.y()) / ray.dir.y();
    if (ray.dir.y() < 0) {
      std::swap(mini, maxi);
    }
    yInterval = Intervalf(mini, maxi);
  }

  curInterval = curInterval.intersection(yInterval);
  if (curInterval.empty()) {
    return false;
  }

  if (std::abs(ray.dir.z()) < EPSILON) {
    zInterval = ray.origin.z() >= m_min.z() && ray.origin.z() <= m_max.z()
                    ? tInterval
                    : Intervalf();
  } else {
    float mini = (m_min.z() - ray.origin.z()) / ray.dir.z();
    float maxi = (m_max.z() - ray.origin.z()) / ray.dir.z();
    if (ray.dir.z() < 0) {
      std::swap(mini, maxi);
    }
    zInterval = Intervalf(mini, maxi);
  }

  curInterval = curInterval.intersection(zInterval);
  return !curInterval.empty();
}

Point3f BoundingBox::getCenter() const { return (m_min + m_max) / 2.f; }

bool BoundingBox::isValid() const {
  return m_min.x() < m_max.x() && m_min.y() < m_max.y() &&
         m_min.z() < m_max.z();
}
} // namespace Neon
