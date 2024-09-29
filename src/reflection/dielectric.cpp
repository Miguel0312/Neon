#include "reflection/dielectric.h"
#include "utils/objectFactory.h"
#include "utils/utils.h"
#include <cmath>

namespace Neon {
Dielectric::Dielectric(const toml::table *table) : BSDF(false) {
  m_refractionIndex = table->get("refractionIndex")->as_floating_point()->get();
  m_refractionIndexInverse = 1 / m_refractionIndex;
}

Color Dielectric::sample(BSDFQueryRecord &query, Sampler *sampler) const {
  int dir = query.wi.z() < 0 ? -1 : 1;
  float eta = dir < 0 ? m_refractionIndex : m_refractionIndexInverse;
  float etaInverse = dir < 0 ? m_refractionIndexInverse : m_refractionIndex;
  Vector3f v(query.wi.x(), query.wi.y(), 0);
  float sinTheta = v.length();
  float sinTheta2 = sinTheta * etaInverse;
  float cosTheta = std::abs(query.wi.z());
  float cosTheta2 = std::sqrt(1 - sinTheta2 * sinTheta2);

  float reflectionCoeff = fresnelReflectionCoeff(cosTheta, cosTheta2, eta);

  if (sampler->next1D() <= reflectionCoeff) {
    // In this case, the photon is reflected
    Vector3f z(0, 0, dir);
    query.wo = query.wi - 2 * query.wi.dot(z) * z;
  } else {
    query.wo = v / sinTheta * sinTheta2 + cosTheta2 * Vector3f(0, 0, dir);
  }

  return Color(1.0f, 1.0f, 1.0f);
}

float Dielectric::pdf(const BSDFQueryRecord &query) const { return 0; }

NEON_REGISTER_CLASS(Dielectric, "dielectric");
} // namespace Neon