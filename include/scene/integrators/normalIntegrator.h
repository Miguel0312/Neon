#ifndef NEON_NORMAL_INTEGRATOR_H
#define NEON_NORMAL_INTEGRATOR_H

#include "scene/integrators/integrator.h"
#include "utils/color.h"

namespace Neon {
class NormalIntegrator final : public Integrator {
public:
  NormalIntegrator() = default;

  ~NormalIntegrator() = default;

  Color Li(Scene *scene, const Ray &r) override;
};
} // namespace Neon

#endif
