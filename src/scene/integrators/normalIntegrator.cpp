#include "math/vector.h"
#include "scene/shape.h"
#include <scene/integrators/normalIntegrator.h>
#include <scene/scene.h>

namespace Neon {
Color NormalIntegrator::Li(Scene *scene, const Ray &r, Sampler *sampler) {
  ShapeIntersectionRecord rec;
  if (scene->rayIntersection(r, rec)) {
    // Convert the interval [-1,1] to [0,1]
    Vector3f v = 0.5 * (rec.n + Vector3f(1));

    return Color(v[0], v[1], v[2]);
  }

  return Color(0);
}
} // namespace Neon
