#include "scene/integrators/whittedIntegrator.h"
#include "math/transform.h"
#include "math/vector.h"
#include "reflection/bsdf.h"
#include "scene/scene.h"
#include "scene/shape.h"
#include "utils/color.h"
#include <iostream>

namespace Neon {
Color WhittedIntegrator::Li(Scene *scene, const Ray &r, Sampler *sampler) {
  return Li(scene, r, sampler, 0);
}

Color WhittedIntegrator::Li(Scene *scene, const Ray &r, Sampler *sampler,
                            int depth) {
  static int maxDepth = 0;
  if (depth > maxDepth) {
    std::cout << depth << std::endl;
    maxDepth = depth;
  }
  if (depth >= m_maxDepth) {
    return Color(0);
  }

  ShapeIntersectionRecord intersectionRec;
  if (!scene->rayIntersection(r, intersectionRec)) {
    // For now, assume that there is a global white light from all directions
    return Color(1.0f);
  }

  BSDFQueryRecord bsdfRec(intersectionRec.frame.worldToLocal(r.dir));
  Color color = intersectionRec.shape->getBSDF()->sample(bsdfRec, sampler);

  Ray newRay(intersectionRec.p, intersectionRec.frame.localToWorld(bsdfRec.wo));

  Color result = Li(scene, newRay, sampler, depth + 1);
  return color * result;
  return color * Li(scene, newRay, sampler, depth + 1);
}
} // namespace Neon
