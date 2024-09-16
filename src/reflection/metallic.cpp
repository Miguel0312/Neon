#include "reflection/metallic.h"
#include "utils/consts.h"

namespace Neon {
Color Metallic::sample(BSDFQueryRecord &query, Sampler *sampler) const {
  Vector3f z(0, 0, 1);
  query.wo = query.wi - 2 * query.wi.dot(z) * z;
  // std::cout << query.wo << std::endl;

  // Returns m_color * cos(theta) / pdf, but cos(theta) = pdf in this case
  return m_albedo;
}

float Metallic::pdf(const BSDFQueryRecord &query) const {
  // The pdf follows a Dirac's delta distribution, so it is 0 almost everywhere
  // We avoid returning infinity for values close to the correct answer, since
  // it has no physical meaning
  return 0;
}
} // namespace Neon
