#ifndef NEON_AREA_LIGHT_H
#define NEON_AREA_LIGHT_H

#include "light.h"
#include "scene/shape.h"

namespace Neon {
class AreaLight final : public Light {
public:
  AreaLight(Shape *shape, Color color) : m_shape(shape), m_color(color) {
    shape->setIsLight(true);
    shape->setLight(this);
    m_colorNormalized = m_color;

    float maxi = std::max(m_color.r, std::max(m_color.g, m_color.b));
    if (maxi > 1) {
      m_colorNormalized = m_color / maxi;
    }
  }

  AreaLight(const toml::table *table);

  ~AreaLight() = default;

  Color sample(const Scene *scene, LightSampleRecord &rec,
               Sampler *sampler) const override;

  Color eval(const Scene *scene, LightSampleRecord &rec) const override;

  float getPDF() const override;

private:
  Shape *m_shape;
  Color m_color;
  Color m_colorNormalized;
};
} // namespace Neon

#endif
