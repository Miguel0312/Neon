#include "math/matrix.h"

namespace Neon {
// TODO: put these methods on the transform class
Vector3f operator*(const SquareMatrix<4> &m, const Vector3f &v) {
  Vector3f res(0);
  // Ignore the last colum because vectors can't be translated
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      res[i] += m.at(i, j) * v[j];
    }
  }

  return res;
}

Point3f operator*(const SquareMatrix<4> &m, const Point3f &v) {
  Point3f res(0);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      res[i] += m.at(i, j) * v[j];
    }
    // Add a fourth element to the point that is equal to 1 to allow
    // translations
    res[i] += m.at(i, 4) * 1;
  }

  return res;
}
} // namespace Neon
