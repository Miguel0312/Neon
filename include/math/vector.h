#ifndef NEON_VECTOR_H
#define NEON_VECTOR_H

#include <cassert>
#include <cmath>
#include <ostream>

namespace Neon {
template <typename T, unsigned int D> class Vector {
public:
  Vector(T x = 0) {
    for (unsigned int i = 0; i < D; i++) {
      m_data[i] = x;
    }
  }

  Vector(T x, T y) {
    assert(D == 2);

    m_data[0] = x;
    m_data[1] = y;
  }

  Vector(T x, T y, T z) {
    assert(D == 3);

    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
  }

  Vector(T x, T y, T z, T w) {
    assert(D == 4);

    m_data[0] = x;
    m_data[1] = y;
    m_data[2] = z;
    m_data[3] = w;
  }

  inline T &operator[](int i) { return m_data[i]; }
  inline const T operator[](int i) const { return m_data[i]; }

  Vector<T, D> operator+(const Vector<T, D> &other) const {
    Vector<T, D> v;
    for (unsigned int i = 0; i < D; i++) {
      v[i] = m_data[i] + other[i];
    }

    return v;
  }

  Vector<T, D> &operator+=(const Vector<T, D> &other) {
    for (unsigned int i = 0; i < D; i++) {
      m_data[i] += other[i];
    }
    return *this;
  }

  Vector<T, D> operator-() const {
    Vector<T, D> v;
    for (unsigned int i = 0; i < D; i++) {
      v[i] = -m_data[i];
    }

    return v;
  }

  Vector<T, D> operator-(const Vector<T, D> &other) const {
    return *this + (-other);
  }

  Vector<T, D> &operator-=(const Vector<T, D> &other) {
    for (int i = 0; i < D; i++) {
      m_data[i] -= other.m_data[i];
    }
    return *this;
  }

  template <typename U> Vector<T, D> operator*(U t) const {
    Vector<T, D> v;
    for (unsigned int i = 0; i < D; i++) {
      v[i] = t * m_data[i];
    }

    return v;
  }

  template <typename U> Vector<T, D> &operator*=(U t) {
    for (unsigned int i = 0; i < D; i++) {
      m_data[i] *= t;
    }
    return *this;
  }

  template <typename U> Vector<T, D> operator/(U t) const {
    assert(t != 0);
    return *this * (1 / t);
  }

  template <typename U> Vector<T, D> &operator/=(U t) const {
    U inv = 1 / t;
    for (unsigned int i = 0; i < D; i++) {
      m_data[i] *= inv;
    }
    return *this;
  }

  T dot(const Vector<T, D> &other) const {
    T result = 0;
    for (unsigned int i = 0; i < D; i++) {
      result += m_data[i] * other[i];
    }

    return result;
  }

  float lengthSq() const { return this->dot(*this); }

  float length() const { return std::sqrt(lengthSq()); }

  Vector<T, D> normalized() const { return *this / length(); }

  Vector<T, D> &normalize() {
    float norm = length();
    for (unsigned int i = 0; i < D; i++) {
      (*this)[i] /= norm;
    }
    return *this;
  }

  Vector<T, D> cross(const Vector<T, D> &other) const {
    assert(D == 3);
    Vector<T, D> result;

    result[0] = m_data[1] * other[2] - m_data[2] * other[1];
    result[1] = m_data[2] * other[0] - m_data[0] * other[2];
    result[2] = m_data[0] * other[1] - m_data[1] * other[0];

    return result;
  }

private:
  T m_data[D];
};

template <typename U, typename T, unsigned int D>
Vector<T, D> operator*(U t, const Vector<T, D> &v) {
  return v * t;
}

template <typename U, typename T, unsigned int D>
Vector<T, D> operator/(U t, const Vector<T, D> &v) {
  return v / t;
}

template <typename T, unsigned int D>
std::ostream &operator<<(std::ostream &out, const Vector<T, D> &v) {
  out << '(' << v[0];
  for (int i = 1; i < D; i++) {
    out << ", " << v[i];
  }
  out << ')';
  return out;
}

typedef Vector<int, 2> Vector2i;
typedef Vector<float, 2> Vector2f;
typedef Vector<double, 2> Vector2d;
typedef Vector<int, 3> Vector3i;
typedef Vector<float, 3> Vector3f;
typedef Vector<double, 3> Vector3d;
} // namespace Neon

#endif
