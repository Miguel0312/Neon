#include "scene/lights/areaLight.h"

namespace Neon {
Color AreaLight::sample(LightSampleRecord &rec, Sampler *sampler) const {
  rec.light = this;
  rec.pdf = 1 / m_shape->area();
  m_shape->sample(rec.p, rec.n, sampler);

  return m_color;
}

Color AreaLight::eval(LightSampleRecord &rec) const {
  rec.light = this;
  rec.pdf = 1 / m_shape->area();
  rec.n = 1 / m_shape->normalAt(rec.p);

  return m_color;
}
} // namespace Neon
