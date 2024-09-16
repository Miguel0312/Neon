#ifndef NEON_SCENE_H
#define NEON_SCENE_H

#include "scene/accelerators/accelerator.h"
#include "scene/camera.h"
#include "scene/integrators/integrator.h"
#include "scene/shape.h"
#include "utils/sampling/sampler.h"
#include <memory>
#include <vector>
namespace Neon {
class Scene final {
public:
  Scene() = default;

  ~Scene() = default;

  void addShape(std::unique_ptr<Shape> &shape);

  const std::unique_ptr<Camera> &getCamera() const { return m_camera; }

  const std::vector<std::vector<Neon::Color>> &getPixels() const {
    return m_pixels;
  }

  void setCamera(std::unique_ptr<Camera> &camera);

  void setIntegrator(std::unique_ptr<Integrator> &integrator);

  void setSampler(std::unique_ptr<Sampler> &sampler);

  void setAccelerator(std::unique_ptr<Accelerator> &accelerator);

  void setFilename(const std::string &filename);

  void setSampleCount(int sampleCount) { m_sampleCount = sampleCount; }

  bool rayIntersection(const Ray &r, ShapeIntersectionRecord &rec);

  const std::vector<std::unique_ptr<Shape>> &getShapes() const {
    return m_shapes;
  }

  const BoundingBox &getBoundingBox() const { return m_box; }

  void render();

private:
  std::vector<std::unique_ptr<Shape>> m_shapes;
  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<Integrator> m_integrator;
  std::unique_ptr<Sampler> m_sampler;
  std::unique_ptr<Accelerator> m_accelerator;
  std::string m_filename = "images/test.png";
  std::vector<std::vector<Neon::Color>> m_pixels;
  int m_sampleCount = 64;
  BoundingBox m_box;
};
} // namespace Neon

#endif
