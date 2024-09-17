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
  }

  ~AreaLight() = default;

  Color sample(const Scene *scene, LightSampleRecord &rec,
               Sampler *sampler) const override;

  Color eval(const Scene *scene, LightSampleRecord &rec) const override;

  float getPDF() const override;

private:
  Shape *m_shape;
  Color m_color;
};
} // namespace Neon

#endif
