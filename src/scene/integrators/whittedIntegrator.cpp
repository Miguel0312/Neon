#include "scene/integrators/whittedIntegrator.h"
#include "math/transform.h"
#include "reflection/bsdf.h"
#include "scene/scene.h"
#include "scene/shape.h"
#include "utils/color.h"
#include <iostream>

namespace Neon {
WhittedIntegrator::WhittedIntegrator(const toml::table *table) {
  m_maxDepth = table->at("maxDepth").as_integer()->get();
}

Color WhittedIntegrator::Li(Scene *scene, const Ray &r, Sampler *sampler) {
  return Li(scene, r, sampler, 0);
}

Color WhittedIntegrator::Li(Scene *scene, const Ray &r, Sampler *sampler,
                            int depth) {
  if (depth >= m_maxDepth) {
    return Color(0);
  }

  ShapeIntersectionRecord intersectionRec;
  if (!scene->rayIntersection(r, intersectionRec)) {
    return Color(0.0f);
  }

  if (intersectionRec.shape->isLight()) {
    LightSampleRecord lightRec;
    lightRec.p = intersectionRec.p;
    return intersectionRec.shape->getLight()->eval(scene, lightRec);
  }

  LightSampleRecord lightRec;
  Color lightColor = scene->sampleLight(lightRec, sampler);

  // Check that there are no objects between the object and the light
  // TODO: enable the shadow ray option here
  Ray shadowRay(lightRec.p, (intersectionRec.p - lightRec.p).normalized());
  ShapeIntersectionRecord shadowRayIntersection;
  scene->rayIntersection(shadowRay, shadowRayIntersection);
  if (intersectionRec.shape != shadowRayIntersection.shape) {
    return 0.0f;
  }

  BSDFQueryRecord bsdfRec(intersectionRec.frame.worldToLocal(r.dir));
  Color color = intersectionRec.shape->getBSDF()->sample(bsdfRec, sampler);

  Vector3f lightDir = lightRec.p - intersectionRec.p;
  float dist = lightDir.lengthSq();
  lightDir.normalize();

  float g = -intersectionRec.n.dot(lightDir) * lightRec.n.dot(lightDir);

  return g * color * lightColor / (dist * lightRec.pdf);
}

NEON_REGISTER_CLASS(WhittedIntegrator, "whitted");
} // namespace Neon
