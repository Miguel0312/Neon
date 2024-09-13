#include "reflection/lambertian.h"
#include "utils/consts.h"
#include "utils/sampling/warp.h"

namespace Neon {
Color Lambertian::sample(BSDFQueryRecord &query, Sampler *sampler) const {
  query.wo = Warp::squareToCosineHemisphere(sampler->next2D());

  // Returns m_color * cos(theta) / pdf, but cos(theta) = pdf in this case
  return m_albedo;
}

float Lambertian::pdf(const BSDFQueryRecord &query) const {
  // Since in this frame the z axis coincides with the normal, cosTheta = wo.z
  return query.wo.z() * INV_PI;
}
} // namespace Neon
