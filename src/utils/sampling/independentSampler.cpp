#include "utils/sampling/independentSampler.h"
#include <memory>
#include <random>

namespace Neon {
IndependentSampler::IndependentSampler(int seed)
    : Sampler(), m_generator(seed), m_distribution(0.0f, 1.0f), m_seed(seed),
      m_generationCount(0) {}

IndependentSampler::IndependentSampler(const toml::table *table)
    : m_distribution(0.0f, 1.0f) {
  auto seedEntry = table->get("seed");
  if (seedEntry != nullptr) {
    m_seed = seedEntry->as_integer()->get();
  } else {
    m_seed = 1;
  }

  m_generator = std::mt19937(m_seed);
}

std::unique_ptr<Sampler> IndependentSampler::clone() {
  std::unique_ptr<IndependentSampler> newSampler =
      std::make_unique<IndependentSampler>(m_seed);

  newSampler->m_generator.discard(m_generationCount);

  return newSampler;
}

float IndependentSampler::next1D() {
  m_generationCount += 1;
  return m_distribution(m_generator);
}

Point2f IndependentSampler::next2D() {
  m_generationCount += 2;
  return Point2f(m_distribution(m_generator), m_distribution(m_generator));
}

NEON_REGISTER_CLASS(IndependentSampler, "independent");

} // namespace Neon
