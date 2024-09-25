#ifndef WHITTED_INTEGRATOR_H
#define WHITTED_INTEGRATOR_H

#include "integrator.h"

namespace Neon {
class WhittedIntegrator final : public Integrator {
public:
  WhittedIntegrator() = delete;

  WhittedIntegrator(const toml::table *table);

  WhittedIntegrator(int maxDepth) : m_maxDepth(maxDepth) {}

  ~WhittedIntegrator() = default;

  Color Li(Scene *scene, const Ray &r, Sampler *sampler) override;

  Color Li(Scene *scene, const Ray &r, Sampler *sampler, int depth);

private:
  int m_maxDepth;
};
} // namespace Neon

#endif
