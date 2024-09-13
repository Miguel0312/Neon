#ifndef NEON_LAMBERTIAN_H
#define NEON_LAMBERTIAN_H

#include "bsdf.h"

namespace Neon {
class Lambertian final : public BSDF {
public:
  Lambertian() = delete;

  Lambertian(Color albedo) : m_albedo(albedo) {}

  ~Lambertian() = default;

  // The cosine factor is already included on the calculation of Color
  Color sample(BSDFQueryRecord &query, Sampler *sampler) const override;

  float pdf(const BSDFQueryRecord &query) const override;

private:
  Color m_albedo;
};
} // namespace Neon

#endif
