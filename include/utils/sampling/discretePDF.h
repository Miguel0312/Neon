#ifndef NEON_DISCRETE_PDF_H
#define NEON_DISCRETE_PDF_H

#include <vector>
namespace Neon {
class DiscretePDF final {
public:
  DiscretePDF() { m_weights.push_back(0); }

  DiscretePDF(unsigned int n) {
    m_weights.reserve(n + 1);
    m_weights.push_back(0);
  }

  unsigned int size() { return m_weights.size(); }

  void add(float weight);

  unsigned int sample(float sampleValue) const;

private:
  std::vector<float> m_weights;
};
} // namespace Neon

#endif
