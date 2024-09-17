#ifndef NEON_LIGHT_H
#define NEON_LIGHT_H

#include "math/ray.h"
#include "utils/color.h"
#include "utils/sampling/sampler.h"

namespace Neon {
class Light;

struct LightSampleRecord {
  const Light *light;
  float pdf;
  Point3f p;
  Vector3f n;
};

class Light {
public:
  Light() = default;

  virtual ~Light() = default;

  virtual Color sample(LightSampleRecord &rec, Sampler *sampler) const = 0;

  virtual Color eval(LightSampleRecord &rec) const = 0;
};
} // namespace Neon

#endif
