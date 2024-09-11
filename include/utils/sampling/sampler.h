#ifndef NEON_SAMPLER_H
#define NEON_SAMPLER_H

#include "math/point.h"
#include <memory>
namespace Neon {
class Sampler {
public:
  Sampler() = default;

  virtual ~Sampler() = default;

  // Returns a new sampler with the same internal state
  // Needed to insure reproducibility in the context of a multi threaded
  // rendering
  virtual std::unique_ptr<Sampler> clone() = 0;

  // Returns a random number between 0 and 1
  virtual float next1D() = 0;

  // Returns a random point int the [0,1]^2 domain
  virtual Point2f next2D() = 0;
};
} // namespace Neon

#endif
