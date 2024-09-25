#include "scene/integrators/whiteBoxIntegrator.h"
#include "math/transform.h"
#include "reflection/bsdf.h"
#include "scene/scene.h"
#include "scene/shape.h"
#include "utils/color.h"

namespace Neon {
WhiteBoxIntegrator::WhiteBoxIntegrator(const toml::table *table) {
  m_maxDepth = table->at("maxDepth").as_integer()->get();
}

Color WhiteBoxIntegrator::Li(Scene *scene, const Ray &r, Sampler *sampler) {
  return Li(scene, r, sampler, 0);
}

Color WhiteBoxIntegrator::Li(Scene *scene, const Ray &r, Sampler *sampler,
                             int depth) {
  if (depth >= m_maxDepth) {
    return Color(0);
  }

  ShapeIntersectionRecord intersectionRec;
  if (!scene->rayIntersection(r, intersectionRec)) {
    // In a white there is a global white light from all directions
    return Color(1.0f);
  }

  BSDFQueryRecord bsdfRec(intersectionRec.frame.worldToLocal(r.dir));
  Color color = intersectionRec.shape->getBSDF()->sample(bsdfRec, sampler);

  Ray newRay(intersectionRec.p, intersectionRec.frame.localToWorld(bsdfRec.wo));

  Color result = Li(scene, newRay, sampler, depth + 1);
  return color * result;
  return color * Li(scene, newRay, sampler, depth + 1);
}

NEON_REGISTER_CLASS(WhiteBoxIntegrator, "whitebox");
} // namespace Neon
