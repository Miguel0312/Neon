#ifndef NEON_SCENE_H
#define NEON_SCENE_H

#include "scene/camera.h"
#include "scene/integrators/integrator.h"
#include "scene/shape.h"
#include <memory>
#include <vector>
namespace Neon {
class Scene final {
public:
  Scene() = default;

  ~Scene() = default;

  void addShape(std::unique_ptr<Shape> &shape);

  void setCamera(std::unique_ptr<Camera> &camera);

  void setIntegrator(std::unique_ptr<Integrator> &integrator);

  void setFilename(const std::string &filename);

  bool rayIntersection(const Ray &r, ShapeIntersectionRecord &rec);

  void render();

private:
  std::vector<std::unique_ptr<Shape>> m_shapes;
  std::unique_ptr<Camera> m_camera;
  std::unique_ptr<Integrator> m_integrator;
  std::string m_filename = "images/test.png";
};
} // namespace Neon

#endif
