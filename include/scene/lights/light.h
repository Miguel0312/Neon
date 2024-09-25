#ifndef NEON_LIGHT_H
#define NEON_LIGHT_H

#include "utils/color.h"
#include "utils/objectFactory.h"
#include "utils/sampling/sampler.h"

namespace Neon {
class Light;
class Scene;

struct LightSampleRecord {
  const Light *light;
  float pdf;
  Point3f p;
  Vector3f n;
};

class Light : public NeonObject {
public:
  Light() = default;

  virtual ~Light() = default;

  virtual Color sample(const Scene *scene, LightSampleRecord &rec,
                       Sampler *sampler) const = 0;

  virtual Color eval(const Scene *scene, LightSampleRecord &rec) const = 0;

  // The (not necessarily normalized) probability that this light source should
  // be chosen among the others on the scene
  virtual float getPDF() const = 0;
};
} // namespace Neon

#endif
