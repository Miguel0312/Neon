#ifndef NEON_POINT_H
#define NEON_POINT_H

#include "vector.h"
#include <cassert>
#include <ostream>

namespace Neon {
template <typename T, unsigned int D> class Point {
public:
  Point(T x = 0) {
    for (unsigned int i = 0; i < D; i++) {
      m_data[i] = x;
    }
  }

  Point(T x, T y) {
    assert(D == 2);

    m_data[0] = x;
    m_data[1] = y;
  }

  Point(T x, T y, T z) {
    assert(D == 3);

    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
  }

  Point(T x, T y, T z, T w) {
    assert(D == 4);

    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
    m_data[3] = w;
  }

  operator Vector<T, D>() {
    switch (D) {
    case 2:
      return Vector<T, D>(m_data[0], m_data[1]);
    case 3:
      return Vector<T, D>(m_data[0], m_data[1], m_data[2]);
    case 4:
      return Vector<T, D>(m_data[0], m_data[1], m_data[2], m_data[3]);
    default:
      return Vector<T, D>(m_data[0]);
    }
  }

  T &operator[](int i) { return m_data[i]; }
  const T operator[](int i) const { return m_data[i]; }

  Point<T, D> operator+(const Vector<T, D> &v) const {
    Point<T, D> p;
    for (unsigned int i = 0; i < D; i++) {
      p[i] = m_data[i] + v[i];
    }

    return p;
  }

  Point<T, D> operator+(const Point<T, D> &v) const {
    Point<T, D> p;
    for (unsigned int i = 0; i < D; i++) {
      p[i] = m_data[i] + v[i];
    }

    return p;
  }

  Point<T, D> &operator+=(const Point<T, D> &v) {
    Point<T, D> p;
    for (int i = 0; i < D; i++) {
      m_data[i] += v[i];
    }
    return *this;
  }

  Point<T, D> operator-() const {
    Point<T, D> v;
    for (unsigned int i = 0; i < D; i++) {
      v[i] = -m_data[i];
    }

    return v;
  }

  Point<T, D> operator-(const Vector<T, D> &v) const { return *this + (-v); }

  Point<T, D> operator-(const Point<T, D> &v) const { return *this + (-v); }

  Point<T, D> &operator-=(const Vector<T, D> &v) {
    for (int i = 0; i < D; i++) {
      m_data[i] -= v[i];
    }
    return *this;
  }

  template <typename U> Point<T, D> operator*(U t) const {
    Point<T, D> v;
    for (unsigned int i = 0; i < D; i++) {
      v[i] = t * m_data[i];
    }

    return v;
  }

  template <typename U> Point<T, D> &operator*=(U t) {
    for (int i = 0; i < D; i++) {
      m_data[i] *= t;
    }
    return *this;
  }

  template <typename U> Point<T, D> operator/(U t) const {
    assert(t != 0);
    return *this * (1 / t);
  }

  template <typename U> Point<T, D> &operator/=(U t) const {
    U inv = 1 / t;
    for (int i = 0; i < D; i++) {
      m_data[i] *= inv;
    }
    return *this;
  }

private:
  T m_data[D];
};

template <typename U, typename T, unsigned int D>
Point<T, D> operator*(U t, Point<T, D> v) {
  return v * t;
}

template <typename U, typename T, unsigned int D>
Point<T, D> operator/(U t, Point<T, D> v) {
  return v / t;
}

template <typename T, unsigned int D>
std::ostream &operator<<(std::ostream &out, const Point<T, D> &v) {
  out << '(' << v[0];
  for (unsigned int i = 1; i < D; i++) {
    out << ", " << v[i];
  }
  out << ')';
  return out;
}

typedef Point<int, 2> Point2i;
typedef Point<float, 2> Point2f;
typedef Point<double, 2> Point2d;
typedef Point<int, 3> Point3i;
typedef Point<float, 3> Point3f;
typedef Point<double, 3> Point3d;
} // namespace Neon

#endif
