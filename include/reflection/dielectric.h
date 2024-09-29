#ifndef NEON_DIELECTRIC_H
#define NEON_DIELECTRIC_H

#include "reflection/bsdf.h"
namespace Neon {
class Dielectric final : public BSDF {
public:
  Dielectric() = delete;

  Dielectric(const toml::table *table);

  Dielectric(float refractionIndex)
      : BSDF(false), m_refractionIndex(refractionIndex) {
    m_refractionIndexInverse = 1 / m_refractionIndex;
  }

  Color sample(BSDFQueryRecord &query, Sampler *sampler) const override;

  float pdf(const BSDFQueryRecord &query) const override;

private:
  float m_refractionIndex;
  // Cache 1 / m_refractionIndex
  float m_refractionIndexInverse;
};
} // namespace Neon

#endif