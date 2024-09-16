#ifndef NEON_BOUNDING_BOX_H
#define NEON_BOUNDING_BOX_H

#include "interval.h"
#include "math/point.h"
#include "math/ray.h"
#include "vector.h"
#include <limits>

namespace Neon {
class BoundingBox {
public:
  // Creates an empty bounding box
  BoundingBox()
      : m_min(std::numeric_limits<float>::max()),
        m_max(std::numeric_limits<float>::min()) {}

  // Creates a box from two points that form a diagonal of the box
  BoundingBox(const Point3f p1, const Point3f p2);

  // Returns a bounding box that contains all points belonging to any of the two
  // bounding boxes
  BoundingBox merge(const BoundingBox &box) const;

  // Returns a bounding box that contains all points that belong to both
  // bounding boxes
  BoundingBox intersection(const BoundingBox &box) const;

  // Returns wheter the point is inside the bounding box. If strict is set to
  // true, admits that if the point is on the border of the box, it is
  // considered outside the box
  bool contains(const Point3f &p, bool strict = false) const;

  // Returns true if the two boxes have at least one point in common
  bool intersects(const BoundingBox &box) const;

  // Returns true if the ray intersects the box in at least one point.
  bool intersects(const Ray &ray, const Intervalf &tInterval) const;

  Point3f getMin() const { return m_min; }

  Point3f getMax() const { return m_max; }

  Point3f getCenter() const;

  // Returns true if the box contains at least one point strictly inside it
  bool isValid() const;

private:
  Point3f m_min;
  Point3f m_max;
};
} // namespace Neon

#endif
