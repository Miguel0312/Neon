#include "math/point.h"
#include "math/vector.h"
#include "thirdparty/toml++/toml.h"
#include "utils/color.h"

namespace Neon {
inline Point3f parsePoint(const toml::array *node) {
  return Point3f(node->at(0).as_floating_point()->get(),
                 node->at(1).as_floating_point()->get(),
                 node->at(2).as_floating_point()->get());
}

inline Vector3f parseVector(const toml::array *node) {
  return Vector3f(node->at(0).as_floating_point()->get(),
                  node->at(1).as_floating_point()->get(),
                  node->at(2).as_floating_point()->get());
}

inline Color parseColor(const toml::array *node) {
  return Color(node->at(0).as_floating_point()->get(),
               node->at(1).as_floating_point()->get(),
               node->at(2).as_floating_point()->get());
}

inline float toRadians(float x) { return x * M_PI / 180; }
} // namespace Neon