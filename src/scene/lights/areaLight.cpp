#include "scene/lights/areaLight.h"
#include "scene/scene.h"

namespace Neon {
Color AreaLight::sample(const Scene *scene, LightSampleRecord &rec,
                        Sampler *sampler) const {
  rec.light = this;
  rec.pdf = 1 / scene->getLightPDFSum();
  m_shape->sample(rec.p, rec.n, sampler);

  return m_color;
}

Color AreaLight::eval(const Scene *scene, LightSampleRecord &rec) const {
  rec.light = this;
  rec.pdf = 1 / scene->getLightPDFSum();
  rec.n = 1 / m_shape->normalAt(rec.p);

  return m_color;
}

float AreaLight::getPDF() const { return m_shape->area(); }
} // namespace Neon
