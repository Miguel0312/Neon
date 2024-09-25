#ifndef NEON_NORMAL_INTEGRATOR_H
#define NEON_NORMAL_INTEGRATOR_H

#include "scene/integrators/integrator.h"
#include "utils/color.h"

namespace Neon {
class NormalIntegrator final : public Integrator {
public:
  NormalIntegrator(const toml::table *table);

  NormalIntegrator() = default;

  ~NormalIntegrator() = default;

  Color Li(Scene *scene, const Ray &r, Sampler *sampler) override;
};
} // namespace Neon

#endif
