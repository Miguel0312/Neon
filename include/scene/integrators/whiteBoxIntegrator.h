#ifndef WHITE_BOX_INTEGRATOR_H
#define WHITE_BOX_INTEGRATOR_H

#include "integrator.h"

namespace Neon {
class WhiteBoxIntegrator final : public Integrator {
public:
  WhiteBoxIntegrator(const toml::table *table);

  WhiteBoxIntegrator() = delete;

  WhiteBoxIntegrator(int maxDepth) : m_maxDepth(maxDepth) {}

  ~WhiteBoxIntegrator() = default;

  Color Li(Scene *scene, const Ray &r, Sampler *sampler) override;

  Color Li(Scene *scene, const Ray &r, Sampler *sampler, int depth);

private:
  int m_maxDepth;
};
} // namespace Neon

#endif
