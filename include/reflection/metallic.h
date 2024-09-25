#ifndef NEON_METALLIC_H
#define NEON_METALLIC_H

#include "bsdf.h"

namespace Neon {
class Metallic final : public BSDF {
public:
  Metallic() = delete;

  Metallic(const toml::table *table);

  Metallic(Color albedo) : m_albedo(albedo) {}

  ~Metallic() = default;

  Color sample(BSDFQueryRecord &query, Sampler *sampler) const override;

  float pdf(const BSDFQueryRecord &query) const override;

private:
  Color m_albedo;
};
} // namespace Neon

#endif
