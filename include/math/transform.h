#ifndef NEON_TRANSFORM_H
#define NEON_TRANSFORM_H

#include "math/matrix.h"

namespace Neon {
class Transform final {
public:
  Transform();

  Transform(const SquareMatrix<4> &mat);

  Transform(float mat[4][4]);

  // Allows the inverse matrix to be passed directly to avoid the expensive
  // computation of computing the inverse
  Transform(const SquareMatrix<4> &mat, const SquareMatrix<4> &inv);

  ~Transform() = default;

  Transform operator()(const Transform &trans) const;

  Vector3f operator()(const Vector3f &v) const;

  Point3f operator()(const Point3f &p) const;

  Vector3f applyInverse(const Vector3f &v) const;

  Point3f applyInverse(const Point3f &p) const;

private:
  SquareMatrix<4> m_direct, m_inverse;
};

Transform Translate(const Vector3f &delta);

Transform Scale(const Vector3f &scale);

Transform RotateX(float theta);

Transform RotateY(float theta);

Transform RotateZ(float theta);

Transform Rotate(const Vector3f &axis, float theta);

Transform Rotate(const Vector3f &axis, float cosTheta, float sinTheta);

// from and to must be normalized
Transform RotateFromTo(const Vector3f &from, const Vector3f &to);

// Utility class to transform from one frame of reference to another
class Frame final {
public:
  Frame() = default;

  // The direct transfortation of transform should be from world to local space
  Frame(const Transform &transform) : m_transform(transform) {}

  inline Vector3f worldToLocal(const Vector3f &v) const {
    return m_transform(v);
  }

  inline Point3f worldToLocal(const Point3f &p) const {
    return m_transform(p);
  };

  inline Vector3f localToWorld(const Vector3f &v) const {
    return m_transform.applyInverse(v);
  }

  inline Point3f localToWorld(const Point3f &p) const {
    return m_transform.applyInverse(p);
  };

private:
  Transform m_transform;
};
} // namespace Neon

#endif
