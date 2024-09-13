#include "math/matrix.h"
#include <math/transform.h>

namespace Neon {
Transform::Transform() : m_direct(), m_inverse() {}

Transform::Transform(const SquareMatrix<4> &mat)
    : Transform(mat, Inverse(mat)) {}

Transform::Transform(float mat[4][4]) : Transform(SquareMatrix<4>(mat)) {}

Transform::Transform(const SquareMatrix<4> &mat, const SquareMatrix<4> &inv)
    : m_direct(mat), m_inverse(inv) {}

Transform Transform::operator()(const Transform &trans) const {
  // (AB)^-1 = B^-1A^-1
  return Transform(m_direct * trans.m_direct, trans.m_inverse * m_inverse);
}

Vector3f Transform::operator()(const Vector3f &v) const { return m_direct * v; }

Point3f Transform::operator()(const Point3f &p) const { return m_direct * p; }

Vector3f Transform::applyInverse(const Vector3f &v) const {
  return m_inverse * v;
}

Point3f Transform::applyInverse(const Point3f &p) const {
  return m_inverse * p;
}

Transform Translate(const Vector3f &delta) {
  float dir[4][4] = {
      {1, 0, 0, delta.x()},
      {0, 1, 0, delta.y()},
      {0, 0, 1, delta.x()},
      {0, 0, 0, 1},
  };

  float inv[4][4] = {
      {1, 0, 0, -delta.x()},
      {0, 1, 0, -delta.y()},
      {0, 0, 1, -delta.x()},
      {0, 0, 0, 1},
  };

  SquareMatrix<4> dirMat(dir), invMat(inv);

  return Transform(dirMat, invMat);
}

Transform Scale(const Vector3f &scale) {
  float dir[4][4] = {
      {scale.x(), 0, 0, 0},
      {0, scale.y(), 0, 0},
      {0, 0, scale.z(), 0},
      {0, 0, 0, 1},
  };

  float inv[4][4] = {
      {1 / scale.x(), 0, 0, 0},
      {0, 1 / scale.y(), 0, 0},
      {0, 0, 1 / scale.z(), 0},
      {0, 0, 0, 1},
  };

  SquareMatrix<4> dirMat(dir), invMat(inv);

  return Transform(dirMat, invMat);
}

Transform RotateX(float theta) {
  float cosTheta = std::cos(theta), sinTheta = std::sin(theta);

  float dir[4][4] = {
      {1, 0, 0, 0},
      {0, cosTheta, -sinTheta, 0},
      {0, sinTheta, cosTheta, 0},
      {0, 0, 0, 1},
  };

  SquareMatrix<4> dirMat(dir);

  // Since it is a rotation matrix, its transpose is equal to its inverse
  return Transform(dirMat, Transpose(dirMat));
}

Transform RotateY(float theta) {
  float cosTheta = std::cos(theta), sinTheta = std::sin(theta);

  float dir[4][4] = {
      {cosTheta, 0, sinTheta, 0},
      {0, 1, 0, 0},
      {-sinTheta, 0, cosTheta, 0},
      {0, 0, 0, 1},
  };

  SquareMatrix<4> dirMat(dir);

  // Since it is a rotation matrix, its transpose is equal to its inverse
  return Transform(dirMat, Transpose(dirMat));
}

Transform RotateZ(float theta) {
  float cosTheta = std::cos(theta), sinTheta = std::sin(theta);

  float dir[4][4] = {
      {cosTheta, -sinTheta, 0, 0},
      {sinTheta, cosTheta, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  };

  SquareMatrix<4> dirMat(dir);

  // Since it is a rotation matrix, its transpose is equal to its inverse
  return Transform(dirMat, Transpose(dirMat));
}

Transform Rotate(const Vector3f &axis, float theta) {
  return Rotate(axis, std::cos(theta), std::sin(theta));
}

Transform Rotate(const Vector3f &axis, float cosTheta, float sinTheta) {
  Vector3f a = axis.normalized();

  float dir[4][4];

  dir[0][0] = a.x() * a.x() + (1 - a.x() * a.x()) * cosTheta;
  dir[0][1] = a.x() * a.y() * (1 - cosTheta) - a.z() * sinTheta;
  dir[0][2] = a.x() * a.z() * (1 - cosTheta) + a.y() * sinTheta;
  dir[0][3] = 0;

  dir[1][0] = a.x() * a.y() * (1 - cosTheta) + a.z() * sinTheta;
  dir[1][1] = a.y() * a.y() + (1 - a.y() * a.y()) * cosTheta;
  dir[1][2] = a.y() * a.z() * (1 - cosTheta) - a.x() * sinTheta;
  dir[1][3] = 0;

  dir[2][0] = a.x() * a.z() * (1 - cosTheta) - a.y() * sinTheta;
  dir[2][1] = a.y() * a.z() * (1 - cosTheta) + a.x() * sinTheta;
  dir[2][2] = a.z() * a.z() + (1 - a.z() * a.z()) * cosTheta;
  dir[2][3] = 0;

  dir[3][0] = 0;
  dir[3][1] = 0;
  dir[3][2] = 0;
  dir[3][3] = 1;

  SquareMatrix<4> dirMat(dir);

  // Since it is a rotation matrix, its transpose is equal to its inverse
  return Transform(dirMat, Transpose(dirMat));
}

Transform RotateFromTo(Vector3f &from, const Vector3f &to) {
  // Implemetation taken from the PBRT book - 4th edition
  // Uses the Householder matrix
  Vector3f refl;
  if (std::abs(from.x()) < 0.72f && std::abs(to.x()) < 0.72f)
    refl = Vector3f(1, 0, 0);
  else if (std::abs(from.y()) < 0.72f && std::abs(to.y()) < 0.72f)
    refl = Vector3f(0, 1, 0);
  else
    refl = Vector3f(0, 0, 1);

  Vector3f u = refl - from, v = refl - to;
  SquareMatrix<4> r;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) {
      float val = ((i == j) ? 1 : 0) - 2 / u.dot(u) * u[i] * u[j] -
                  2 / v.dot(v) * v[i] * v[j] +
                  4 * u.dot(v) / (u.dot(u) * v.dot(v)) * v[i] * u[j];
      r.set(val, i, j);
    }

  return Transform(r, Transpose(r));
}
} // namespace Neon
