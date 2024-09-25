#include "reflection/metallic.h"
#include "utils/objectFactory.h"
#include "utils/utils.h"

namespace Neon {
Metallic::Metallic(const toml::table *table) {
  m_albedo = parseColor(table->at("albedo").as_array());
}

Color Metallic::sample(BSDFQueryRecord &query, Sampler *sampler) const {
  Vector3f z(0, 0, 1);
  query.wo = query.wi - 2 * query.wi.dot(z) * z;

  return m_albedo;
}

float Metallic::pdf(const BSDFQueryRecord &query) const {
  // The pdf follows a Dirac's delta distribution, so it is 0 almost everywhere
  // We avoid returning infinity for values close to the correct answer, since
  // it has no physical meaning
  return 0;
}

NEON_REGISTER_CLASS(Metallic, "metallic");
} // namespace Neon
