#ifndef NEON_ACCELERATOR_H
#define NEON_ACCELERATOR_H

#include "math/ray.h"
#include "scene/shape.h"
#include "utils/objectFactory.h"

namespace Neon {
class Scene;

class Accelerator : public NeonObject {
public:
  Accelerator() : m_scene(nullptr) {}

  virtual ~Accelerator() = default;

  void setScene(Scene *scene) { m_scene = scene; };

  virtual void preprocess() = 0;

  virtual bool rayIntersection(const Ray &r, ShapeIntersectionRecord &rec) = 0;

protected:
  Scene *m_scene;
};
} // namespace Neon

#endif
