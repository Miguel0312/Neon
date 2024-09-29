#include "scene/lights/areaLight.h"
#include "scene/scene.h"
#include "scene/sceneParser.h"
#include "utils/objectFactory.h"
#include "utils/utils.h"

namespace Neon {
AreaLight::AreaLight(const toml::table *table) {
  m_color = parseColor(table->at("color").as_array());
  m_shape = SceneParser::getSingleton()->getShape(
      table->at("shape").as_string()->get());
  m_shape->setIsLight(true);
  m_shape->setLight(this);

  m_colorNormalized = m_color;

  float maxi = std::max(m_color.r, std::max(m_color.g, m_color.b));
  if (maxi > 1) {
    m_colorNormalized = m_color / maxi;
  }
}

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

  return m_colorNormalized;
}

float AreaLight::getPDF() const { return m_shape->area(); }

NEON_REGISTER_CLASS(AreaLight, "area");
} // namespace Neon
