#ifndef NEON_INTEGRATOR_H
#define NEON_INTEGRATOR_H

#include "math/ray.h"
#include "utils/color.h"
#include "utils/sampling/sampler.h"
namespace Neon {
class Scene;

class Integrator {
public:
  Integrator() = default;

  ~Integrator() = default;

  virtual Color Li(Scene *scene, const Ray &ray, Sampler *sampler) = 0;
};
} // namespace Neon

#endif
