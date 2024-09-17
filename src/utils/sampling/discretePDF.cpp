#include "utils/sampling/discretePDF.h"
#include <algorithm>
#include <iostream>

namespace Neon {
void DiscretePDF::add(float weight) {
  m_weights.push_back(m_weights[m_weights.size() - 1] + weight);
}

unsigned int DiscretePDF::sample(float sampleValue) const {
  float val = m_weights[m_weights.size() - 1] * sampleValue;
  auto it = std::upper_bound(m_weights.begin(), m_weights.end(), val);
  return it - m_weights.begin() - 1;
}
} // namespace Neon
