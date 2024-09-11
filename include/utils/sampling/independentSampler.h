#ifndef NEON_INDEPENDENT_SAMPLER_H
#define NEON_INDEPENDENT_SAMPLER_H

#include "sampler.h"
#include <random>

namespace Neon {
class IndependentSampler final : public Sampler {
public:
  IndependentSampler(int seed = 1);

  ~IndependentSampler() = default;

  std::unique_ptr<Sampler> clone() override;

  float next1D() override;

  Point2f next2D() override;

private:
  std::mt19937 m_generator;
  std::uniform_real_distribution<float> m_distribution;
  int m_seed, m_generationCount;
};
} // namespace Neon

#endif
