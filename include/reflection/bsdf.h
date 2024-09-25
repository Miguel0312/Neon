#ifndef NEON_BSDF_H
#define NEON_BSDF_H

#include "math/vector.h"
#include "utils/color.h"
#include "utils/objectFactory.h"
#include "utils/sampling/sampler.h"
namespace Neon {
struct BSDFQueryRecord {
  // wi should be passed in a frame in which the normal to the object at the
  // point of contact points to (0,0,1)
  Vector3f wi;

  // wo os in a frame in which the normal to the object at the
  // point of contact points to (0,0,1)
  Vector3f wo;

  BSDFQueryRecord() = delete;

  BSDFQueryRecord(Vector3f wi) : wi(wi) {}
};

class BSDF : public NeonObject {
public:
  BSDF() = default;

  virtual ~BSDF() = default;

  // The cosine factor and the division by the pdf are already included on the
  // return value
  virtual Color sample(BSDFQueryRecord &query, Sampler *sampler) const = 0;

  virtual float pdf(const BSDFQueryRecord &query) const = 0;
};
} // namespace Neon

#endif
