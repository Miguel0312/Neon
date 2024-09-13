#ifndef NEON_MATRIX_H
#define NEON_MATRIX_H

#include "math/point.h"
#include "math/vector.h"
#include <cmath>
#include <cstdlib>
#include <ostream>
#include <utility>
namespace Neon {
template <unsigned int N> class SquareMatrix final {
public:
  // Builds the identity matrix
  SquareMatrix() {
    for (unsigned int i = 0; i < N; i++) {
      for (unsigned int j = 0; j < N; j++) {
        m_data[i][j] = (i == j);
      }
    }
  }

  SquareMatrix(float data[N][N]) {
    for (unsigned int i = 0; i < N; i++) {
      for (unsigned int j = 0; j < N; j++) {
        m_data[i][j] = data[i][j];
      }
    }
  }

  ~SquareMatrix() = default;

  SquareMatrix<N> operator*(const SquareMatrix<N> &other) const {
    float newData[N][N];
    // Since we are dealing with up to 4x4 matrices, we can use the naive N^3
    // method
    for (unsigned int i = 0; i < N; i++) {
      for (unsigned int j = 0; j < N; j++) {
        newData[i][j] = 0;
        for (unsigned int k = 0; k < N; k++) {
          newData[i][j] += m_data[i][k] * other.m_data[k][j];
        }
      }
    }

    return SquareMatrix<N>(newData);
  }

  float at(unsigned int i, unsigned int j) const { return m_data[i][j]; }
  void set(float val, unsigned int i, unsigned int j) { m_data[i][j] = val; }

  SquareMatrix<N> operator+(const SquareMatrix<N> &other) const {
    float newData[N][N];
    for (unsigned int i = 0; i < N; i++) {
      for (unsigned int j = 0; j < N; j++) {
        newData[i][j] = m_data[i][j] + other.m_data[i][j];
      }
    }

    return SquareMatrix<N>(newData);
  }

  SquareMatrix<N> operator-(const SquareMatrix<N> &other) const {
    float newData[N][N];
    for (unsigned int i = 0; i < N; i++) {
      for (unsigned int j = 0; j < N; j++) {
        newData[i][j] = m_data[i][j] - other.m_data[i][j];
      }
    }

    return SquareMatrix<N>(newData);
  }

private:
  float m_data[N][N];
};

template <unsigned int N> SquareMatrix<N> Transpose(const SquareMatrix<N> &m) {
  float newData[N][N];
  for (unsigned int i = 0; i < N; i++) {
    for (unsigned int j = 0; j < N; j++) {
      newData[i][j] = m.at(j, i);
    }
  }

  return SquareMatrix<N>(newData);
}

template <unsigned int N> float Determinant(const SquareMatrix<N> &m);

template <unsigned int N> float Determinant(const SquareMatrix<N> &m) {
  SquareMatrix<N - 1> sub;
  float det = 0;
  // Inefficient, but we don't currently use N>4 anyway..
  for (unsigned int i = 0; i < N; ++i) {
    // Sub-matrix without row 0 and column i
    for (unsigned int j = 0; j < N - 1; ++j)
      for (unsigned int k = 0; k < N - 1; ++k)
        sub.set(m.at(j + 1, k < i ? k : k + 1), j, k);

    float sign = (i & 1) ? -1 : 1;
    det += sign * m.at(0, i) * determinant(sub);
  }
  return det;
}

template <> inline float Determinant(const SquareMatrix<1> &m) {
  return m.at(0, 0);
}

template <unsigned int N>
inline SquareMatrix<N> Inverse(const SquareMatrix<N> &m) {
  int indxc[N], indxr[N];
  int ipiv[N] = {0};
  float minv[N][N];
  for (unsigned int i = 0; i < N; ++i)
    for (unsigned int j = 0; j < N; ++j)
      minv[i][j] = m.at(i, j);
  for (unsigned int i = 0; i < N; i++) {
    unsigned int irow = 0, icol = 0;
    float big = 0.f;
    // Choose pivot
    for (unsigned int j = 0; j < N; j++) {
      if (ipiv[j] != 1) {
        for (unsigned int k = 0; k < N; k++) {
          if (ipiv[k] == 0) {
            if (std::abs(minv[j][k]) >= big) {
              big = std::abs(minv[j][k]);
              irow = j;
              icol = k;
            }
          } else if (ipiv[k] > 1)
            return {}; // singular
        }
      }
    }
    ++ipiv[icol];
    // Swap rows _irow_ and _icol_ for pivot
    if (irow != icol) {
      for (unsigned int k = 0; k < N; ++k)
        std::swap(minv[irow][k], minv[icol][k]);
    }
    indxr[i] = irow;
    indxc[i] = icol;
    if (minv[icol][icol] == 0.f)
      return {}; // singular

    // Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
    float pivinv = 1. / minv[icol][icol];
    minv[icol][icol] = 1.;
    for (unsigned int j = 0; j < N; j++)
      minv[icol][j] *= pivinv;

    // Subtract this row from others to zero out their columns
    for (unsigned int j = 0; j < N; j++) {
      if (j != icol) {
        float save = minv[j][icol];
        minv[j][icol] = 0;
        for (unsigned int k = 0; k < N; k++)
          minv[j][k] = std::fma(-minv[icol][k], save, minv[j][k]);
      }
    }
  }
  // Swap columns to reflect permutation
  for (int j = N - 1; j >= 0; j--) {
    if (indxr[j] != indxc[j]) {
      for (unsigned int k = 0; k < N; k++)
        std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
    }
  }
  return SquareMatrix<N>(minv);
}

template <unsigned int N>
std::ostream &operator<<(std::ostream &out, const SquareMatrix<N> &m) {
  for (unsigned int i = 0; i < N; i++) {
    out << '[' << m.at(i, 0);
    for (unsigned int j = 1; j < N; j++) {
      out << ", " << m.at(i, j);
    }
    out << "]\n";
  }
  return out;
}

Vector3f operator*(const SquareMatrix<4> &m, const Vector3f &v);

Point3f operator*(const SquareMatrix<4> &m, const Point3f &v);

} // namespace Neon

#endif
